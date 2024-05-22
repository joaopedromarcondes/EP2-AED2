#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 2

typedef int TipoChave;
typedef struct str_no {
    TipoChave chave[2*t-1];
    struct str_no* filhos[2*t];
    int numChaves;
    bool folha;
} NO;

typedef struct {
NO* raiz;
} ArvoreBMais;


int main() {
    return 0;
}