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
#define N       7
#define SIZE    (N+1)
#define K       (N-3)
#define G      ((uint16_t ) 0xb )
/**/

/* Data for the NN(15,11) codes*/
/*#define N       15
#define SIZE    (N+1)
#define K       (N-4)
#define G      ((uint16_t ) 0x13)
*/

#define TREE    1
#define GRAPH   (1<<1)
#define MINTREE (1<<2)


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

typedef char bool;
#define true 1
#define false 0

/*Triangular Adjacency matrix*/
ele_t adjacency[(SIZE+1)*(SIZE)/2];
ele_t *lines[SIZE];


uint16_t ZAux[SIZE] = {0};
uint16_t Z[SIZE];

/*unsigned long long int NCodigos = 0;*/
int SCount = 0;

/*int aaa = 0, bbb = 0, ccc = 0;*/

/*FILE *f_seq , *f_cod;*/

/******************************************************************/
/************************   MAIN   ********************************/
/******************************************************************/
int main() {

    int generated[SIZE-4];
    int i;
    int a = -1;
    /*int j;
    uint16_t c = 0, b, t;*/
    
    /*f_seq = fopen("seq_edges.txt", "w");
    f_cod = fopen("cod_cortes.txt", "w");*/
    
    /*Generate adjacency matrix and respective lines pointer array*/
    for (i=0; i < SIZE; i++) {
        lines[i] = adjacency + a;
        a += SIZE - i - 1; /*a += SIZE - 2 - i;*/
    }
    lines[0][1] = lines[0][2] = 1;
    
    /*Generates array of words with each syndrome which have */
    /*for (j = 0; j < N; j++) {*/
        /*creates the word and checks syndrome*/
/*        b = c ^ (uint16_t)(1<<j);
        t = syndrome(N, K, G, b);*/
        
        /*printf("j : %d\t word : %d\t syndrome : %d \n", j, b, t);*/
        /*saves zero, records next_z*/
        /*printf("palavra: %d, sindrome: %d\n", b, t);*/
  /*      ZAux[t] = b;
    }*/
    /*printf("ZAux: ");
    for(j = 1; j < SIZE; j++)  printf("%s %d %s", j==1 ? "ZAUX : ":"", ZAux[j], j ==SIZE -1?"\n":" "); */
    /*Generate Prüfer sequences*/
    generate_seq(SIZE-4, generated);

    /*printf("Número de códigos: %lld\n", NCodigos);*/
    printf("numero de sequências: %d\n", SCount);
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
    /*int j;*/ /*used for printing*/
    
    if (spaces > 1) {
        for( i = 3; i < (SIZE)+1; i++) {
            /*if (i == 8) break;*/
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

            /*printf("Will generate a new tree:\n");*/

            SCount++;
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
    int edgesSuperNode[SIZE-3] = {0}; /*values from 3 to N*/

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
        lines[min(x, y)][max(x, y)] = TREE; /*Adjacency Case*/
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
    lines[min(x, y)][max(x, y)] = TREE; /*Adjacency Case*/

    /*check edges connecting to SUPER NODE */
    for (i = 3, j = 0; i < SIZE; i++) {
        if (lines[i][SIZE] == TREE) {
            edgesSuperNode[j++] = i;
        }
    }
    /*printf("\nMatrix with super nodes\n");
    print_adj();*/
    recursive(j, 0, edgesSuperNode);

    /*Clear Adj matrix*/
    clear_adj();
}


void recursive(int degree, int next_edge, int *edges) {
    int i;
    if (degree > 1) {
        for (i = 0; i < 3; i++) {
            lines[i][edges[next_edge]] = 1;
            recursive(degree - 1, next_edge + 1, edges);
            lines[i][edges[next_edge]] = 0;
        }
    }
    else{
        for (i = 0; i < 3; i++) {
            lines[i][edges[next_edge]] = 1;
            /*print_adj();*/
            /*NCodigos++;*/
            /*generate_graph();*/
            lines[i][edges[next_edge]] = 0;
        }
    }
}

/*Function that generates the neutral network graph from the tree*/
void generate_graph() {
    /*NOTE: This function populates the Zeros array of the neutral neutwork and calculates the graph's edges*/
    /*It operates in two steps: FIRST, from the tree root(0), checks all nodes directly connected. For each, searches the corresponding word, with Hamming distance 1 and with the corresponding Syndrome. Then, recursively does it for the other nodes. SECONDLY, Knowing that the graph has at least the tree edges, the edges with 0(zero) in the tree adjacency matrix are tested for their Hamming distance, being set to one in the graph adjacency matrix if distance is 1.*/
    /*WARNING: The graph's edges info is in the form of the second bit of the adjacency matrix. Changing this way of doing implies refactoring this function*/

    int i, k;
    int a;
    int next = 1; /*next zero to be set*/
    int count = 1; /* number of zeros defined*/
    /*uint16_t t_s, t_w; *//*temporary syndrome and word*/
    uint16_t next_z[SIZE] = {0, 1, 2};

    /*for the first known elements of the sequence: 0, 1, 2*/
    for(k = 0; k<3; k++) {
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
    }
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
            if (lines[next_z[k]][i] == 1 && Z[i] == 0) {
                count++;
                Z[i] = Z[next_z[k]]^ZAux[(next_z[k]^i)];
                next_z[next++]=i;
            }
        }
        /*and columns*/
        for (i = 3; i < next_z[k]; i++){ /*start in 3 because 1st iterations are with z = 0, 1, 2, and there is no need to repeat*/
            if (count == SIZE) {
                goto CUIDADO;
            }
            if (lines[i][next_z[k]] == 1 && Z[i] == 0) {
                count++;
                /*Z[i] = Z[next_z[k]]^ZAux[(next_z[k]^i)];*/
                next_z[next++] = i;
            }
        }
    }
    /*print Z*/
    /*printf("Zero vector: \n");*/
    CUIDADO:
    for (a = 0; a < SIZE; a++) {
        /*printf("%d%c ", Z[a], a < N ? ',' : '\n');*/
        Z[a] = 0;
    }
    /*printf("\n");*/

}



/*Function that prints the adjacency matrix of a given tree*/
void print_adj() {
    /*NOTE: Print adjacency matrix using array of pointers lines, and alligned to right.*/ 
    /*WARNING: Size of strings is hard coded.*/

    int i, j;

    /*Print using adacency matrix by lines*/
    printf("Adjacency matrix:\n");
    char temp[128];
    for (i=0; i < (SIZE); i++) {
        temp[0] = '\0';
        for (j = i+1; j < (SIZE+1); j++) {
            sprintf(temp, "%s%2d%c ", temp, lines[i][j], j<(SIZE) ? ',' : ' ');
        }
        printf("%32s\n", temp);
    }
}

/*Function that clears an adjacency matrix BUT THE (0,1) and (0,2) edges*/
void clear_adj() {
    /*NOTE: sets all values to 0*/
    int i;
    for (i=2; i < (SIZE)*(SIZE+1)/2; i++) {
        adjacency[i] = 0;
    }
}

/*Function that verifies if the sequance is valid or not*/
void check_seq(int *seq) {
    /*NOTE: the sequence is valid if both edges (0,1) and (0,2) are in the tree, so this verification guaranties if the sequence fulfills the requirements for that to be true.*/
    int i = 0;
    int last_zero = -1, last_one = -1, last_two = -1;/*index of the last corresponding occurrence.Only used if it exists.*/

    /*Verify if 0, 1 and 2 is in the sequence*/
    for (i = (SIZE - 3); i >= 0; i--) {
        if (last_one < 0 && seq[i] == 1) {
            last_one = i;
        }
        else if (last_two < 0 && seq[i] == 2) {
            last_two = i;
        }
        else if (last_zero < 0 && seq[i] == 0) {
            last_zero = i;
        }
    }
    if (last_zero > -1) {
        if ( last_one < 0 ) {
            if ( last_two < 0 ) {                             /* First Case */
                if ((seq[0] == 0) && (seq[1] == 0)) {
                    /*printf("\tAPPROVED!!!!!!\n");*/
                    /*NCodigos++;*/
                    generate_tree(seq);
                  /*          fprintf(f_cod, "[");
                    for ( j = 0; j < SIZE-2; j++) {
                        fprintf(f_cod,"%d %s", seq[j], j< SIZE-3 ? ",\0":"]\n");
                    }*/
                }
                else {
                    /*printf("\tREJECTED\n");*/
                    return;
                }
            }
            else {                                      /* Second Case */
                /* Checks ...02... OR ...20... AND 0...*/
                if (seq[0] == 0) {
                    if ((last_zero != (SIZE-3) && seq[last_zero + 1] == 2) || (last_two != (SIZE-3) && seq[last_two + 1] == 0)) {
                        /*printf("\tAPPROVED!!!!!!\n");*/
                        /*NCodigos++;*/
                        generate_tree(seq);
                      /*      fprintf(f_cod, "[");
                        for ( j = 0; j < SIZE-2; j++) {
                            fprintf(f_cod,"%d %s", seq[j], j< SIZE-3 ? ",\0":"]\n");
                        }*/
                    }
                    else {
                        /*printf("\tREJECTED\n");*/
                        return;
                    }
                }
                else {
                    /*printf("\tREJECTED\n");*/
                    return;
                }
            }
        }
        else {
            if ( last_two < 0 ) {                             /* Third Case */
                /* Checks ...01... OR ...10... AND 0...*/
                if (seq[0] == 0) {
                    if ((last_zero != (SIZE-3) && seq[last_zero + 1] == 1) || (last_one != (SIZE-3) && seq[last_one + 1] == 0)) {
                        /*printf("\tAPPROVED!!!!!!\n");*/
                        /*NCodigos++;*/
                        generate_tree(seq);
                     /*       fprintf(f_cod, "[");
                        for ( j = 0; j < SIZE-2; j++) {
                            fprintf(f_cod,"%d %s", seq[j], j< SIZE-3 ? ",\0":"]\n");
                        }*/
                    }
                    else {
                        /*printf("\tREJECTED\n");*/
                        return;
                    }
                }
                else {
                    /*printf("\tREJECTED\n");*/
                    return;
                }
            }
            else {                                      /* Forth Case */
                /* Checks ...10...02... OR ...20...01... */
                if (((last_one != (SIZE-3) && seq[last_one + 1] == 0) && ((last_zero != (SIZE-3) && seq[last_zero + 1] == 2)||(last_two != (SIZE-3) && seq[last_two + 1] == 0))) || ((last_two != (SIZE-3) && seq[last_two + 1] == 0) && ((last_zero != (SIZE-3) && seq[last_zero + 1] == 1)||(last_one != (SIZE-3) && seq[last_one + 1] == 0)))) {
                        /*printf("\tAPPROVED!!!!!!\n");*/
                        /*NCodigos++;*/
                        generate_tree(seq);
                            /*fprintf(f_cod, "[");
                        for ( j = 0; j < SIZE-2; j++) {
                            fprintf(f_cod,"%d %s", seq[j], j< SIZE-3 ? ",\0":"]\n");
                        }*/
                    }
                else {
                    /*printf("\tREJECTED\n");*/
                    return;
                }
            }
        }
    }
    else {
        /*printf("\tREJECTED\n");*/
        return;
    }
}
