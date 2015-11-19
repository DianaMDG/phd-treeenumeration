#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "tree.h"
#include "neutral_rep.h"
#ifdef ITERATIVE
    #include "iterative.h"
#endif

/******************************************************************
*
*              PARAMETER Declaration & Definition
*
******************************************************************/

/*NOTE
 N    : Number of bits used by the representation, N used in the neutral network NN(n,k)
 SIZE : Number of syndromes, number of nodes in the tree
 K    : Number of information bits
 G    : Generator polynomial for the codes*/

/*Define the value of N. IF N == 15, DEFINE THE CUTTING LEVEL!!!!!*/
#define N       7
#define LEVEL   2

/*Data for the NN(7, 4) codes*/
#if N == 7
    #define SIZE    (N + 1)
    #define K       (N - 3)
    #define G       ((uint16_t) 0xb)

/*Data for the NN(15, 11) codes*/
#elif N == 15
    #define SIZE    (N + 1)
    #define K       (N - 4)
    #define G       ((uint16_t) 0x13)
    #define CORTE   corte[LEVEL]
    int corte[4] = {28672, 2480058, 58720256, 683593750};
#else
    #pragma message "PLEASE CHOOSE VALUE OF N FROM {7, 15}!!"
#endif

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/******************************************************************
*
*          GLOBAL VARIABLES Declaration & Definition
*
******************************************************************/

int list[SIZE + 1][SIZE - 1] = {{1, 2}};      /*Adjacency list*/
int list_index[SIZE + 1]     = {2};           /*Indexes to access the next list index of a given node to be added*/
int parent[SIZE]             = {-1, 0, 0};    /*Parent array*/

uint16_t ZAux[SIZE] = {0};            /*Auxiliar Zeros with each syndrome with Hamming distance 1 from 0 for computing representation zeros*/
uint16_t Z[SIZE]    = {0, 1, 2};      /*Zeros of the representation*/

#ifdef NUMBERS
    unsigned long long int CCount      = 0;     /*Number of generated codes*/
    unsigned long long int CCount1     = 0;     /*Number of something TODO*/
    unsigned long      int SCount      = 0;     /*Number of generated Sequences*/
    unsigned           int CCutsPrim   = 0;     /*Number of representations the trees of which are NOT canonical*/
    unsigned           int FinalsPrim  = 0;     /*Number of representations the trees of which are canonical*/
    unsigned           int CCutsConst  = 0;     /*Number of representations which are NOT minimal regarding constants*/
    unsigned           int FinalsConst = 0;     /*Number of representations which are minimal regarding constants*/
#endif

#ifdef TOFILE
    FILE *f_list;               /*File with the list of Prüfer sequences generated TODO name*/
    #ifdef GRAPH
        FILE *f_cut;            /*File with the list of representations that do NOT fulfill the requirements of the program*/
        FILE *f_pass;           /*File with the list of representations that fulfill all requirements of the program*/
    #endif
#endif

#ifdef ITERATIVE
    int powb3[13];              /*Array used for the iterative case*/
#endif


/******************************************************************
*
*                             MAIN
*
******************************************************************/
int main(int argc, char *argv[]) {

    #ifdef ITERATIVE
        int i;
        for (i = 0; i < 13; i++) {
            powb3[i] = pow(3, i);
        }
    #endif

    #ifdef TOFILE
        f_list = fopen("list_representations.txt", "w");
        #ifdef GRAPH
            f_cut = fopen("cut.txt", "w"); 
            f_pass = fopen("pass.txt", "w");
        #endif
    #endif

    int j;
    uint16_t c = 0, b, t;

    /*Generates array of words with each syndrome which have */
    for (j = 0; j < N; j++) {
        b = c ^ (uint16_t)(1 << j);
        t = syndrome(N, K, G, b);
        ZAux[t] = b;
    }
    /*print the complementary array */
    /*for(j = 1; j < SIZE; j++) printf("%s %d %s", j == 1 ? "ZAux : " : "", ZAux[j], j == SIZE - 1 ? "\n" : " ");*/

    /*sequences to run*/
    #ifdef TEST
        #if N == 7
            int test[SIZE - 4] = {7, 5, 3, 8};
        #elif N == 15
            int test[SIZE - 4] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3};
        #endif
        generate_tree(teste);
    #else
        int generated[SIZE - 4];
        #ifndef ITERATIVE
            generate_seq(SIZE - 4, generated);
        #else
            generate_seq_iterative(generated);
        #endif
    #endif

    #ifdef NUMBERS
        printf("Número de sequências: %ld\n", SCount);
        printf("Número de códigos: %lld e %lld \n", CCount, CCount1); /*TODO*/
        #ifdef GRAPH
            printf("Verificação de PRIM: Número de cortes : %d | Número de códigos resultantes : %d\n", CCutsPrim, FinalsPrim);
            #ifdef CONSTANTS
                printf("Verificação das CONSTANTS: Número de cortes : %d | Número de códigos resultantes : %d\n", CCutsConst, FinalsConst);
            #endif
        #endif
    #endif

    #ifdef TOFILE
        fclose(f_list);
        #ifdef GRAPH
            fclose(f_cut);
            fclose(f_pass);
        #endif
    #endif

    return 0;
}

/******************************************************************/
/******************   FUNCTION Definition   ***********************/
/******************************************************************/

void generate_seq(int spaces, int *generated) {
    /*Recursive function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
    /*spaces    - blank spaces in the sequence to fill up*/
    /*generated - sequence generated so far */
    /*NOTE: because the sequences are not saved, it is reused, and so, filled sequencially*/

    int i;

    if (spaces > 1) {
        for (i = 3; i < SIZE + 1; i++) {
            #if N == 15
                if (i == LEVEL + 5) break;
            #endif
            /*if (i == 4) break;*/  /*additional break for testing*/
            generated[SIZE - 4 - spaces] = i; /*generated index starts in 0 because first call is spaces = SIZE - 4*/
            generate_seq(spaces - 1, generated);
            /*break;*/    /*additional break for testing*/
        }
    }
    else {
        /*It is now in the last element of the sequence*/
        for (i = 3; i < SIZE + 1; i++) {
            generated[SIZE - 4 - spaces] = i;

            /*Prints the current sequence values*/
            /*int j; for (j = 0; j < SIZE - 4; j++) printf("%s %d %s", j == 0 ? "\nNew sequence: [" : "", generated[j], j < SIZE - 5 ? ",\0" : "]\n");*/

            /*printf("\n\n\nWill generate a new tree:\n\n\n");*/

            #ifdef NUMBERS
                SCount++;
            #endif

            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);

            /*break;*/      /*additional break for testing*/
            #ifdef TOFILE
                int j; for (j = 0; j < SIZE - 4; j++) fprintf(f_list, "%s %d %s", j == 0 ? "[" : "", generated[j], j < SIZE - 5 ? ",\0" : "]\n");
            #endif
        }
    }
}

void generate_tree(int *seq) {
    /*Function that generates trees from Prüfer sequences with the 1-0-2 super node*/
    /*seq - Prüfer sequence to be decoded*/
    /*NOTE: The decodig algorithm implemented in this functions runs in O(n), as is explained in Wang et al; "An Optimal Algorithm for Prüfer Codes" J. Software Engineering & Applications, 2009, 2 (111-115)*/

    int i, j;
    int degree[SIZE - 2];    /*array with the degree of each node in the tree*/
    int index = 0, x = 0, y; /*auxiliary variable to the linear time decoding of the Prüfer sequence*/

    /*1st step - degree array construction*/
    for (i = 0; i < SIZE - 2; i++) {
        degree[i] = 1;
    }
    for (i = 0; i < SIZE - 4; i++) {
        degree[seq[i] - 3] += 1;
    }

    /*check first node with degree equal to 1 */
    for (i = 3; i < SIZE + 1; i++) {
        if (degree[i - 3] == 1) {
            index = x = i;
            break;
        }
    }

    /*Graph edge definition and degree array destruction*/
    for (i = 0; i < SIZE - 4; i++) {
        y = seq[i];

        list[y][list_index[y]++] = x;
        parent[x] = y;

        degree[y - 3] -= 1;
        if ((y < index) && (degree[y - 3] == 1)) {
            x = y;
        }
        else {
            for (j = index + 1; j < SIZE + 1; j++) {
                if (degree[j - 3] == 1) {
                    index = x = j;
                    break;
                }
            }
        }
    }
    y = SIZE;

    list[y][list_index[y]++] = x;
    parent[x] = y;

    /*Generates trees from combinations of edges to super nodes*/
    #ifndef ITERATIVE
        unfold_SN_list(0);
    #else
        unfold_SN_list_iterative();
    #endif

    /*Clear Adjacency List*/
    clear_list();
}

/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

void unfold_SN_list(int index) {
    /*Recursive function used to combinate connections to the super node*/
    /*index - index of the nodes connected to the super node*/
    /*NOTE: This function procedes as follows: for each node connected to the
    super node (which number is SIZE), associates it with all the individual
    nodes in the super node and then recursively calls the next one*/

    int i;

    if (index <= list_index[SIZE] - 2) { /*if the next one is not the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            parent[list[SIZE][index]] = i;

            unfold_SN_list(index + 1);

            list[i][--list_index[i]] = 0;
        }
    }
    else { /*if the next one is the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            parent[list[SIZE][index]] = i;

            /*printf("newly generated adjacency list : \n");*/
            /*print_list();*/

            /*print the parent array*/
            /*int j; for (j = 0; j < SIZE; j++) printf ("%s %d %s", 
                j == 0 ? "Z = [" : "", parent[j], j < SIZE - 1 ? ",\0" : "]\n");
                */

            #ifdef NUMBERS
                CCount++;
            #endif

            #ifdef GRAPH
                generate_graph();
            #endif

            list[i][--list_index[i]] = 0;
        }
    }
}

void generate_graph() {/*TODO description*/
    /*Function that generates the zero representation for a given tree*/
    /*NOTE: */

    int i, j;
    int next_node[SIZE] = {0};
    int next_index = 1;         /*already has 0 in it*/
    int a, b;

    for (i = 0; i < SIZE; i++) {
        a = next_node[i]; /*a = next node to be evaluated*/
        for (j = 0; j < list_index[a]; j++) {
            b = list[a][j]; /*b = son of a*/
            next_node[next_index++] = b;
            Z[b] = Z[a] ^ ZAux[a ^ b];
        }
    }

    /*print the parent array*/
    /*for (j = 0; j < SIZE; j++) printf ("%s %d %s", j == 0 ? "Z = [" : "", parent[j], j < SIZE - 1 ? ",\0" : "]\n\n\n");*/
    /*print the representation Z*/
    /*for (j = 0; j < SIZE; j++) printf("%s %d %s", j == 0 ? "\nZ = [" : "", Z[j], j < SIZE - 1 ? ",\0" : "]\n\n");*/

    #if defined(PRIM) || defined(CONSTANTS)
        verify_representation();
    #endif

    #ifdef TOFILE
        for (j = 0; j < SIZE; j++)  fprintf(f_list, "%s %d %s", j == 0 ? "Z = [" : "", Z[j], j < SIZE - 1 ? ",\0" : "]\n");
    #endif

    /*clear Z*/
    for (i = 3; i < SIZE; i++) {
        Z[i] = 0;
    }
}

void verify_representation() {
    /*Function that applies cuts to a given representation according to user defines*/

    int i, j;
    int next_node[SIZE] = {0};
    int index = 1;              /*index of next_node array*/
    int mask[SIZE] = {1};
    #ifdef TOFILE
        int l;
    #endif

    #ifdef PRIM
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (!mask[j] && __builtin_popcount(Z[next_node[i]] ^ Z[j]) == 1) {
                    next_node[index++] = j;
                    mask[j] = 1;

                    if (parent[j] != next_node[i]) {

                        #ifdef TOFILE
                            for (l = 0; l < SIZE; l++) fprintf(f_cut, "%s %d %s", l == 0 ? "Z = [" : "", Z[l], l < SIZE - 1 ? ",\0" : "]\n");
                        #endif

                        #ifdef NUMBERS
                            CCutsPrim++;
                        #endif

                        return;
                    }
                }
            }
        }

        #if defined(TOFILE) && !defined(CONSTANTS)
            for (l = 0; l < SIZE; l++) fprintf(f_pass, "%s %d %s", l == 0 ? "Z = [" : "", Z[l], l < SIZE - 1 ? ",\0" : "]\n");
        #endif

        #ifdef NUMBERS
            FinalsPrim++;
        #endif
    #endif

    #ifdef CONSTANTS
        int temp;
        for (i = 1 ; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                temp = Z[i] ^ Z[i ^ j];
                if (temp < Z[j]) {
                    #ifdef NUMBERS
                        CCutsConst++;
                    #endif
                    return;
                }
                else if (temp > Z[j]) {
                    break;
                }
            }
        }

        #ifdef TOFILE
            for (l = 0; l < SIZE; l++) fprintf(f_pass, "%s %d %s", l == 0 ? "Z = [" : "", Z[l], l < SIZE - 1 ? ",\0" : "]\n");
        #endif

        #ifdef NUMBERS
            FinalsConst++;
        #endif
    #endif

}

void print_list() {
    /*Function that prints the adjacency list*/
    /*NOTE: */

    int i, j;

    printf("Adjacency list: \n");
    for (i = 0; i < SIZE + 1; i++) {
        printf("%d :  ", i);
        for (j = 0; j < list_index[i]; j++) {
            printf("%d, ", list[i][j]);
        }
        printf("\b\b  \n");
    }
    printf("\n");
}

void clear_list() {
    /*Function that clears the adjacency for the next Prüfer sequence*/
    /*NOTE: Clears list by reseting the list_indexes, and maintaining the information about the (0-1) and the (0-2) edges*/

    int i;

    /*Reset the information of the list indexes*/
    list_index[0] = 2;
    for (i = 1; i < SIZE + 1; i++) {
        list_index[i] = 0;
    }
}
