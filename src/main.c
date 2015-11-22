#include <stdlib.h>
#include <stdio.h>

#include "tree.h"
#include "utils.h"

/* where to save time measurements */
times * timemes;

void this_functions_is_called_when_control_c() {
    times_print_elapsed(timemes);

    printf("this_functions_is_called_when_control_c()\n");
}
void this_will_be_called_if_control_c_speedy_gonzales() {
    times_print(timemes, 1);

    printf("this_will_be_called_if_control_c_speedy_gonzales()\n");
}

void this_will_be_called_before_quit_by_control_slash() {
    times_print(timemes, 1);

    printf("this_will_be_called_before_quit_by_control_slash()\n");
}


int main(int argc, char *argv[]) {

    signal_handle_sigint(this_functions_is_called_when_control_c,
                         this_will_be_called_if_control_c_speedy_gonzales,
                         0.5);

    signal_handle_sigquit(this_will_be_called_before_quit_by_control_slash);


    timemes = times_init(2);
    program();
    times_tick(timemes, "finished program()");

    times_print(timemes, 1);

    return 0;

}
