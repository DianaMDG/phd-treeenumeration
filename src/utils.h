#define USECOLOURS              /* if to use colours */

/*
 * Signal handling
 */
/* f: called whenever control-c; e: called before quits; time: time between two control-c to quit in s */
void            signal_handle_sigint(void (*f)(void), void (*e)(void), double time);

/* q: called whenever control-\ */
void            signal_handle_sigquit(void (*q)(void));


/*
 * Time measurement
 */
/* anonymous struct */
typedef struct times times;

/* init where to store time measurements data. maxmes: estimated measurements ticks */
times *         times_init(int maxmes);

/* make a measurement and give a desc, if you want */
void            times_tick(times * t, char * desc);

/* reset counting / forget last elapsed time */
void            times_zero(times * t);

/* print total time. verbose = 1 prints every tick and verbose = 0 prints the total */
void            times_print(times * t, int verbose);

/* print total elapsed time until now */
void            times_print_elapsed(times * t);


