#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 4
#define POW 16

/*int seq[(int) pow(N,N-2)][N-2]; /*Array de ponteiros para onde vão ser guardadas as sequencias de Prüfer*/
int seq[POW][N-2]; /*Array de ponteiros para onde vão ser guardadas as sequencias de Prüfer*/
int linha_seq=0;

/*Função recursiva que gera as sequências de Prüfer que vão gerar as árvores*/
void gerar(int n, int casas, int gerado[]) {

    int i, j;
    if (casas > 1)
        for( i = 0; i < n; i++) {
            gerado[n-2-casas] = i+1; /*i+1 porque os valores são de 1 a n, e i vai de 0 a n-1 */
            /*printf("%d %c", gerado[j], j< n-3-casas ? ',':']');*/
            gerar (n, casas-1, gerado);
        }
    else
    /*Está na última casa da sequência*/
        for ( i = 0; i<n ;i++) {
            gerado[n-2-casas] = i+1; /*i+1 porque os valores são de 1 a n, e i vai de 0 a n-1*/
            /*printf("Nova sequência: [");
            for ( j = 0; j < n-2; j++) {*/
                printf("%d %s", gerado[j], j< n-3 ? ",\0":"]\n", seq);
                /*seq[linha_seq][j] = gerado[j];*/
            }
            /*copiar gerado para *seq*/
            linha_seq += 1;
        }
}

int main() {
    
    int gerado[N-2];
    int i, j;
    gerar(N, N-2, gerado);
    
    /*imprimir a tabela das sequencias*/
    printf("Sequências: [");
    for (i = 0; i < POW; i++) {
        for (j = 0; j < N-2; j++) {
            printf("%d %s", seq[i][j], j< N-3 ? ",\0":"]\n[");
            /*seq[i][j] = gerado[j];*/
        }
    }
    return 0;
}
