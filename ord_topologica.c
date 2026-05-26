/*
 * Ordenação Topológica
 * Implementações:
 *   1. Ordenação Topológica via DFS (busca em profundidade)
 *   2. Algoritmo de Kahn
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 10001

/* Lista de adjacência */
typedef struct No {
    int vertice;
    struct No *prox;
} No;

typedef struct {
    No *cabeca;
} Lista;

/* Pilha (usada pela DFS) */
typedef struct {
    int dados[MAX_V];
    int topo;
} Pilha;

/* Fila (usada pelo Kahn) */
typedef struct {
    int dados[MAX_V];
    int inicio, fim;
} Fila;


void lista_inserir(Lista adj[], int u, int v) {
    No *novo = (No *)malloc(sizeof(No));
    novo->vertice = v;
    novo->prox = adj[u].cabeca;
    adj[u].cabeca = novo;
}

void pilha_iniciar(Pilha *p)          { p->topo = -1; }
void pilha_empurrar(Pilha *p, int x)  { p->dados[++p->topo] = x; }
int  pilha_vazia(Pilha *p)            { return p->topo == -1; }
int  pilha_desempilhar(Pilha *p)      { return p->dados[p->topo--]; }

void fila_iniciar(Fila *f)            { f->inicio = f->fim = 0; }
void fila_enfileirar(Fila *f, int x)  { f->dados[f->fim++] = x; }
int  fila_vazia(Fila *f)              { return f->inicio == f->fim; }
int  fila_desenfileirar(Fila *f)      { return f->dados[f->inicio++]; }

/* Cores para a DFS */
#define BRANCO 0
#define CINZA  1
#define PRETO  2

int  cor_dfs[MAX_V];
int  tempo_dfs;
Pilha pilha_resultado;

void dfs_visitar(Lista adj[], int u) {
    cor_dfs[u] = CINZA;
    tempo_dfs++;

    No *vizinho = adj[u].cabeca;
    while (vizinho != NULL) {
        int v = vizinho->vertice;
        if (cor_dfs[v] == BRANCO)
            dfs_visitar(adj, v);
        vizinho = vizinho->prox;
    }

    /* Vértice terminado: insere na pilha (passo 2) */
    cor_dfs[u] = PRETO;
    tempo_dfs++;
    pilha_empurrar(&pilha_resultado, u);
}

void ordenacao_topologica_dfs(Lista adj[], int n) {
    /* Inicializa cores */
    for (int i = 1; i <= n; i++)
        cor_dfs[i] = BRANCO;

    tempo_dfs = 0;
    pilha_iniciar(&pilha_resultado);

    /* Passo 1: executa DFS para todo vértice não visitado */
    for (int i = 1; i <= n; i++)
        if (cor_dfs[i] == BRANCO)
            dfs_visitar(adj, i);

    /* Passo 3: imprime a pilha (ordem topológica) */
    printf("DFS - Ordenação topológica: ");
    while (!pilha_vazia(&pilha_resultado))
        printf("%d ", pilha_desempilhar(&pilha_resultado));
    printf("\n");
}

int grau_entrada[MAX_V];
int solucao[MAX_V];

int kahn(Lista adj[], int grau[], int n) {
    Fila Q;
    fila_iniciar(&Q);
    int tam_S = 0;

    /* Insere na fila todos os vértices com grau de entrada 0 */
    for (int i = 1; i <= n; i++)
        if (grau[i] == 0)
            fila_enfileirar(&Q, i);

    while (!fila_vazia(&Q)) {
        int v = fila_desenfileirar(&Q);
        solucao[tam_S++] = v;           /* insere(S, v) */

        No *vizinho = adj[v].cabeca;
        while (vizinho != NULL) {
            int w = vizinho->vertice;
            grau[w]--;                  /* remove arco vw */
            if (grau[w] == 0)
                fila_enfileirar(&Q, w);
            vizinho = vizinho->prox;
        }
    }

    if (tam_S == n) {
        printf("Kahn - Ordenação topológica: ");
        for (int i = 0; i < tam_S; i++)
            printf("%d ", solucao[i]);
        printf("\n");
        return 1;
    }

    /* Ciclo detectado */
    printf("Grafo contém ciclo! Nenhuma ordenação topológica possível.\n");
    return 0;
}

int main() {
    printf("=================================================\n");
    printf("  Ordenação Topológica - Teoria dos Grafos\n");
    printf("=================================================\n\n");
    printf("--- Exemplo 1: Professor Bumstead ---\n");
    printf("Vértices: 1=camisa 2=gravata 3=paletó 4=cinto\n");
    printf("          5=calças 6=cuecas  7=sapatos 8=meias 9=relógio\n\n");

    int n1 = 9;
    Lista adj1[MAX_V];
    int   grau1[MAX_V];

    for (int i = 1; i <= n1; i++) { adj1[i].cabeca = NULL; grau1[i] = 0; }

    int arestas1[][2] = {
        {1,2},{1,4},{2,3},{4,3},{6,5},{6,7},{5,7}
    };
    int m1 = sizeof(arestas1) / sizeof(arestas1[0]);

    for (int i = 0; i < m1; i++) {
        int u = arestas1[i][0], v = arestas1[i][1];
        lista_inserir(adj1, u, v);
        grau1[v]++;
    }

    ordenacao_topologica_dfs(adj1, n1);

    for (int i = 1; i <= n1; i++) grau1[i] = 0;
    for (int i = 0; i < m1; i++) grau1[arestas1[i][1]]++;

    kahn(adj1, grau1, n1);

    printf("\n--- Exemplo 2: Fábrica de Cadeiras de Balanço ---\n");
    printf("Tarefas: 1=Seleção madeira  2=Arcos  3=Assento  4=Encosto\n");
    printf("         5=Braços  6=Tecido  7=Almofada  8=Montagem\n");
    printf("         9=Fix.Braços 10=Fix.Arcos 11=Verniz 12=Almofada\n\n");

    int n2 = 12;
    Lista adj2[MAX_V];
    int   grau2[MAX_V];

    for (int i = 1; i <= n2; i++) { adj2[i].cabeca = NULL; grau2[i] = 0; }

    int arestas2[][2] = {
        {1,2},{1,3},{1,4},{1,5},
        {6,7},
        {3,8},{4,8},
        {5,9},{8,9},
        {2,10},{8,10},
        {9,11},{10,11},
        {7,12},{11,12}
    };
    int m2 = sizeof(arestas2) / sizeof(arestas2[0]);

    for (int i = 0; i < m2; i++) {
        int u = arestas2[i][0], v = arestas2[i][1];
        lista_inserir(adj2, u, v);
        grau2[v]++;
    }

    kahn(adj2, grau2, n2);

    return 0;
}