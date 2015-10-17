#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint16_t h74_g = 0xb;               /* Hamming (7,4) generator polynomial */
uint16_t h74_codeword[1 << 4];      /* Hamming (7,4) codewords <=> V0(x)*/
uint16_t h74_codeword_1[1 << 4];      /* Hamming (7,4) codewords S1 <=> V1(x)*/
uint16_t h74_syndrome[1 << 7];      /* Hamming (7,4) syndromes */
uint16_t h74_decoding[1 << 7];        /* Decoding de todas as palavras código posíveis*/

uint16_t h1511_g = 0x13;            /* Hamming (15,11) generator polynomial */
uint16_t h1511_codeword[1 << 11];   /* Hamming (15,11) codewords */
uint16_t h1511_syndrome[1 << 15];   /* Hamming (15,11) syndromes */

/* The syndrome is the remainder of the division of the word w by the generator polynomial g */ 
uint16_t syndrome(uint16_t n, uint16_t k, uint16_t g, uint16_t w) {
    uint16_t i;
    for (i = 1; i <= k; i++)
        if (w & 0x1 << (n-i))
            w ^= g << (k-i);
    return w;
}

uint16_t encode(uint16_t n, uint16_t k, uint16_t g, uint16_t w) {
    uint16_t r = w << (n-k);
    return r ^ syndrome(n, k, g, r); /* V0(x)=x^(l-n).U(x) + x^(l-n).U(x)mod g(x)*/
}

/* z is an array of coset representatives characterising the representation */
uint16_t decode(uint16_t n, uint16_t k, uint16_t g, uint16_t *z, uint16_t w) {
    uint16_t s = syndrome(n, k, g, w);
    return (w ^ z[s]) >> (n-k);
}

/*calcula a palavra de código de todas as palavras de informação*/
void make_codewords(uint16_t n, uint16_t k, uint16_t g, uint16_t *c, uint16_t *z, uint16_t s) {
    uint16_t i;
    for (i = 0; i < (1 << k); i++)
        c[i] = encode(n, k, g, i) ^ z[s];
}

/*Calcula as síndromes para todas as palavras código */
void make_syndromes(uint16_t n, uint16_t k, uint16_t g, uint16_t *s) {
    uint16_t i;
    for (i = 0; i < (1 << n); i++)
        s[i] = syndrome(n, k, g, i);
}


/* TODO Função que calcula a conectividade da rede. */
/*int conectividade(uint16_t n, uint16_t k, uint16_t g, uint16_t *z) {
    uint16_t i, j;
    uint16_t * a;
    a = (uint16_t *) malloc(2^k);
    for (i = 0; i < (1 << 7); i++) {
        a[i] = 0;
    }
    for (i = 0; i < n; i++) { */ /*para cada zero*/
    /*    for (j = 0; j < n; j++) {*/ /* para cada mutação em torno de cada zero */
    /*        printf("teste %3o %3o", z[i] & (1<<j) ,decode(n, k, g, z, z[i] & (1<<j)));
            a[decode(n, k, g, z, z[i] & 1<<j)] += 1; *//* vê fenótipo */
    /*    }
    }
    free(a);
    return a[0]/(2^n);
}*/


/*int main() {
    uint16_t i;
    uint16_t R1[] = {0, 1, 2, 8, 4, 64, 16, 32};
    uint16_t R2[] = {0, 1, 2, 4, 8, 16, 32, 64};
    */
    /*make_codewords(7, 4, h74_g, h74_codeword, R, 0);
    printf("Hamming (7,4) code\n");
    for(i = 0; i < (1 << 4); i++)
        printf("%03o %03o\n", i, h74_codeword[i]);
    printf("\n");
    
    make_codewords(7,4,h74_g, h74_codeword_1, R, 1);
    printf("Hamming (7,4) code syndrome 1 %03o \n", R[1]);
    for(i = 0; i < (1 << 4); i++)
        printf("%03o %03o\n", i, h74_codeword_1[i]);
    printf("\n");
    */
    /*
    make_syndromes(7, 4, h74_g, h74_syndrome);
    printf("Hamming (7,4) syndromes\n");
    for(i = 0; i < (1 << 7); i++)
        printf("%03o %03o\n", i, h74_syndrome[i]);
    printf("\n");
    
    
    for(i = 0; i < (1 << 7); i++)
        printf("%03o %03o %03o\n", i, decode(7, 4, h74_g, R1, i), decode(7, 4, h74_g, R2, i));
    printf("\n");*/
/*    printf("decode de %03o = %03o\n", 0177, decode(7, 4, h74_g, R, 0177));*/
    
    /*printf("conectividade da rede = %03o\n", conectividade(7, 4, h74_g, R));*/

    /*make_codewords(15, 11, h1511_g, h1511_codeword);
    printf("Hamming (15,11) code\n");
    for(i = 0; i < (1 << 11); i++)
        printf("%04x %04x\n", i, h1511_codeword[i]);
    printf("\n");
    make_syndromes(15, 11, h1511_g, h1511_syndrome);
    printf("Hamming (15,11) syndromes\n");
    for(i = 0; i < (1 << 15); i++)
        printf("%04x %04x\n", i, h1511_syndrome[i]);
    
    return 0;
}*/

