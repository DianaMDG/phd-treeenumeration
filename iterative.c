#include "tree.h"
#include "iterative.h"

/************************************************************************
*
*                       FUNCTION Definition
*
************************************************************************/

/************************************************************************
*                  Functions for the Iterative Case
*************************************************************************/

void generate_seq_iterative(int *generated) {
    /*Iterative function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
    /*NOTE: may be outdated */

    int i, j;
    int div, res;
    int base = SIZE - 2;
    /*int index;*/

    #if N == 15
        for (i = 0; i < CORTE; i++) {
    #else
        for (i = 0; i < pow((SIZE - 2), (SIZE - 4)); i++) {
    #endif
        /*index = 0;*/
        div = i / base;
        res = i % base;
        generated [SIZE - 5] = res + 3;   /*3 comes from shifting the nodes not in the Super Node*/

        for (j = SIZE - 6; j > 1; j--) {
        /*while (div != 0 || res != 0) {*/
            res = div % base;
            div = div / base;
            generated [j] = res + 3; /*again, 3 ... */
        }

        /*just in case some missed*/
        /*for (j = index; j < SIZE - 4; j++) {
            generated [j] = 3;
        }*/

        #ifdef NUMBERS
            SCount++;
        #endif

        /*int j; for (j = 0; j < SIZE - 4; j++) printf("%s %d %s", j == 0 ? "\nNew sequence: [" : "", generated[j], j < SIZE - 5 ? ",\0" : "]\n");*/
        #ifdef TOFILE
            int j; for (j = 0; j < SIZE - 4; j++) fprintf(f_list, "%s %d %s", j == 0 ? "[" : "", generated[j], j < SIZE - 5 ? ",\0" : "]\n");
        #endif
        /*Generate tree associated to the sequence generated*/
        generate_tree(generated);
    }
}



static void unfold_SN_list_iterative() {
    /*Iterative function used to generate combinations of connections to super node*/

    int i = powb3[list_index[SIZE]];
    int j;
    int div, res;
    int index; /*index do no ligado ao super no*/
    /*int nodes[3] = {list_index[0], list_index[1], list_index[2]};*/

    /*for (i = 0; i < powb3[list_index[SIZE]]; i++) {*/
    while (i--) {
        index = 0;                  /*restarting from the first node*/
        for (j = 0, div = i; j < list_index[SIZE]; j++) {
            res = div % 3;
            div = div / 3;
            list[res][list_index[res]++] = list[SIZE][index++];
        }
        #ifdef NUMBERS
            CCount++;
        #endif
        #ifdef GRAPH
            generate_graph();
        #endif

        /*reseting the list indexes */
        list_index[0] = 2;
        list_index[1] = 0;
        list_index[2] = 0;
    }
}

