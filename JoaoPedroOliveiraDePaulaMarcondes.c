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

void mostrarArvoreBMais(NO* p, int i) {
    
    if (!p->folha) {
        printf("(");
        mostrarArvoreBMais(p->filhos[i], 0);
        printf(") ");
    }
    
    printf("%d", p->chave[i]);
    if (i + 1 < p->numChaves) {
        printf(" ");
        mostrarArvoreBMais(p, i+1);
    }  else {
        if (!p->folha) {
            printf(" (");
            mostrarArvoreBMais(p->filhos[i+1], 0);
            printf(")");
        }
    }

}


NO* criarNo() {
    NO* novo = (NO*) malloc(sizeof(NO));
    int a;
    for (a = 0; a < 2*t; a++) {
        novo->chave[a] = -1;
        novo->filhos[a] = NULL;
    }
    novo->filhos[2*t] = NULL;
    novo->folha = false;
    novo->numChaves = 0;
    return novo;
}

void split(NO* pai, NO* p, int i) {
    NO* novo = criarNo();
    novo->folha = p->folha;

    // arruma NO novo
    int a;
    for (a = t; a < 2*t-1; a++) {
        novo->chave[a-t] = p->chave[a];
        p->chave[a] = -1;
        p->numChaves--;
        novo->numChaves++;

        novo->filhos[a-t] = p->filhos[a];
        p->filhos[a] = NULL;
    }

    novo->filhos[t-1] = p->filhos[2*t-1];
    p->filhos[2*t-1] = NULL;


    
    // atualiza filhos (Arvore B+)
    if (p->folha) {
        novo->filhos[2*t-1] = p->filhos[2*t-1];
        p->filhos[2*t-1] = novo;
    } 

    // ajusta vertice pai
    if (pai) {
        for (a = pai->numChaves; a > i+1; a--) {
            pai->chave[a] = pai->chave[a-1];
            pai->filhos[a] = pai->filhos[a-1];
        }
        pai->filhos[i] = p;
        pai->filhos[i+1] = novo;
        pai->chave[i] = novo->chave[0];
        pai->numChaves++;
    } 
}

void inicializarArvoreBMais(ArvoreBMais* arv) {
    arv->raiz = criarNo();
    arv->raiz->folha = true;
}

void insereSeNaoEstaCheio(NO* p, int registro) {

    int pos = 0;
    while (registro > p->chave[pos] && pos < p->numChaves) {
        pos++;
    }

    int a;
    for (a = p->numChaves; a > pos; a--) {
        p->chave[a] = p->chave[a-1];
    }
    
    p->chave[pos] = registro;
    p->numChaves++;
}

void insereArvoreBMais(NO* p, int registro) {
    int pos = 0;
    while (registro > p->chave[pos] && pos < p->numChaves) {
        pos++;
    }
    //printf("registro: %d, pos: %d\n", registro, p->folha);
    if (p->folha) {
        insereSeNaoEstaCheio(p, registro);
    } else {
        insereArvoreBMais(p->filhos[pos], registro);
        if (p->filhos[pos]->numChaves == 2*t-1) {
            split(p, p->filhos[pos], pos);
        }
        p->chave[pos] = p->filhos[pos]->chave[0];
    }
}


void adicionarArvoreBMais(ArvoreBMais* arv, int registro) {
    NO* p = arv->raiz;
    if (p->numChaves == 2*t-1) {
        NO* novo = criarNo();
        split(novo, p, 0);
        arv->raiz = novo;
        arv->raiz->folha = false;
        p = arv->raiz;
    }
    //printf("%d\n", p->chave[0]);
    insereArvoreBMais(p, registro);
}

void removerArvoreBMais(ArvoreBMais* arv, int registro) {

}




int main() {
    //printf("oi\n");

    ArvoreBMais arv;
    inicializarArvoreBMais(&arv);

    adicionarArvoreBMais(&arv, 10);
    adicionarArvoreBMais(&arv, 15);
    adicionarArvoreBMais(&arv, 20);
    adicionarArvoreBMais(&arv, 11);
    adicionarArvoreBMais(&arv, 25);
    adicionarArvoreBMais(&arv, 30);
    adicionarArvoreBMais(&arv, 9);
    //adicionarArvoreBMais(&arv, 12);
    //adicionarArvoreBMais(&arv, 8);
    //adicionarArvoreBMais(&arv, 13);
    //adicionarArvoreBMais(&arv, 23);


    mostrarArvoreBMais(arv.raiz, 0);
    printf("\n");






    return 0;
}