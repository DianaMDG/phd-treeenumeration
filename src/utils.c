#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "utils.h"

#ifdef USECOLOURS
    #define BOLD    "\033[1m"
    #define UND     "\033[4m"
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define YELLOW  "\033[33m"
    #define BLUE    "\033[34m"
    #define PURPLE  "\033[35m"
    #define CYAN    "\033[36m"
    #define RESET   "\033[0m"

    #define INFOT   PURPLE BOLD

    #define WARNT   YELLOW BOLD

    #define ERRT    RED BOLD
#else
    #define BOLD    ""
    #define UND     ""
    #define RED     ""
    #define GREEN   ""
    #define YELLOW  ""
    #define BLUE    ""
    #define PURPLE  ""
    #define CYAN    ""
    #define ERRT    ""
    #define ERRM    ""

    #define RESET   ""
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                                             Signal Handling
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void    sig_handler(int signo);
void    (*sigint_f)(void);
void    (*sigint_e)(void);
void    (*sigint_q)(void);
double  signal_time;
struct timeval timelast;



void signal_handle_sigint(void (*f)(void), void (*e)(void), double time) {
    sigint_f = f;
    sigint_e = e;
    signal_time = time;
    signal(SIGINT, sig_handler);
    gettimeofday(&timelast, NULL);
}

void signal_handle_sigquit(void (*q)(void)) {
    sigint_q = q;
    signal(SIGQUIT, sig_handler);
}



void sig_handler(int signo) {
    /*
     * This handle signals
     */
    if (signo == SIGINT) {
        /* count time */
        struct timeval timenow;
        gettimeofday(&timenow, NULL);
        double last = timelast.tv_sec + (timelast.tv_usec / 10.0e6);
        double now  = timenow.tv_sec + (timenow.tv_usec / 10.0e6);
        timelast = timenow;

        printf("\r      \r");
        /* if too much time, call sigint_e */
        if ((now - last) < signal_time) {
            if (sigint_e != NULL) sigint_e();
            printf("^C terminated by SIGINT\n");
            exit(0);
        } else
            if (sigint_f != NULL) sigint_f();

        signal(SIGINT, sig_handler);
    }
    else if (signo == SIGQUIT) {
        printf("\r      \r");

        if (sigint_q != NULL) sigint_q();

        printf("^\\ terminated by SIGQUIT\n");
        exit(0);
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                                             TIME MEASUREMENT
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 /*
 * Time measurement
 */
#define TIMEDESC 1024
typedef struct times {
    struct ttick    *tticks;                /* the time measurements */
    int             size;                   /* the size of above array */
    int             count;                  /* how many used untill now */
} times;

typedef struct ttick {
    char            desc[TIMEDESC];         /* a description of the this measurement */
    double          ticku;                  /* the actual time tick: user time */
    double          ticks;                  /* the actual time tick: sys  time */
    char            skip;                   /* -1: init; 0: tick; 1: skip the measurement */
} ttick;


times * times_init(int initsize) {
    times * t = (times *) malloc(sizeof(times));

    initsize = initsize > 0 ? initsize : 1;

    t->tticks = (ttick *) malloc(sizeof(ttick) * initsize);
    t->size   = initsize;
    t->count   = 0;

    /* the initial tick */
    times_tick(t, "");
    t->tticks[t->count - 1].skip = -1;

    return t;
}

void times_tick(times * t, char * desc) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    /* if we reach the maximum size, allocate more! */
    if (t->count == t->size) {
        ttick    *ntt = (ttick *) malloc(sizeof(ttick) * t->size * 2);
        memcpy(ntt, t->tticks, sizeof(ttick) * t->size );
        free(t->tticks);
        t->tticks = ntt;

        t->size *= 2;
    }

    t->tticks[t->count].ticku = usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 10.0e6);
    t->tticks[t->count].ticks = usage.ru_stime.tv_sec + (usage.ru_stime.tv_usec / 10.0e6);

    if (desc == NULL)
        sprintf(t->tticks[t->count].desc, "step %d", t->count);
    /*else if (desc[0] == '\0')
        t->tticks[t->count].desc[0] = '\0';*/
    else
        sprintf(t->tticks[t->count].desc, "'%s'", desc);

    t->tticks[t->count].skip = 0;
    t->count++;
}

void times_zero(times * t) {
    times_tick(t, "");
    t->tticks[t->count - 1].skip = 1;
}

void times_print(times * t, int verbose) {
    /* if total counts is 0, creates a new tick before print */
    if (t->count <= 1) {
        times_tick(t, "");
        verbose = 0;
    }
    int i;
    char tmp[256 + TIMEDESC];
    double elapsedu, elapseds,
           telapsedu = 0.0, telapseds = 0.0;

    /* compute the maximum string lengh */
    int maxmargin = 0;
    for (i = 1; i < t->count; i++) {
        int m = strlen(t->tticks[i].desc);
            maxmargin = m > maxmargin ? m : maxmargin;
    }
    maxmargin += 8;

    /* count total */
    for (i = 1; i < t->count; i++) {
        if (t->tticks[i].skip) {
            continue;
        }

        elapsedu = t->tticks[i].ticku - t->tticks[i - 1].ticku;
        elapseds = t->tticks[i].ticks - t->tticks[i - 1].ticks;

        telapsedu += elapsedu;
        telapseds += elapseds;
    }

    int maxd = 6;
    int maxs = (int) log10(telapsedu+telapseds) + 1 + maxd + 1;

    if (verbose)
        for (i = 1; i < t->count; i++) {
            if (t->tticks[i].skip) {
                continue;
            }

            elapsedu = t->tticks[i].ticku - t->tticks[i - 1].ticku;
            elapseds = t->tticks[i].ticks - t->tticks[i - 1].ticks;

            sprintf(tmp, "Time in %s", t->tticks[i].desc);
            printf("%-*s: %*.*fs (user %*.*fs + sys %*.*fs)\n", maxmargin, tmp, maxs, maxd, elapsedu + elapseds, maxs, maxd, elapsedu, maxs, maxd, elapseds);
        }

    /* Total time */
    sprintf(tmp, "Total time");
    printf(BOLD"%-*s"RESET": "BOLD"%*.*fs"RESET" (user %*.*fs + sys %*.*fs)\n", maxmargin, tmp, maxs, maxd, telapsedu + telapseds, maxs, maxd, telapsedu, maxs, maxd, telapseds);
}

void times_print_elapsed(times * t) {
    times_tick(t, "");
    int i;
    char tmp[256 + TIMEDESC];
    double elapsedu, elapseds,
           telapsedu = 0.0, telapseds = 0.0;

    /* count total */
    for (i = 1; i < t->count; i++) {
        if (t->tticks[i].skip) {
            continue;
        }

        elapsedu = t->tticks[i].ticku - t->tticks[i - 1].ticku;
        elapseds = t->tticks[i].ticks - t->tticks[i - 1].ticks;

        telapsedu += elapsedu;
        telapseds += elapseds;
    }
    t->count--;

    int maxmargin = 8;
    int maxd = 6;
    int maxs = (int) log10(telapsedu+telapseds) + 1 + maxd + 1;

    /* time elapsed until now */
    sprintf(tmp, "Time elapsed");
    printf(BOLD"%-*s"RESET": "BOLD"%*.*fs"RESET" (user %*.*fs + sys %*.*fs)\n", maxmargin, tmp, maxs, maxd, telapsedu + telapseds, maxs, maxd, telapsedu, maxs, maxd, telapseds);
}



