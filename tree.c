#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 7



/*Função recursiva que gera as sequências de Prüfer que vão gerar as árvores*/
void gerar(int n, int casas, int gerado[]) {

    int i, j;
    if (casas > 1)
        for( i = 0; i <= n; i++) {
            gerado[n-2-casas] = i; /*i porque os valores são de 0 a n, e i vai de 0 a n */
            gerar (n, casas-1, gerado);
        }
    else
    /*Está na última casa da sequência*/
        for ( i = 0; i<=n ;i++)
            gerado[n-2-casas] = i; /*i+1 porque os valores são de 1 a n, e i vai de 0 a n-1*/
            /*Imprime os valores das sequências*/
            /*printf("Nova sequência: [");
            for ( j = 0; j < n-2; j++) {
                printf("%d %s", gerado[j], j< n-3 ? ",\0":"]\n");
            }*/
}

int main() {
    
    int gerado[N-2];
    gerar(N, N-2, gerado);
    
    return 0;
}
