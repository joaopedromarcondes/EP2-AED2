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

void mostrarArvoreBMais(NO* p, int i, FILE* saida) {
    
    if (!p->folha) {
        fprintf(saida, "(");
        mostrarArvoreBMais(p->filhos[i], 0, saida);
        fprintf(saida, ") ");
    }
    
    fprintf(saida, "%d", p->chave[i]);
    if (i + 1 < p->numChaves) {
        fprintf(saida, " ");
        mostrarArvoreBMais(p, i+1, saida);
    }  else {
        if (!p->folha) {
            fprintf(saida, " (");
            mostrarArvoreBMais(p->filhos[i+1], 0, saida);
            fprintf(saida, ")");
        }
    }

}

void mostrar(ArvoreBMais* arv, FILE* saida) {
    if (arv->raiz->numChaves == 0) {
        fprintf(saida, "Vazia\n");
        return;
    }
    fprintf(saida, "(");
    mostrarArvoreBMais(arv->raiz, 0, saida);
    fprintf(saida, ")\n");
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

    if (p->filhos[pos]->numChaves == 2*t-1) {
        split(p, p->filhos[pos], pos);
        insereArvoreBMais(p, registro);
    } else {
        insereArvoreBMais(p->filhos[pos], registro);
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

    free(q);

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

void trocarChaveFilhoEsquerdo(NO* pai, NO* filho_esquerdo, NO* filho, int i) {
    int a;
    for (a = filho->numChaves; a > 0; a--) {
        filho->chave[a] = filho->chave[a-1];
        filho->filhos[a+1] = filho->filhos[a];
    }
    filho->numChaves++;

    filho->chave[0] = filho_esquerdo->chave[filho_esquerdo->numChaves-1];
    filho_esquerdo->chave[filho_esquerdo->numChaves-1] = -1;
    filho_esquerdo->numChaves--;

    filho->filhos[0] = filho_esquerdo->filhos[filho_esquerdo->numChaves];
    filho_esquerdo->filhos[filho_esquerdo->numChaves] = NULL;

    pai->chave[i] = filho->chave[0];
}

void trocarChaveFilhoDireito(NO* pai, NO* filho, NO* filho_direito, int i) {

    filho->chave[filho->numChaves] = filho_direito->chave[0];
    filho->filhos[filho->numChaves+1] = filho_direito->filhos[0];
    filho->numChaves++;
    int a;
    for (a = 0; a < filho_direito->numChaves; a++) {
        filho_direito->chave[a] = filho_direito->chave[a+1];
        filho_direito->filhos[a] = filho_direito->filhos[a+1];
    }

    filho_direito->chave[filho_direito->numChaves-1] = -1;
    filho_direito->numChaves--;

    filho_direito->filhos[filho_direito->numChaves] = NULL;

    pai->chave[i] = filho_direito->chave[0];
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
    
    NO* filho = p->filhos[i];
    if (filho->numChaves < t) {
        if (i > 0) {
            NO* filho_esquerda = p->filhos[i-1];
            if (filho_esquerda->numChaves < t) {
                merge(p, filho_esquerda, filho, i-1);
            } else {
                trocarChaveFilhoEsquerdo(p, filho_esquerda, filho, i-1);
            }
        } else {
            NO* filho_direita = p->filhos[i+1];
            if (filho_direita->numChaves < t) {
                merge(p, filho, filho_direita, i);
            } else {
                trocarChaveFilhoDireito(p, filho, filho_direita, i);
            }
        }

        removerArv(p, registro);
        return;
    }
    removerArv(p->filhos[i], registro);
    p->chave[i-1] = minimoFilhoEsquerdo(p->filhos[i]);

}

void removerArvoreBMais(ArvoreBMais* arv, int registro) {
    NO* p = arv->raiz;

    removerArv(p, registro);

    if (p->numChaves == 0) {
        arv->raiz = p->filhos[0];
    }

}




int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Erro. Passe dois argumentos como parametro. Ex: ./a.out entrada.txt saida.txt");
        return 1;
    }


    ArvoreBMais arv;
    inicializarArvoreBMais(&arv);

    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = fopen(argv[2], "w");

    if (!saida || !entrada) {
        fprintf(stderr, "Erro na abertura de algum dos arquivos.");
    }

    char comando;
    int registro;

    fscanf(entrada, "%c", &comando);
    
    while (comando != 'f') {

        switch (comando) {
            case 'p':
                mostrar(&arv, saida);
                break;
            
            case 'i':
                fscanf(entrada, "%d\n", &registro);
                //printf("Adicionar o registro: %d\n", registro);
                adicionarArvoreBMais(&arv, registro);
                break;

            case 'r':
                fscanf(entrada, "%d\n", &registro);
                //printf("Remover o registro: %d\n", registro);
                removerArvoreBMais(&arv, registro);
                break;
            
            default:
                break;
        }
        fscanf(entrada, "%c\n", &comando);

    }


    return 0;
}