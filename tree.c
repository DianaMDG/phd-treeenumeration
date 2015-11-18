#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>

#include"tree.h"
#include"neutral_rep.h"

/*#define matriz      0*/

/*#define NUMBERS     0*/
/*#define tofile      0*/
/*#define graph       1*/
/*#define prim        0*/
/*#define CONSTANTS   0*/

/*#ifdef matriz
    #pragma message "USING MATRIX"
#else
    #pragma message "USING LIST"
#endif*/

#ifdef NUMBERS
    #pragma message "counting"
#endif

#ifdef tofile
    #pragma message "printing to file"
#endif

#ifdef graph
    #pragma message "generating representations"
#endif

#ifdef prim
    #pragma message "calculating minimum spanning tree using Prim algorithm"
#endif

#ifdef CONSTANTS
    #pragma message "Verifying if is minimal regarding constants"
#endif

/*NOTE*/
/* N    : Number of bis used by the representation, N used in the neutral network NN(n,k)*/
/* SIZE : Number of syndromes, number of nodes in the tree*/
/* K    : Number of information bits*/
/* G    : Generator polynomial for the codes */

/*Define the value of N. IF N == 15, DEFINE THE CUTTING LEVEL!!!!!*/
#define N 7
#define LEVEL 2

#if N == 7
    /* Data for the NN(7,4) codes*/
    #define SIZE    (N+1)
    #define K       (N-3)
    #define G       ((uint16_t ) 0xb )
#elif N == 15
    /* Data for the NN(15,11) codes*/
    #define SIZE    (N+1)
    #define K       (N-4)
    #define G       ((uint16_t ) 0x13)
    #define CORTE   corte[LEVEL]
    int corte[4] = {28672, 2480058, 58720256, 683593750};
#else
    #pragma message "PLEASE CHOOSE VALUE OF N FROM {7, 15}"
#endif

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))


int list[SIZE+1][SIZE-1] = {{1,2}};        /*Adjacency list*/
int list_index [SIZE+1]  = {2};            /*Indexes to access the next list index of a given node to be added*/
int parent[SIZE]         = {-1,0,0};       /*Parent array*/

uint16_t ZAux[SIZE] = {0};              /*Auxiliar Zeros with each syndrome with Hamming distance 1 from 0 for computing representation zeros*/
uint16_t Z[SIZE] = {0,1,2};             /*Zeros of the representation*/

#ifdef NUMBERS
    unsigned long long int CCount = 0;    /*Number of generated codes*/
    unsigned long long int CCount1 = 0;   /*Number of something*/
    unsigned long      int SCount = 0;    /*Number of generated Sequences*/
    int CCuts = 0;
    int Finals = 0;
    int CCutsConst = 0;
    int FinalsConst = 0;
#endif

#ifdef tofile
    FILE *f_list;
    #ifdef graph
        FILE *f_cut, *f_pass;
    #endif
#endif

int powb3[13];            /*Array used for the iterative case*/
/******************************************************************/
/************************   MAIN   ********************************/
/******************************************************************/
int main() {

    int generated[SIZE-4];
    int i;

    for (i = 0; i < 13; i++) {
        powb3[i] = pow(3, i);
    }

    int j;
    uint16_t c = 0, b, t;

    #if N == 7
        int teste[SIZE-4] = {7,5,3,8};
    #elif N == 15
        int teste[SIZE-4] = {4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,3 ,3 ,3};
    #endif

    #ifdef tofile
        f_list = fopen("list_representations.txt", "w");
        #ifdef graph
            f_cut = fopen("cut.txt", "w"); f_pass = fopen("pass.txt", "w");
        #endif
    #endif

    /*Generates array of words with each syndrome which have */
    for (j = 0; j < N; j++) {
        /*creates the word and checks syndrome*/
        b = c ^ (uint16_t)(1<<j);
        t = syndrome(N, K, G, b);
        /*printf("j : %d\t word : %d\t syndrome : %d \n", j, b, t);*/
        /*saves zero, records next_z*/
        /*printf("palavra: %d, sindrome: %d\n", b, t);*/
        ZAux[t] = b;
    }

    /*print the complementary array */
    /*for(j = 1; j < SIZE; j++)  printf("%s %d %s", j==1 ? "ZAux : ":"", ZAux[j], j ==SIZE -1?"\n":" ");*/

    /*Generate Prüfer sequences*/
    generate_seq(SIZE-4, generated);
    /*generate_seq_iteratively(generated);*/
    /*generate_tree(teste);*/

    #ifdef NUMBERS
        printf("Número de códigos: %lld e %lld \n", CCount, CCount1);
        printf("numero de sequências: %ld\n", SCount);

        #ifdef graph
            printf("Verificação de PRIM: Número de cortes : %d | Número de códigos resultantes : %d\n", CCuts, Finals);
            #ifdef CONSTANTS
                printf("Verificação das CONSTANTS: Número de cortes : %d | Número de códigos resultantes : %d\n", CCutsConst, FinalsConst);
            #endif
        #endif
    #endif

    #ifdef tofile
        fclose(f_list);
        #ifdef graph
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
    /* Recursive function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
    /* spaces    - blank spaces in the sequence to fill up*/
    /* generated - sequence generated so far */
    /* NOTE: because the sequences are not saved, it is reused, and so, filled sequencially*/

    int i;

    if (spaces > 1) {
        for( i = 3; i < (SIZE)+1; i++) {
            #if N == 15
                if (i == LEVEL + 5) break;
            #endif
            /*if (i == 4) break;*/  /*additional break for testing*/
            generated[SIZE-4-spaces] = i; /*i because values go from 0 to n and i goes from 0 to n */
            generate_seq (spaces-1, generated);
            /*break;*/
        }
    }
    else {
        /*It is now in the last element of the sequence*/
        for (i = 3; i< (SIZE) + 1; i++) {
            generated[SIZE-4-spaces] = i;

            /*Prints the current sequence values*/
            /*int j; for ( j = 0; j < SIZE-4; j++) printf("%s%d %s",  j==0?"\nNew sequence: [":"", generated[j], j< SIZE-5 ? ",\0":"]\n");*/

            /*printf("\n\n\nWill generate a new tree:\n\n\n");*/

            #ifdef NUMBERS
                SCount++;
            #endif

            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);

            /*break;*/      /*additional break for testing*/
            #ifdef tofile
                int j; for ( j = 0; j < SIZE-4; j++) fprintf(f_list, "%s%d %s",  j==0?"[":"", generated[j], j< SIZE-5 ? ",\0":"]\n");
            #endif
        }
    }
}

void generate_seq_iteratively(int *generated) {
    /*Iterative function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
    /* NOTE: may be outdated */

    int i, j;
    int div, res;
    int base = SIZE - 2;
    /*int index;*/

    #if N == 15
        for (i = 0; i < CORTE; i++) {
    #else
        for (i = 0; i < pow((SIZE-2), (SIZE-4)); i++) {
    #endif
        /*index = 0;*/
        div = i / base;
        res = i % base;
        generated [SIZE-5] = res + 3;   /*3 comes from shifting the nodes not in the Super Node*/

        for (j = SIZE-6 ; j > 1; j--) {
        /*while (div != 0 || res != 0) {*/
            res = div % base;
            div = div / base;
            generated [j] = res + 3; /*again, 3 ... */
        }

        /*just in case some missed*/
        /*for (j = index; j < (SIZE - 4); j++) {
            generated [j] = 3;*/ /*again, 3 ... */
        /*}*/

        #ifdef NUMBERS
            SCount++;
        #endif

        /*int j; for ( j = 0; j < SIZE-4; j++) printf("%s%d %s",  j==0?"\nNew sequence: [":"", generated[j], j< SIZE-5 ? ",\0":"]\n");*/
        #ifdef tofile
            int j; for ( j = 0; j < SIZE-4; j++) fprintf(f_list, "%s%d %s",  j==0?"[":"", generated[j], j< SIZE-5 ? ",\0":"]\n");
        #endif
        /*Generate tree associated to the sequence generated*/
        generate_tree(generated);
    }
}

void generate_tree(int *seq){
    /*Function that generates trees from Prüfer sequences with the 1-0-2 super node*/
    /*NOTE: The decodig algorithm implemented in this functions runs in O(n), as is explained in Wang et al; "An Optimal Algorithm for Prüfer Codes" J. Software Engineering & Applications, 2009, 2 (111-115)*/

    int i, j;
    int degree[SIZE-2];
    int index = 0, x = 0, y; /*auxiliary variable to the linear time decoding of the Prüfer sequence */

    /*1st step - degree array construction*/
    for (i = 0; i < (SIZE)-2; i++){
        degree[i] = 1;
    }
    for (i = 0; i < SIZE-4; i++){
        degree[seq[i]-3] += 1;
    }

    /*check first node with degree equal to 1 */
    for (i = 3; i < SIZE+1; i++){
        if (degree[i-3]==1) {
            index = x = i;
            break;
        }
    }

    /*Graph edge definition and degree array destruction*/
    for (i = 0; i < (SIZE)-4 ; i++) { /* seq elements: 0 -> SIZE-2 */
        y = seq[i];

        list[y][list_index[y]++] = x;
        parent[x] = y;

        degree[y-3] -= 1;
        if ((y < index) && (degree[y-3] == 1)) {
            x = y;
        }
        else {
            for (j = index + 1; j < SIZE+1; j++){
                if (degree[j-3]==1) {
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
    unfold_SN_list_recursive(0);

    /*Clear Adjacency List*/
    clear_list();
}

/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

void unfold_SN_list_recursive(int index) {
    /*Function used to generate combinations of connections to super node, recursively*/

    int i, j;
    if (index <= list_index[SIZE]-2) { /*if the next one is not the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            parent[list[SIZE][index]] = i;
            unfold_SN_list_recursive(index + 1);
            list[i][--list_index[i]] = 0;
        }
    }
    else{ /*if the next one is the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            parent[list[SIZE][index]] = i;
            /*printf("newly generated adjacency list : \n");*/
            /*print_list();*/
        /*for (j = 0; j < SIZE; j++) printf ("%s %d %s",  j==0?"Z = [":"", parent[j], j< SIZE-1 ? ",\0":"]\n");*/
            #ifdef NUMBERS
                CCount++;
            #endif
            #ifdef graph
                generate_graph_list();
            #endif
            list[i][--list_index[i]] = 0;
        }
    }
}

static void unfold_SN_list(void) {
    /*Function used to generate combinations of connections to super node*/

    int i = powb3[list_index[SIZE]], j;
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
        #ifdef graph
            generate_graph_list();
        #endif

        /*reseting the list indexes TODO might be unecessary*/
        list_index[0] = 2;
        list_index[1] = 0;
        list_index[2] = 0;
    }
}

void generate_graph_list(void) {
    /*Function that generates the representation for a given tree*/
    /*NOTE: The first 3 elements of the mask array are never read, because the unfolding of the super node does not add 2 edges. Only from {0,1,2} to the rest*/

    int i, j;
    int next_node[SIZE] = {0};
    int next_index = 1;         /*already has 0 in it*/
    int a, b;

    for (i = 0; i < SIZE; i++) {
        a = next_node[i]; /*next node to be evaluated */
        for (j = 0; j < list_index[a]; j++) {
            b = list[a][j]; /*son of a*/
            next_node[next_index++] = b;
            Z[b] = Z[a] ^ ZAux[a ^ b];
        }
    }

    /*print the parent array*/
    /*for (j = 0; j < SIZE; j++) printf ("%s %d %s",  j==0?"Z = [":"", parent[j], j< SIZE-1 ? ",\0":"]\n\n\n");*/
    /*print the representation Z*/
    /*for ( j = 0; j < SIZE; j++) printf("%s %d %s",  j==0?"\nZ = [":"", Z[j], j< SIZE-1 ? ",\0":"]\n\n");*/

    #ifdef prim
        apply_prim_list();
    #endif

    #ifdef tofile
        for ( j = 0; j < SIZE; j++) { fprintf(f_list,"%s %d %s",  j==0?"Z = [":"", Z[j], j< SIZE-1 ? ",\0":"]\n"); }
    #endif

    /*clear Z*/
    for (i = 3; i < SIZE; i++) {
        Z[i] = 0;
    }
}

/*Function that checks if the tree of a given representation is its minnimum spanning tree*/
void apply_prim_list(void){
    /*TODO*/
    int i, j, k;
    int next_node[SIZE] = {0}, index = 1;
    int mask[SIZE] = {1};
    int l;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (!mask[j] && __builtin_popcount(Z[next_node[i]]^Z[j]) == 1) {
                next_node[index++] = j;
                mask[j] = 1;

                if (parent[j] != next_node[i]) {
                    #ifdef tofile
                        for (l = 0; l < SIZE; l ++) fprintf(f_cut, "%s %d %s",  l==0?"Z = [":"", Z[l], l< SIZE-1 ? ",\0":"]\n");
                    #endif
                    #ifdef NUMBERS
                        CCuts++;
                    #endif
                    return;
                }
            }
        }
    }

    #ifdef tofile
        for (l = 0; l < SIZE; l ++) fprintf(f_pass, "%s %d %s",  l==0?"Z = [":"", Z[l], l< SIZE-1 ? ",\0":"]\n");
    #endif

    #ifdef NUMBERS
        Finals++;
    #endif
    #ifdef CONSTANTS
        int temp;
        for (i = 1 ; i < SIZE; i++) {
            for (j = 0; j < SIZE ; j++) {
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
    #endif
    #ifdef NUMBERS
        FinalsConst++;
    #endif

}

void print_list(void) {
    /*Function that prints the adjacency list*/
    /*NOTE: */
    int i, j;

    printf("Adjacency list: \n");
    for (i = 0; i < SIZE+1; i++) {
        printf("%d :  ", i);
        for (j = 0; j < list_index[i] ; j++) {
            printf("%d, ", list[i][j] );
        }
        printf("\b\b  \n");
    }
    printf("\n");
}

void clear_list(void) {
    /*Function that clears all edges in an adjacency list but the (0,1) and (0,2) edges*/
    /*NOTE: Clears list by reseting the list_indexes, and maintaining the information about the 2 referred edges*/
    int i;

    /*Reset the information of the list indexes*/
    list_index[0] = 2;
    for (i = 1; i < SIZE + 1;i++) {
        list_index[i] = 0;
    }
}
