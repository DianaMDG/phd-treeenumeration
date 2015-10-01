#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include"tree.h"
#include"neutral_rep.h"

#define N       7         /*Number of bis used by the representation, N used in the neutral network NN(n,k)*/
#define SIZE    (N+1)     /*Number of syndromes, number of nodes in the tree*/
#define K       (N-3)     /*Number of information bits*/

#define TREE    1
#define GRAPH   (1<<1)
#define MINTREE (1<<2)


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

uint16_t g_3 = 0xb;

/*Triangular Adjacency matrix*/
ele_t adjacency[(SIZE-1)*(SIZE)/2];
ele_t *lines[SIZE-1];

/*ele_t adjacency[SIZE][SIZE];*/

uint16_t Z[SIZE];

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
    /*------------------TEST: Set matrix values*/
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
    /*print_adj(lines);*/
    /*-------------------END OF TEST*/

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
            break;
        }
    }
    else {
        /*It is now in the last sequence space*/
        for (i = 0; i< (SIZE); i++) {
            generated[SIZE-2-spaces] = i;

            /*Prints the current sequence values*/
            /*printf("New sequence: [");
            for ( j = 0; j < SIZE-2; j++) {
                printf("%d %s", generated[j], j< SIZE-3 ? ",\0":"]\n");
            }*/
            /*printf("Will generate a new tree:\n");*/
            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);
            /*break;*/
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
        degree[seq[i]] += 1;
    }

    /*Graph edge definition and degree array destruction*/
    for (i = 0; i < (SIZE)-2 ; i++) { /* seq elements: 0 -> SIZE-2 */
        for (j = 0; j < (SIZE); j++) { /* degree elements*/
            if (degree[j] == 1) {
                /*insert edge (seq[i], j)*/
                lines[min(seq[i], j)][max(seq[i],j)] = TREE; /*Adjacency Case*/
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
        }
        else if (degree[j] == 1 && i >= 0) {
            lines[i][j] = TREE;
        }
    }
    /*print adjacency matrix*/
    print_adj();

    /*Calculate the graph associated with it*/
    generate_graph();
    /*Clear Adj matrix*/
    clear_adj();
}

/*Function that generates the neutral network graph from the tree*/
void generate_graph() {
    /*NOTE: This function populates the Zeros array of the neutral neutwork and calculates the graph's edges*/
    /*It operates in two steps: FIRST, from the tree root(0), checks all nodes directly connected. For each, searches the corresponding word, with Hamming distance 1 and with the corresponding Syndrome. Then, recursively does it for the other nodes. SECONDLY, Knowing that the graph has at least the tree edges, the edges with 0(zero) in the tree adjacency matrix are tested for their Hamming distance, being set to one in the graph adjacency matrix if distance is 1.*/
    /*WARNING: The graph's edges info is in the form of the second bit of the adjacency matrix. Changing this way of doing implies refactoring this function*/

    int i, j, k;
    int a, next = 1, count = 1;
    uint16_t t_s, t_w; /*temporary syndrome and word*/
    uint16_t next_z[SIZE];

    /*First step*/
    for (i = 0; i < SIZE; i++) {
        next_z[i]=0;
    }
                            
                            printf("next_z: \t");
                            for (a = 0; a < SIZE; a++) {
                                printf(" %d%c", next_z[a], a < N ? ',' : '\n');
                            }
    /*for each element of next_z*/
    for (k = 0; k < SIZE; k++) {
    printf("elemtno de next_z: %d (%d)\n", k, next_z[k]);
        if (count == 8) {
            printf("BREAK\n");
            break;
        }
    /*printf("check k: %d\n", k);*/
        /*Checks edges in adjacency lines*/
        for (i = next_z[k] + 1; i < SIZE; i++) {
            if (count == 8) {
            printf("BREAK\n");
                break;
            }
                                            printf("check i: %d\n", i);
                                            printf("next_z: \t");
                                                for (a = 0; a < SIZE; a++) {
                                                    printf(" %d%c", next_z[a], a < N ? ',' : '\n');
                                                }
                           printf("lines[%d][%d]\n", next_z[k], i);
            if (lines[next_z[k]][i] == 1) {
                count +=1;
                           printf("lines[%d][%d] = 1\n", next_z[k], i);
                for (j = 0; j < N; j++) { /* for each d = 1*/
                    /*creates the word and checks syndrome*/
                    t_w = Z[next_z[k]] + (uint16_t)(1<<j);
                    t_s = syndrome(N, K, g_3, t_w);
                    if (t_s == i) {
                        /*saves zero, records next_z, breaks inner cycle*/
                        Z[i] = t_w;
                        next_z[next] = i;
                        next +=1;
                        printf("break! next = %d", next-1);
                        printf("check\n");
                            printf("Z\t: \t");
                            for (a = 0; a < SIZE; a++) {
                                printf(" %d%c", Z[a], a < N ? ',' : '\n');
                            }
                        break;
                    }
                }
            }
        }
        /*and columns*/
        for (i = 1; i < next_z[k] ; i++){ /*start in 1 because 1st iteration is with z = 0, and there is no need to repeat*/
            printf("Buuuuuuu!!!!!\n");
            if (count == 8) {
                printf("BREAK\n");
                break;
            }
                                printf("lines[%d][%d]\n", i, next_z[k]);
            if (lines[i][next_z[k]] == 1) {
                              printf("lines[%d][%d] = 1\n", i, next_z[k]);
                for (j = 0; j < N; j++) { /* for each d = 1*/
                    /*creates the word and checks syndrome*/
                    t_w = Z[next_z[k]] + (uint16_t)(1<<j);
                    t_s = syndrome(N, K, g_3, t_w);
                    if (t_s == i) {
                        /*saves zero, records next_z, breaks inner cycle*/
                        Z[i] = t_w;
                        next_z[next] = i;
                        next +=1;
                        break;
                    }
                }
            }
        }
    }
                                /*print Z*/
                                printf("Zero vector: \n");
                                for (a = 0; a < SIZE; a++) {
                                    printf("%d%c ", Z[a], a < N ? ',' : ' ');
                                    Z[a] = 0;
                                }
                                printf("\n");

}

/*Function that prints the adjacency matrix of a given tree*/
void print_adj() {
    /*NOTE: Print adjacency matrix using array of pointers lines, and alligned to right.*/ 
    /*WARNING: Size of strings is hard coded.*/

    int i, j;

    /*Print using adacency matrix by lines*/
    printf("Adjacency matrix:\n");
    char temp[128];
    for (i=0; i < (SIZE-1); i++) {
        temp[0] = '\0';
        for (j = i+1; j < (SIZE); j++) {
            sprintf(temp, "%s%2d%c ", temp, lines[i][j], j<(SIZE-1) ? ',' : ' ');
        }
        printf("%30s\n", temp);
    }
}

/*Function that clears an adjacency matrix -> sets to zero */
void clear_adj() {
    /*NOTE: */
    int i;
    for (i=0; i < (SIZE)*(SIZE-1)/2; i++) {
        adjacency[i] = 0;
    }
}
