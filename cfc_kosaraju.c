#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 100

// ─── Estrutura de lista de adjacência ───────────────────────────────────────

typedef struct No {
    int vertice;
    struct No *prox;
} No;

typedef struct {
    int V, E;
    No *adj[MAX_V];
} Grafo;

// ─── Funções auxiliares ──────────────────────────────────────────────────────

Grafo *criarGrafo(int V) {
    Grafo *g = malloc(sizeof(Grafo));
    g->V = V;
    g->E = 0;
    for (int i = 0; i < V; i++)
        g->adj[i] = NULL;
    return g;
}

void adicionarArco(Grafo *g, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->vertice = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
    g->E++;
}

// ─── Passo 2: Grafo transposto GT ────────────────────────────────────────────
// GT = (V, ET), onde ET = {(u,v) : (v,u) ∈ E}

Grafo *grafoTransposto(Grafo *g) {
    Grafo *gt = criarGrafo(g->V);
    for (int u = 0; u < g->V; u++) {
        No *p = g->adj[u];
        while (p) {
            adicionarArco(gt, p->vertice, u); // inverte orientação
            p = p->prox;
        }
    }
    return gt;
}

// ─── Passo 1: DFS em G para obter f[v] (ordem de finalização) ────────────────

int visitado[MAX_V];
int ordem[MAX_V]; // pilha de finalização
int topo;

void dfs1(Grafo *g, int u) {
    visitado[u] = 1;
    No *p = g->adj[u];
    while (p) {
        if (!visitado[p->vertice])
            dfs1(g, p->vertice);
        p = p->prox;
    }
    ordem[topo++] = u; // empilha ao finalizar (ordem crescente de f[v])
}

// ─── Passo 3: DFS em GT na ordem decrescente de f[v] ─────────────────────────

int componente[MAX_V]; // qual SCC cada vértice pertence
int numSCC;

void dfs2(Grafo *gt, int u) {
    visitado[u] = 1;
    componente[u] = numSCC;
    No *p = gt->adj[u];
    while (p) {
        if (!visitado[p->vertice])
            dfs2(gt, p->vertice);
        p = p->prox;
    }
}

// ─── Algoritmo de Kosaraju (1978) ────────────────────────────────────────────
// Componentes-Fortemente-Conexos(G):
//   1. Execute DFS(G) para obter f[v] para v ∈ V
//   2. Obter o grafo transposto GT
//   3. Execute DFS(GT) em ordem decrescente de f[v]
//   4. Devolva os conjuntos de vértices de cada árvore

void kosaraju(Grafo *g) {

    // --- Passo 1 ---
    memset(visitado, 0, sizeof(visitado));
    topo = 0;
    for (int u = 0; u < g->V; u++)
        if (!visitado[u])
            dfs1(g, u);

    // --- Passo 2 ---
    Grafo *gt = grafoTransposto(g);

    // --- Passo 3 ---
    memset(visitado, 0, sizeof(visitado));
    numSCC = 0;
    // percorre em ordem decrescente de f[v] (topo → base da pilha)
    for (int i = topo - 1; i >= 0; i--) {
        int v = ordem[i];
        if (!visitado[v]) {
            dfs2(gt, v);
            numSCC++;
        }
    }

    // --- Passo 4: exibe os SCCs ---
    printf("Numero de componentes fortemente conexos: %d\n\n", numSCC);
    for (int c = 0; c < numSCC; c++) {
        printf("SCC %d: { ", c + 1);
        for (int v = 0; v < g->V; v++)
            if (componente[v] == c)
                printf("%d ", v);
        printf("}\n");
    }
}

int main(void) {
    int V = 8;
    Grafo *g = criarGrafo(V);
    adicionarArco(g, 0, 1); // a → b
    adicionarArco(g, 1, 2); // b → c
    adicionarArco(g, 1, 4); // b → e
    adicionarArco(g, 1, 5); // b → f
    adicionarArco(g, 2, 3); // c → d
    adicionarArco(g, 2, 6); // c → g
    adicionarArco(g, 3, 2); // d → c
    adicionarArco(g, 3, 7); // d → h
    adicionarArco(g, 4, 0); // e → a
    adicionarArco(g, 4, 5); // e → f
    adicionarArco(g, 5, 6); // f → g
    adicionarArco(g, 6, 5); // g → f
    adicionarArco(g, 7, 3); // h → d
    adicionarArco(g, 7, 6); // h → g

    printf("=== Algoritmo de Kosaraju ===\n");
    printf("Vertices: a=0, b=1, c=2, d=3, e=4, f=5, g=6, h=7\n\n");
    kosaraju(g);
    
    return 0;
}