#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 4
/*TODO struct tree {

};*/

/*Função que gera as árvores a partir das sequencias de Prüfer */
void gerar_tree(int *seq){
    int i, j;
    int degree[N+1];
    for (i = 0; i <= N; i++){
        degree[i] = 1;
    }
    for (i = 0; i < N-1; i++){
        printf("check2\n");
        degree[seq[i]] += 1;
    }
    printf("%d\n", degree[N]);
    
    for (i=0; i < N-1 ; i++) { /* elementos da seq : 0 -> N-1 */
        for (j = 0; j <= N; j++) { /*elementos do degree*/
            if (degree[j] == 1) {
                /*insert edge (seq[i], j)*/
                degree[j] -= 1;
                degree[seq[i]] -= 1;
                break;
            }
        }
    }
}

/*Função recursiva que gera as sequências de Prüfer que vão gerar as árvores*/
void gerar_seq(int casas, int *gerado) {

    int i, j;
    if (casas > 1)
        for( i = 0; i <= N; i++) {
            gerado[N-1-casas] = i; /*i porque os valores são de 0 a n, e i vai de 0 a n */
            gerar_seq (casas-1, gerado);
        }
    else
    /*Está na última casa da sequência*/
        for ( i = 0; i<=N ;i++){
            gerado[N-1-casas] = i; /*i+1 porque os valores são de 1 a n, e i vai de 0 a n-1*/
            
            /*Imprime os valores das sequências*/
            printf("Nova sequência: [");
            for ( j = 0; j < N-1; j++) {
                printf("%d %s", gerado[j], j< N-2 ? ",\0":"]\n");
            }
            printf("Vai gerar nova árvore:\n");
            gerar_tree(gerado);
         }
}

int main() {
    
    int gerado[N-1];
    gerar_seq(N-1, gerado);
    
    return 0;
}
