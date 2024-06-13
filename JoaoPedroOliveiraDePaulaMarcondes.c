#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define t 3

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

NO* criaNo() {
    NO* novo = (NO*) malloc(sizeof(NO));
    int a;
    for (a = 0; a < 2*t; a++) {
        novo->chave[a] = -1;
        novo->filhos[a] = NULL;
    }
    novo->folha = false;
    novo->numChaves = 0;
    return novo;
}

void inicializarArvoreBMais(ArvoreBMais* arv) {
    arv->raiz = NULL;
}

void insereSeNaoEstaCheio(NO* p, int registro) {
    p->chave[p->numChaves] = registro;
    p->numChaves++;
}

void insereNoCheio(NO* x, int i, NO* y) {
    NO* z = (NO*) malloc(sizeof(NO));
    z->folha = y->folha;
    z->numChaves = t;

}


void adicionarArvoreBMais(ArvoreBMais* arv, int registro) {
    NO* p = arv->raiz;
    if (!p) {
        arv->raiz = (NO*) malloc(sizeof(NO));
        arv->raiz->chave[0] = registro;
        arv->raiz->numChaves = 1;
        arv->raiz->folha = true;
        return;
    }
    if (p->numChaves < 2*t-1) {
        p->chave[p->numChaves] = registro;
        p->numChaves++;
        return;
    }



}

void removerArvoreBMais(ArvoreBMais* arv, int registro) {

}

void mostrarArvoreBMais(ArvoreBMais* arv) {

}


int main() {
    printf("oi\n");

    ArvoreBMais arv;
    inicializarArvoreBMais(&arv);

    adicionarArvoreBMais(&arv, 10);
    adicionarArvoreBMais(&arv, 15);
    adicionarArvoreBMais(&arv, 20);
    adicionarArvoreBMais(&arv, 10);









    return 0;
}