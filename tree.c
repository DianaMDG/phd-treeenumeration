#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N       4       /*N utilizado pela Marisol*/
#define SIZE    N+1     /*Número de bits utilizado pela representação*/

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/*typedef uint16_t ele_t;*/
typedef char ele_t;

/*Indexed array*/
struct node {
    int valor;
    struct node *children[N-1];
};

/*Matriz de adjacência triangular*/
ele_t adjacencia[(SIZE-1)*(SIZE)/2];
ele_t *linhas[SIZE-1];

/*ele_t adjacencia[SIZE][SIZE];*/

/*Função que gera as árvores a partir das sequencias de Prüfer */
void gerar_tree(int *seq){
    int i, j;
    int degree[SIZE];

    /*Primeiro passo da construção da árvore - construção array degree*/
    for (i = 0; i < SIZE; i++){
        degree[i] = 1;
    }
    for (i = 0; i < SIZE-2; i++){
        /*printf("check2\n");*/
        degree[seq[i]] += 1;
    }
    /*printf("%d\n", degree[SIZE-1]);*/

    /*Definição das arestas do grafo e destruição do array degree*/
    for (i=0; i < SIZE-2 ; i++) { /* elementos da seq : 0 -> SIZE-2 */
        for (j = 0; j < SIZE; j++) { /*elementos do degree*/
            if (degree[j] == 1) {
                /*insert edge (seq[i], j) TODO*/
                /*Caso adjacência*/
                /**(linhas[(int)min(seq[i],j)] + (int)((int)max(seq[i],j)-(int)min(seq[i],j)-1))= 1; + (int)((int)max(seq[i],j)-(int)min(seq[i],j)-1)*/
                linhas[seq[i]][j] = 1;
                /*Caso Array*/
                degree[j] -= 1;
                degree[seq[i]] -= 1;
                break;
            }
        }
    }
    /*Imprime a matriz de adjacencia*/
    printf("matriz de adjacencia: %4d\n", SIZE*(SIZE-1)/2);
    for (i=0;i< (SIZE*(SIZE-1)/2); i++){
        printf("%d ", adjacencia[i]);
    }
    printf("\n");
}

/*Função recursiva que gera as sequências de Prüfer que vão gerar as árvores*/
void gerar_seq(int casas, int *gerado) {

    int i, j;
    if (casas > 1)
        for( i = 0; i < SIZE; i++) {
            gerado[SIZE-2-casas] = i; /*i porque os valores são de 0 a n, e i vai de 0 a n */
            gerar_seq (casas-1, gerado);
        }
    else
    /*Está na última casa da sequência*/
        for ( i = 0; i< SIZE ;i++){
            gerado[SIZE-2-casas] = i; /*i+1 porque os valores são de 1 a n, e i vai de 0 a n-1*/

            /*Imprime os valores das sequências*/
            printf("Nova sequência: [");
            for ( j = 0; j < SIZE-2; j++)
                printf("%d %s", gerado[j], j< SIZE-3 ? ",\0":"]\n");
            printf("Vai gerar nova árvore:\n");
            /*Gerar árvore associada à sequência*/
            gerar_tree(gerado);
         }
}
/*MAIN*/
int main() {

    int gerado[SIZE-2];
    int a=-1, i;

    /*Gerar matriz de adjacência e acessos a ela*/
    for (i=0; i < SIZE-1; i++) {
        linhas[i] = adjacencia + a;
        a+=N-i;
    }

    /*Gerar sequências de Prüfer*/
    gerar_seq(SIZE-2, gerado);

    return 0;
}
