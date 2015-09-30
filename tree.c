#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"tree.h"

#define N       11       /*N used in the neutral network NN(n,l) */
#define SIZE    N+1     /*Number of bis used by the representation */

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/*Triangular Adjacency matrix*/
ele_t adjacency[(SIZE-1)*(SIZE)/2];
ele_t *lines[SIZE-1];

/*ele_t adjacency[SIZE][SIZE];*/


/******************************************************************/
/************************   MAIN   ********************************/
/******************************************************************/
int main() {

    int generated[SIZE-2];
    int i;
    int a = -1;

    /*Generate adjacency matrix and respective lines pointer array*/
    for (i=0; i < SIZE-1; i++) {
        lines[i] = adjacency + a;
        a += N - i - 1; /*a += SIZE - 2 - i;*/
    }
    /*TEST: Set matrix values*/
    /*for (i=0; i < (SIZE-1)*(SIZE)/2; i++) {
        adjacency[i] = i+1;
    }*/

    /*Print adjacency matrix by lines*/
    /*print_adj(lines);*/

    /*TEST: Change 2 lines of the matrix*/
    /*j=2;
    for (i=j+1; i < SIZE; i++) {
        lines[j][i] = 0;
    }*/
    /*Print adjacency matrix by lines*/
    /*printf("Print adjacency matrix by lines:\n");
    for (i=0; i < SIZE-1; i++) {
        for (j = i+1; j < SIZE; j++) {
            printf("%d, ",lines[i][j]);
        }
        printf("\n");
    }*/

    /*Generate Prüfer sequences*/
    generate_seq(SIZE-2, generated);

    return 0;
}


/******************************************************************/
/******************   FUNCTION Definition   ***********************/
/******************************************************************/

/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated) {
    /* spaces    - blank spaces in the sequence to fill up*/
    /* generated - sequence generated so far */
    /* NOTE: because the sequences are not saved, it is reused, and so, filled sequencially*/
    
    int i;
    /*int j;*/ /*Separated because j is unused when not printing*/
    if (spaces > 1) {
        for( i = 0; i < SIZE; i++) {
            generated[SIZE-2-spaces] = i; /*i because values go from 0 to n and i goes from 0 to n */
            generate_seq (spaces-1, generated);
        }
    }
    else {
        /*It is now in the last sequence space*/
        for (i = 0; i< (SIZE); i++) {
            generated[SIZE-2-spaces] = i;

            /*Prints the present sequence values*/
            /*printf("New sequence: [");
            for ( j = 0; j < SIZE-2; j++) {
                printf("%d %s", generated[j], j< SIZE-3 ? ",\0":"]\n");
            }*/
            /*printf("Will generate a new tree:\n");*/
            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);
         }
     }
}

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq){
    int i, j;
    int degree[SIZE];

    /*1st step - degree array construction*/
    for (i = 0; i < SIZE; i++){
        degree[i] = 1;
    }
    for (i = 0; i < SIZE-2; i++){
        /*printf("check2\n");*/
        degree[seq[i]] += 1;
    }
    /*printf("%d\n", degree[SIZE-1]);*/

    /*Graph edge definition and degree array destruction*/
    for (i = 0; i < (SIZE)-2 ; i++) { /* seq elements: 0 -> SIZE-2 */
        for (j = 0; j < (SIZE); j++) { /* degree elements*/
            if (degree[j] == 1) {
                /*insert edge (seq[i], j)*/
                /*Adjacency Case*/
                /*printf("seq[i], %d\t", seq[i]);*/
                /*printf("j, %d\n", j);*/
                lines[min(seq[i], j)][max(seq[i],j)] = 1;
                /*Array Case TODO*/
                degree[j] -= 1;
                degree[seq[i]] -= 1;
                break;
            }
        }
    }
    /*Finds the last two indexes with value 1*/
    i = -1;
    for (j = 0; j < (SIZE); j++) {
        if (degree[j] == 1 && i < 0) {
            i = j;
            /*printf("i: %d\n", i);*/
        }
        else if (degree[j] == 1 && i >= 0) {
            lines[i][j] = 1;
            /*printf("j: %d\n", j);*/
        }
    }
    /*print adjacency matrix*/
    /*print_adj(lines);*/
    /*Clear Adj matrix*/
    clear_adj(adjacency);
}

/*Function that prints the adjacency matrix of a given tree*/
inline void print_adj(ele_t **lines) {
    /*NOTE: Print adjacency matrix using array of pointers lines, and alligned to right.*/ 
    /*WARNING: Size of stings is hard coded.*/

    int i, j;
    /*Print using adjacency array*/
    /*printf("Print Adj matrix: %d\n",  (SIZE-1)*(SIZE)/2);
    for (i=0; i < (SIZE-1)*(SIZE)/2; i++) {
        printf("%d, ",adjacency[i]);
    }
    printf("\n");*/
    
    printf("Print adjacency matrix by lines:\n");
    char temp[128];
    for (i=0; i < (SIZE-1); i++) {
        temp[0] = '\0';
        for (j = i+1; j < (SIZE); j++) {
            sprintf(temp, "%s%2d%c ", temp, lines[i][j], j<(SIZE-1) ? ',' : ' ');
        }
        printf("%16s\n", temp);
    }
}

/*Function that clears an adjacency matrix -> sets to zeros */
inline void clear_adj(ele_t adjacency[]) {
    /*NOTE: */
    int i;
    for (i=0; i < (SIZE)*(SIZE-1)/2; i++) {
        adjacency[i] = 0;
    }
}
