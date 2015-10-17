#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include"tree.h"
#include"neutral_rep.h"

#define N      7         /*Number of bis used by the representation, N used in the neutral network NN(n,k)*/
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
            /*break;*/
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
            printf("\n\nWill generate a new tree:\n\n\n");

            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);
            /*break;*/
         }
     }
}

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq){
    int i;
    int degree[SIZE];
    int index = 0, x = 0, y; /*auxiliary variable to the linear time decoding of the Prüfer sequence */
    int heap[SIZE-2] = {0}; /*each tree has at least 2 leaves*/
    int ind = 0;        /*auxiliary variable to know in which position of heap we are*/

    /*1st step - degree array construction*/
    for (i = 0; i < SIZE; i++){
        degree[i] = 1;
    }
    for (i = 0; i < SIZE-2; i++){
        degree[seq[i]]++;
    }
    /*fill heap*/
    for (i = 0; i < SIZE; i++) {
        if (degree[i] == 1) {
            /*printf("Antes: i = %d, degree[i] = %d, ind = %d, heap[ind] = %d.\n", i, degree[i], ind, heap[ind]);*/
            heap[ind] = i;
            printf("Antes: i = %d, degree[i] = %d, ind = %d, heap[ind] = %d.\n", i, degree[i], ind, heap[ind]);
            ind++;
        }
    }
    /*check first node with degree equal to 1 --> Replaced by HEAP*/
    /*for (i = 0; i < SIZE; i++){
        if (degree[i]==1) {
            x = i;
            index = x;
            break;
        }
    }*/

    ind = 0;
    index = x = heap[ind];
    ind++;
    /*Graph edge definition and degree array destruction*/
    for (i = 0; i < (SIZE)-2 ; i++) { /* seq elements: 0 -> SIZE-2 */
        y = seq[i];
        lines[min(x, y)][max(x, y)] = TREE; /*Adjacency Case*/
        degree[y] -= 1;
        if ((y < index) && (degree[y] == 1)) {
            x = y;
            if (i == (SIZE)-3) { printf("CENAS A: x = %d\n", x); }
        }
        else {
        
            if (i == (SIZE)-3) { printf("CENAS B: ind = %d\n", ind); }
            index = x = heap[ind];
            ind++;
            
            
            if (i == (SIZE)-3) { printf("CENAS B: x = %d\n", x); }
            /*for (j = index + 1; j < SIZE; j++){
                if (degree[j]==1) {
                    index = x = j;
                    break;
                }
            }*/
        }
    }
    y = SIZE-1;
    printf("x = %d, y = %d, ind = %d.\n", x, y, ind);
    lines[min(x, y)][max(x, y)] = TREE; /*Adjacency Case*/

    /*print adjacency matrix*/
    /*print_adj();*/

    /*Calculate the graph associated with it*/
    /*generate_graph();*/

    /*Clear Adj matrix*/
    clear_adj();
}

/*Function that generates the neutral network graph from the tree*/
void generate_graph() {
    /*NOTE: This function populates the Zeros array of the neutral neutwork and calculates the graph's edges*/
    /*It operates in two steps: FIRST, from the tree root(0), checks all nodes directly connected. For each, searches the corresponding word, with Hamming distance 1 and with the corresponding Syndrome. Then, recursively does it for the other nodes. SECONDLY, Knowing that the graph has at least the tree edges, the edges with 0(zero) in the tree adjacency matrix are tested for their Hamming distance, being set to one in the graph adjacency matrix if distance is 1.*/
    /*WARNING: The graph's edges info is in the form of the second bit of the adjacency matrix. Changing this way of doing implies refactoring this function*/

    int i, j, k;
    /*int a;*/
    int next = 1, count = 1;
    uint16_t t_s, t_w; /*temporary syndrome and word*/
    uint16_t next_z[SIZE];

    /*First step*/
    for (i = 0; i < SIZE; i++) {
        next_z[i]=0;
    }
    /*for each element of next_z*/
    for (k = 0; k < SIZE; k++) {
        if (count == 8) {
            break;
        }
        /*Checks edges in adjacency lines*/
        for (i = next_z[k] + 1; i < SIZE; i++) {
            if (count == 8) {
                break;
            }
            if (lines[next_z[k]][i] == 1 && Z[i] == 0) {
                count +=1;
                for (j = 0; j < N; j++) { /* for each d = 1*/
                    /*creates the word and checks syndrome*/
                    t_w = Z[next_z[k]] ^ (uint16_t)(1<<j);
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
        /*and columns*/
        for (i = 1; i < next_z[k]; i++){ /*start in 1 because 1st iteration is with z = 0, and there is no need to repeat*/
            if (count == 8) {
                break;
            }
            if (lines[i][next_z[k]] == 1 && Z[i] == 0) {
                count += 1;
                for (j = 0; j < N; j++) { /* for each d = 1*/
                    /*creates the word and checks syndrome*/
                    t_w = Z[next_z[k]] ^ (uint16_t)(1<<j);
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
    /*printf("Zero vector: \n");
    for (a = 0; a < SIZE; a++) {
        printf("%d%c ", Z[a], a < N ? ',' : ' ');
        Z[a] = 0;
    }
    printf("\n");*/

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
