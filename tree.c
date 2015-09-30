#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N       4       /*N used in the neutral network NN(n,l) */
#define SIZE    N+1     /*Number of bis used by the representation */

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/*typedef uint16_t ele_t;*/
typedef char ele_t;

/*Indexed array*/
struct node {
    int value;
    struct node *children[N-1];
};

/*Triangular Adjacency matrix*/
ele_t adjacency[(SIZE-1)*(SIZE)/2];
ele_t *lines[SIZE-1];

/*ele_t adjacency[SIZE][SIZE];*/

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
    for (i = 0; i < SIZE-2 ; i++) { /* seq elements: 0 -> SIZE-2 */
        for (j = 0; j < SIZE; j++) { /* degree elements*/
            if (degree[j] == 1) {
                /*insert edge (seq[i], j) TODO*/
                /*Adjacency Case*/
                /**(lines[(int)min(seq[i],j)] + (int)((int)max(seq[i],j)-(int)min(seq[i],j)-1))= 1; + (int)((int)max(seq[i],j)-(int)min(seq[i],j)-1)*/
                lines[seq[i]][j] = 1;
                /*Array Case*/
                degree[j] -= 1;
                degree[seq[i]] -= 1;
                break;
            }
        }
    }
    /*print adjacency matrix*/
    printf("Adjacency matrix: %4d\n", SIZE*(SIZE-1)/2);
    for (i=0;i< (SIZE*(SIZE-1)/2); i++){
        printf("%d ", adjacency[i]);
    }
    printf("\n");
}

/*Function that prints the adjacency matrix of a given tree*/
void print_adj(ele_t adjacency[]) {

}

/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated) {
    /* spaces    - blank spaces in the sequence to fill up*/
    /* generated - sequence generated so far */
    /* NOTE: because the sequences are not saved, it is reused, and so, filled sequencially*/
    
    int i, j;
    if (spaces > 1) {
        for( i = 0; i < SIZE; i++) {
            generated[SIZE-2-spaces] = i; /*i because values go from 0 to n and i goes from 0 to n */
            generate_seq (spaces-1, generated);
        }
    }
    else {
        /*It is in the last sequence space*/
        for (i = 0; i< SIZE ;i++){
            generated[SIZE-2-spaces] = i;

            /*Prints the present sequence values*/
            printf("New sequence: [");
            for ( j = 0; j < SIZE-2; j++) {
                printf("%d %s", generated[j], j< SIZE-3 ? ",\0":"]\n");
            }
            printf("Will generate a new tree:\n");
            /*Generate tree associated to the sequence generated*/
            generate_tree(generated);
         }
     }
}
/*MAIN*/
int main() {

    int generated[SIZE-2];
    int i, j;
    int a = -1;

    /*Generate adjacency matrix and respective lines pointer array*/
    for (i=0; i < SIZE-1; i++) {
        lines[i] = adjacency + a;
        a += N - i - 1; /*a += SIZE - 2 - i;*/
    }
    /*TEST: Set matrix values*/
    for (i=0; i < (SIZE-1)*(SIZE)/2; i++) {
        adjacency[i] = i+1;
    }
    /*Print adjacency matrix*/
    printf("Print Adj matrix: %d\n",  (SIZE-1)*(SIZE)/2);
    for (i=0; i < (SIZE-1)*(SIZE)/2; i++) {
        printf("%d, ",adjacency[i]);
    }
    printf("\n");
    /*Print adjacency matrix by lines*/
    printf("Print adjacency matrix by lines:\n");
    char temp[128];
    for (i=0; i < SIZE-1; i++) {
        temp[0] = '\0';
        for (j = i+1; j < SIZE; j++) {
            /*printf("%d, ",linhas[i][j]);*/
            sprintf(temp, "%s%2d, ", temp, lines[i][j]);
        }
        printf("%16s\n", temp);
    }
    /*TEST: Change 2 lines of the matrix*/
    j=2;
    for (i=j+1; i < SIZE; i++) {
        lines[j][i] = 0;
    }
    /*Print adjacency matrix by lines*/
    printf("Print adjacency matrix by lines:\n");
    for (i=0; i < SIZE-1; i++) {
        for (j = i+1; j < SIZE; j++) {
            printf("%d, ",lines[i][j]);
        }
        printf("\n");
    }

    /*Generate Prüfer sequences*/
    generate_seq(SIZE-2, generated);

    return 0;
}
