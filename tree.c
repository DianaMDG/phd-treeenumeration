#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include"tree.h"
#include"neutral_rep.h"

/*NOTE*/
/* N    : Number of bis used by the representation, N used in the neutral network NN(n,k)*/
/* SIZE : Number of syndromes, number of nodes in the tree*/
/* K    : Number of information bits*/
/* G    : Generator polynomial for the codes */


/* Data for the NN(7,4) codes*/
/*#define N       7
#define SIZE    (N+1)
#define K       (N-3)
#define G      ((uint16_t ) 0xb )
*/

/* Data for the NN(15,11) codes*/
#define N       15
#define SIZE    (N+1)
#define K       (N-4)
#define G      ((uint16_t ) 0x13)


#define TREE    1
#define GRAPH   (1<<1)
#define MINTREE (1<<2)


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

typedef char bool;
#define true 1
#define false 0

/*ele_t adjacency[(SIZE+1)*(SIZE)/2];*/     /*Triangular Adjacency matrix*/
/*ele_t *lines[SIZE];                */     /*Array of indexes to access the adjacency matrix*/

int list[SIZE+1][SIZE-1] = {{1,2}};         /*Adjacency list*/
int list_index [SIZE+1] = {2};            /*indexes to access the current list index of a given node*/

uint16_t ZAux[SIZE] = {0};              /*Auxiliar Array of Zeros with each syndrome with Hamming distance 1 from 0 for computing representation zeros*/
uint16_t Z[SIZE] = {0,1,2};             /*Zeros of the representation*/

unsigned long long int NCodigos = 0;
/*int                    SCount = 0;*/

/*FILE *f_seq , *f_cod;*/


/******************************************************************/
/************************   MAIN   ********************************/
/******************************************************************/
int main() {

    int generated[SIZE-4];
    int i;
    int a = -1;
    int j;
    uint16_t c = 0, b, t;
    /*int teste[SIZE-4] = {4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,3 ,3 ,3};*/
    /*int teste[SIZE-4] = {6,7,8,8};*/
    
    /*f_seq = fopen("seq_edges.txt", "w");
    f_cod = fopen("cod_cortes.txt", "w");*/
    
    /*set adjacency list to -1*/
    for (i = 2; i < (SIZE+1)*(SIZE-1); i++) {
        list[0][i] = -1;
    }
    /*print_list();*/
    /*Generate adjacency matrix and respective lines pointer array*/
    /*for (i=0; i < SIZE; i++) {
        lines[i] = adjacency + a;
        a += SIZE - i - 1; *//*a += SIZE - 2 - i;*/
    /*}
    lines[0][1] = lines[0][2] = 1;*/
    
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
    
    /*printf("ZAux: ");*/
    /*for(j = 1; j < SIZE; j++)  printf("%s %d %s", j==1 ? "ZAux : ":"", ZAux[j], j ==SIZE -1?"\n":" ");*/
    
    /*Generate Prüfer sequences*/
    generate_seq(SIZE-4, generated);
    /*generate_tree(teste);*/
    
    printf("Número de códigos: %lld\n", NCodigos);
    /*printf("numero de sequências: %d\n", SCount);*/

    /*fclose(f_seq);
    fclose(f_cod);*/

    return 0;
}


/******************************************************************/
/******************   FUNCTION Definition   ***********************/
/******************************************************************/

/*Recursive function that generates the Prüfer sequences that will generate the trees WITH THE SUPER NODE*/
void generate_seq(int spaces, int *generated) {
    /* spaces    - blank spaces in the sequence to fill up*/
    /* generated - sequence generated so far */
    /* NOTE: because the sequences are not saved, it is reused, and so, filled sequencially*/

    int i;
    int j; /*used for printing*/

    if (spaces > 1) {
        for( i = 3; i < (SIZE)+1; i++) {
            if (i == 5) break;
            generated[SIZE-4-spaces] = i; /*i because values go from 0 to n and i goes from 0 to n */
            generate_seq (spaces-1, generated);
            /*break;*/
        }
    }
    else {
        /*It is now in the last sequence element*/
        for (i = 3; i< (SIZE) + 1; i++) {
            generated[SIZE-4-spaces] = i;

            /*Prints the current sequence values*/
            /*for ( j = 0; j < SIZE-4; j++) printf("%s%d %s",  j==0?"\nNew sequence: [":"", generated[j], j< SIZE-5 ? ",\0":"]\n");*/

            /*printf("\n\n\nWill generate a new tree:\n\n\n");*/

            /*SCount++;*/
            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);
            /*break;*/
        }
    }
}

/*Function that generates trees from Prüfer sequences WITH SUPER NODE*/
void generate_tree(int *seq){
    /*NOTE: The decodig algorithm implemented in this functions runs in O(n), as is explained in Wang et al; "An Optimal Algorithm for Prüfer Codes" J. Software Engineering & Applications, 2009, 2 (111-115)*/
    int i, j;
    int degree[SIZE-2];
    int index = 0, x = 0, y; /*auxiliary variable to the linear time decoding of the Prüfer sequence */
    /*int edgesSuperNode[SIZE-3] = {0};*/ /*values from 3 to N*/
    /*variables to the adjacency list*/
    int a, b;
/*    int next[SIZE-3] = {SIZE};
    int index_next = 0;*/

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

        /*Adjacency List*/
        if ((a = max(x, y)) == SIZE) {
            b = min(x,y);
            list[a][list_index[a]++] = b;
        }
        else {
            list[x][list_index[x]++] = y;
            list[y][list_index[y]++] = x;
        }
        /*print_list();*/

        /*Adjacency Matrix*/
        /*lines[min(x, y)][max(x, y)] = TREE;*/

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
    /*print_list();*/

    /*Adjacency List*/
    if ((a = max(x, y)) == SIZE) {
        b = min(x,y);
        list[a][list_index[a]++] = b;
    }
    else {
        list[x][list_index[x]++] = y;
        list[y][list_index[y]++] = x;
    }

    /*printf("Adj List temporary: \n");
    print_list();*/

    /*Adjacency Matrix*/
    /*lines[min(x, y)][max(x, y)] = TREE;*/

    /*Delete repeated nodes in Adjacency list*/
    for (i = 0; (i < SIZE) && ((a = list[SIZE][i]) > -1); i++) {
        /*printf("a : %d\n", a);*/
        verify_node(a);
    }

    /*check edges connecting to SUPER NODE */
    /*for (i = 3, j = 0; i < SIZE; i++) {
        if (lines[i][SIZE] == TREE) {
            edgesSuperNode[j++] = i;
        }
    }*/
    /*printf("\nMatrix with super nodes\n");*/
    /*print_adj();*/
    /*print_list();*/

    /*Generates trees from combinations of edges to super nodes*/
    /*Adjacency Matrix*/
    /*recursive(j, 0, edgesSuperNode);*/

    /*Adjacency List*/
    recursive_list(0);

    /*Clear Adjacency Matrix*/
    /*clear_adj();*/
    
    /*Clear Adjacency List*/
    clear_list();
}

/*Function used to delete repeated in adjacency list*/
void verify_node(int node) {
    /*NOTE: For every other node connected to the given node, which is nearest the root, it deletes the extra edge (connecting from leaf to root) and sends every other node to be checked too.*/

    int i, j;
    int b;

    /*for every node connecting to node*/
    for (i = 0; (i < SIZE) &&((b = list[node][i]) > -1)  ; i++ ) {
        /*checks the information about the edge in the node farthest from the root*/
        for (j = 0 ; (j < SIZE) && (list[b][j] > -1) ; j++) {
            if (list[b][j] == node) {
                /*found the edge that was looking for*/
                list[b][j] = 0;
                break;
            }
        }
        verify_node(b);
    }
}

/*Function used to generate combinations of connections to super node*/
void recursive_list(int index) {
    int i;
    if (list[SIZE][index+1] > -1) { /*if the next one is not the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            recursive_list(index + 1);
            list[i][--list_index[i]] = 0;
        }
    }
    else{ /*if the next one is the last*/
        for (i = 0; i < 3; i++) {
            list[i][list_index[i]++] = list[SIZE][index];
            /*printf("newly generated adjcency list : \n");*/
            /*print_list();*/
            NCodigos++;
            /*generate_graph();*/
            list[i][--list_index[i]] = 0;
        }
    }
}

/*Function that generates the neutral network graph from the tree*/
void generate_graph(void) {
    /*NOTE: This function populates the Zeros array of the neutral neutwork and calculates the graph's edges*/
    /*It operates in two steps: FIRST, from the tree root(0), checks all nodes directly connected. For each, searches the corresponding word, with Hamming distance 1 and with the corresponding Syndrome. Then, recursively does it for the other nodes. SECONDLY, Knowing that the graph has at least the tree edges, the edges with 0(zero) in the tree adjacency matrix are tested for their Hamming distance, being set to one in the graph adjacency matrix if distance is 1.*/
    /*WARNING: The graph's edges info is in the form of the second bit of the adjacency matrix. Changing this way of doing implies refactoring this function*/

    int i, k;
    int a;
    /*int j;*/ /*used for printing*/
    int next = 1; /*next zero to be set*/
    int count = 1; /* number of zeros defined*/
    /*uint16_t t_s, t_w; *//*temporary syndrome and word*/
    /*uint16_t next_z[SIZE] = {0, 1, 2};*/
    
    
    /*for the first known elements of the sequence: 0, 1, 2*/
    /*for (k = 0; k<3; k++) {
        if (count == SIZE) {
            goto CUIDADO;
        }
        for (i = 3; i < SIZE; i++) {
            if (count == SIZE) {
                goto CUIDADO;
            }
            if (lines[k][i] == 1) {
                count++;
                Z[i] = Z[k]^ZAux[(k^i)];
                next_z[next++] = i;
            }
        }
    }*/

    /*for each element of next_z, all besides 0, 1, 2*/
    for (k = 0 ; k < SIZE - 3; k++) {
        if (count == SIZE) {
            goto CUIDADO;
        }
        /*Checks edges in adjacency lines*/
        for (i = next_z[k] + 1; i < SIZE; i++) {
            if (count == SIZE) {
                goto CUIDADO;
            }
           /* if (lines[next_z[k]][i] == 1 && Z[i] == 0) {
                count++;
                Z[i] = Z[next_z[k]] ^ ZAux[(next_z[k]^i)];
                next_z[next++]=i;
            }*/
        }
        /*and columns*/
        for (i = 3; i < next_z[k]; i++){ /*start in 3 because 1st iterations are with z = 0, 1, 2, and there is no need to repeat*/
            if (count == SIZE) {
                goto CUIDADO;
            }
            /*if (lines[i][next_z[k]] == 1 && Z[i] == 0) {
                count++;
                Z[i] = Z[next_z[k]] ^ ZAux[(next_z[k]^i)];
                next_z[next++] = i;
            }*/
        }
    }

    CUIDADO:

    /*NCodigos++;*/

    /*print zero array*/
    /*for ( j = 0; j < SIZE; j++) printf("%s %d %s",  j==0?"\nZ = [":"", Z[j], j< SIZE-1 ? ",\0":"]\n");*/

    /*clear Z*/
    for (a = 3; a < SIZE; a++) {
        Z[a] = 0;
    }
    /*printf("\n");*/

}

/*Function that attributes zeros to the nodes, in a depth search*/
void check_node(int node) {
    int i;
    int a;

    for(i = 0; i < list_index[node]; i++) {
        a = list[node][i];
        if (a != 0) {
            Z[a] = Z[node] ^ ZAux[node ^ a];
            check_node(a);
        }
    }
}

/*Function that prints the adjacency list*/
void print_list(void) {
    /*NOTE: */
    int i, j;
    
    printf("Adjacency list: \n");
    for (i = 0; i < SIZE+1; i++) {
        printf("%d :  ", i);
        for (j = 0;  (j < SIZE) && (list[i][j] > -1); j++) {
            printf("%d, ", list[i][j] );
        }
        printf("\b\b  \n");
    }
        printf("\n");
}


/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/
void clear_list(void) {
    int i;

    /*Clear Adjacancy List*/
    for (i = 2; i < (SIZE+1)*(SIZE-1); i++) {
        list[0][i] = -1;
    }

    /*Reset the information of the list indexes*/
    list_index[0] = 2;
    for (i = 1; i < SIZE + 1;i++) {
        list_index[i] = 0;
    }
}
