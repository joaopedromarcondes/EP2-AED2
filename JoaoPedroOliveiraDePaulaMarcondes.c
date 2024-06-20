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

void mostrar(ArvoreBMais* arv) {
    if (arv->raiz->numChaves == 0) {
        printf("Vazia\n");
        return;
    }
    printf("(");
    mostrarArvoreBMais(arv->raiz, 0);
    printf(")\n");
}

void mostrarDetalhes(NO* p) {
    if (!p) {
        return;
    } 
    printf("\n");
    int a;
    if (p->folha) {
        printf("Eh folha: true\n");
    } else {
        printf("Eh folha: false\n");
    }
    
    printf("Quantidade de nos: %d\n", p->numChaves);
    printf("Nós: ");
    for (a = 0; a < 2*t-1; a++) {
        printf("%d ", p->chave[a]);
    }
    printf("\n");

    printf("Filhos: ");
    for (a = 0; a < 2*t; a++) {
        if (p->filhos[a]) {
            printf("%d ", a);
        } 
    }
    printf("\n");

    if (!p->folha) {
        for (a = 0; a < 2*t; a++) {
            mostrarDetalhes(p->filhos[a]);
        }
    }
}

int contarRegistros(ArvoreBMais* arv) {
    NO* p = arv->raiz;

    while (!p->folha) {
        p = p->filhos[0];
    }

    int cont = 0;
    while (p) {
        cont += p->numChaves;
        p = p->filhos[5];
    }
    return cont;
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

int minimoFilhoEsquerdo(NO* p) {
    if (p->folha) {
        return p->chave[0];
    }
    return minimoFilhoEsquerdo(p->filhos[0]);
}

void split(NO* pai, NO* p, int i) {
    NO* novo = criarNo();
    novo->folha = p->folha;

    if (p->folha) {
        // arruma NO novo
        int a;
        for (a = t-1; a < 2*t-1; a++) {
            novo->chave[a-t+1] = p->chave[a];
            p->chave[a] = -1;
            p->numChaves--;
            novo->numChaves++;
        }



        
        // atualiza filhos (Arvore B+)
        if (p->folha) {
            novo->filhos[2*t-1] = p->filhos[2*t-1];
            p->filhos[2*t-1] = novo;
        } 

        // ajusta vertice pai
        if (pai) {
            for (a = pai->numChaves; a > i; a--) {
                pai->chave[a] = pai->chave[a-1];
                pai->filhos[a+1] = pai->filhos[a];
            }
            pai->filhos[i] = p;
            pai->filhos[i+1] = novo;
            pai->chave[i] = novo->chave[0];
            pai->numChaves++;
        } 
        return;
    }




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

    p->numChaves--;
    p->chave[t-1] = -1;



    // ajusta vertice pai
    if (pai) {
        for (a = pai->numChaves; a > i; a--) {
            pai->chave[a] = pai->chave[a-1];
            pai->filhos[a+1] = pai->filhos[a];
        }
        pai->filhos[i] = p;
        pai->filhos[i+1] = novo;
        pai->chave[i] = minimoFilhoEsquerdo(novo);
        pai->numChaves++;
    } 
}

void inicializarArvoreBMais(ArvoreBMais* arv) {
    arv->raiz = criarNo();
    arv->raiz->folha = true;
}

void insereEmNoFolha(NO* p, int registro) {

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
    if (p->folha) {
        insereEmNoFolha(p, registro);
        return;
    }

    int pos = 0;
    while (registro > p->chave[pos] && pos < p->numChaves) {
        pos++;
    }
    //printf("registro: %d, pos: %d\n", registro, p->folha);
    insereArvoreBMais(p->filhos[pos], registro);
    if (p->filhos[pos]->numChaves == 2*t-1) {
        split(p, p->filhos[pos], pos);
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

void removerNoFolha(NO* p, int registro) {
    int i = 0;
    while (i < p->numChaves && registro > p->chave[i]) {
        i++;
    }

    if (p->chave[i] == registro) {
        int a;
        for (a = i; a < p->numChaves; a++) {
            p->chave[a] = p->chave[a+1];
        }
        p->chave[a] = -1;
        p->numChaves--;
    }

}

void merge(NO* pai, NO* p, NO* q, int i) {
    int a, b;
    for (a = p->numChaves, b = 0; b < q->numChaves; a++, b++) {
        p->chave[a] = q->chave[b];
    }
    p->numChaves += q->numChaves;
    p->filhos[2*t-1] = q->filhos[2*t-1];

    //free(q);

    // arruma pai
    int c;
    for (c = i; c < pai->numChaves; c++) {
        pai->chave[c] = pai->chave[c+1];
        pai->filhos[c+1] = pai->filhos[c+2];
    }
    pai->chave[c] = -1;
    pai->filhos[c] = NULL;
    pai->numChaves--;

    pai->chave[i-1] = minimoFilhoEsquerdo(pai->filhos[i]);
}

void removerArv(NO* p, int registro) {
    if (p->folha) {
        removerNoFolha(p, registro);
        return;
    }

    int i = 0; // indica o filho da direita da chave que quer remover
    while (registro >= p->chave[i] && i < p->numChaves) {
        i++;
    }
    removerArv(p->filhos[i], registro);
    p->chave[i-1] = minimoFilhoEsquerdo(p->filhos[i]);
    NO* filho = p->filhos[i];
    if (filho->numChaves < t) {
        if (i > 0) {
            NO* filho_esquerda = p->filhos[i-1];
            if (filho_esquerda->numChaves < t) {
                merge(p, filho_esquerda, filho, i-1);
            } else {
                // pegar a chave do lado
            }
        }
    }
}

void removerArvoreBMais(ArvoreBMais* arv, int registro) {
    NO* p = arv->raiz;

    removerArv(p, registro);

    if (p->numChaves == 0) {
        arv->raiz = p->filhos[0];
    }

}




int main() {
    //printf("oi\n");

    ArvoreBMais arv;
    inicializarArvoreBMais(&arv);

    mostrar(&arv);

    adicionarArvoreBMais(&arv, 30);

    mostrar(&arv);

    adicionarArvoreBMais(&arv, 9);
    adicionarArvoreBMais(&arv, 5);
    adicionarArvoreBMais(&arv, 8);
    adicionarArvoreBMais(&arv, 17);
    adicionarArvoreBMais(&arv, 26);

    mostrar(&arv);

    //adicionarArvoreBMais(&arv, 25);
    //adicionarArvoreBMais(&arv, 30);
    //adicionarArvoreBMais(&arv, 9);
    /*adicionarArvoreBMais(&arv, 12);
    adicionarArvoreBMais(&arv, 8);
    adicionarArvoreBMais(&arv, 13);
    adicionarArvoreBMais(&arv, 23);
    adicionarArvoreBMais(&arv, 24);
    adicionarArvoreBMais(&arv, 13);
    adicionarArvoreBMais(&arv, 7);
    adicionarArvoreBMais(&arv, 16);
    adicionarArvoreBMais(&arv, 17); */
    
    //removerArvoreBMais(&arv, 20);
    //removerArvoreBMais(&arv, 9);
    //removerArvoreBMais(&arv, 10);

    //removerArvoreBMais(&arv, 25);

    //adicionarArvoreBMais(&arv, 12);
    //adicionarArvoreBMais(&arv, 40);

    //mostrar(&arv);
    //printf("\n");
    //mostrarDetalhes(arv.raiz);

    int qnt = contarRegistros(&arv);

    printf("A arvore tem: %d registros\n", qnt);



    return 0;
}