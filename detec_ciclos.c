/*
 * Detecção de Ciclos em Digrafos
 * Três abordagens implementadas:
 *   1. Usando função de caminho (ineficiente)
 *   2. Usando pilha de recursão
 *   3. Usando cores (BRANCO / CINZA / PRETO)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura do Grafo (lista de adjacências) */

typedef struct No {
    int dest;
    struct No *prox;
} No;

typedef struct {
    int V;       /* número de vértices */
    No **adj;    /* vetor de listas de adjacência */
} Grafo;

Grafo *criar_grafo(int V) {
    Grafo *g = malloc(sizeof(Grafo));
    g->V   = V;
    g->adj = calloc(V, sizeof(No *));
    return g;
}

void adicionar_arco(Grafo *g, int u, int v) {
    No *no  = malloc(sizeof(No));
    no->dest = v;
    no->prox = g->adj[u];
    g->adj[u] = no;
}

void liberar_grafo(Grafo *g) {
    for (int i = 0; i < g->V; i++) {
        No *cur = g->adj[i];
        while (cur) { No *tmp = cur->prox; free(cur); cur = tmp; }
    }
    free(g->adj);
    free(g);
}

static void visita_caminho(Grafo *g, int v, int *visitado) {
    visitado[v] = 1;
    for (No *w = g->adj[v]; w; w = w->prox)
        if (!visitado[w->dest])
            visita_caminho(g, w->dest, visitado);
}
static int tem_caminho(Grafo *g, int s, int t) {
    int *visitado = calloc(g->V, sizeof(int));
    visita_caminho(g, s, visitado);
    int resultado = visitado[t];
    free(visitado);
    return resultado;
}

int tem_ciclo_caminho(Grafo *g) {
    for (int v = 0; v < g->V; v++)
        for (No *w = g->adj[v]; w; w = w->prox)
            if (tem_caminho(g, w->dest, v))
                return 1;
    return 0;
}

static int tem_ciclo_visita_pilha(Grafo *g, int u,
                                  int *visitado, int *na_pilha) {
    visitado[u] = 1;
    na_pilha[u] = 1;

    for (No *w = g->adj[u]; w; w = w->prox) {
        if (!visitado[w->dest]) {
            if (tem_ciclo_visita_pilha(g, w->dest, visitado, na_pilha))
                return 1;
        } else if (na_pilha[w->dest]) {
            return 1;   /* arco de retorno encontrado */
        }
    }

    na_pilha[u] = 0;   /* remove vértice da pilha de recursão */
    return 0;
}

int tem_ciclo_pilha(Grafo *g) {
    int *visitado = calloc(g->V, sizeof(int));
    int *na_pilha = calloc(g->V, sizeof(int));

    int resultado = 0;
    for (int v = 0; v < g->V && !resultado; v++)
        if (!visitado[v])
            resultado = tem_ciclo_visita_pilha(g, v, visitado, na_pilha);

    free(visitado);
    free(na_pilha);
    return resultado;
}

#define BRANCO 0
#define CINZA  1
#define PRETO  2

static int tem_arco_de_retorno(Grafo *g, int v, int *cor) {
    cor[v] = CINZA;

    for (No *w = g->adj[v]; w; w = w->prox) {
        if (cor[w->dest] == CINZA)
            return 1;
        if (cor[w->dest] == BRANCO)
            if (tem_arco_de_retorno(g, w->dest, cor))
                return 1;
    }

    cor[v] = PRETO;
    return 0;
}

int tem_ciclo_cores(Grafo *g) {
    int *cor = malloc(g->V * sizeof(int));
    for (int i = 0; i < g->V; i++) cor[i] = BRANCO;

    int resultado = 0;
    for (int v = 0; v < g->V && !resultado; v++)
        if (cor[v] == BRANCO)
            resultado = tem_arco_de_retorno(g, v, cor);

    free(cor);
    return resultado;
}

static void testar(const char *nome, Grafo *g) {
    printf("  [Caminho] %s: %s\n", nome,
           tem_ciclo_caminho(g) ? "SIM" : "NÃO");
    printf("  [Pilha  ] %s: %s\n", nome,
           tem_ciclo_pilha(g)   ? "SIM" : "NÃO");
    printf("  [Cores  ] %s: %s\n\n", nome,
           tem_ciclo_cores(g)   ? "SIM" : "NÃO");
}

int main(void) {
    printf("=== Detecção de Ciclos em Digrafos ===\n\n");

    Grafo *g1 = criar_grafo(6);
    adicionar_arco(g1, 0, 2);
    adicionar_arco(g1, 0, 5);
    adicionar_arco(g1, 2, 1);
    adicionar_arco(g1, 1, 5);
    adicionar_arco(g1, 5, 3);
    adicionar_arco(g1, 3, 4);
    adicionar_arco(g1, 4, 2); 
    printf("Grafo 1 (COM ciclo: 2→1→5→3→4→2)\n");
    testar("grafo 1", g1);
    liberar_grafo(g1);

    Grafo *g2 = criar_grafo(6);
    adicionar_arco(g2, 0, 2);
    adicionar_arco(g2, 0, 5);
    adicionar_arco(g2, 2, 1);
    adicionar_arco(g2, 1, 5);
    adicionar_arco(g2, 5, 3);
    adicionar_arco(g2, 3, 4);
    printf("Grafo 2 (SEM ciclo)\n");
    testar("grafo 2", g2);
    liberar_grafo(g2);

    Grafo *g3 = criar_grafo(4);
    adicionar_arco(g3, 0, 1);
    adicionar_arco(g3, 1, 2);
    adicionar_arco(g3, 2, 0);   /* fecha o ciclo */
    adicionar_arco(g3, 2, 3);
    printf("Grafo 3 (COM ciclo: 0→1→2→0)\n");
    testar("grafo 3", g3);
    liberar_grafo(g3);

    /* --- Exemplo 4: DAG (sem ciclo) --- */
    Grafo *g4 = criar_grafo(4);
    adicionar_arco(g4, 0, 1);
    adicionar_arco(g4, 0, 2);
    adicionar_arco(g4, 1, 3);
    adicionar_arco(g4, 2, 3);
    printf("Grafo 4 – DAG (SEM ciclo)\n");
    testar("grafo 4", g4);
    liberar_grafo(g4);

    return 0;
}