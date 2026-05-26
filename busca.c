/* 
   Grafo representado por lista de adjacência.
   Exemplo:
     0 → [1, 4]
     1 → []
     2 → [0, 3, 4]
     3 → [4, 5]
     4 → [1, 5]
     5 → [1]
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_V 10   /* número máximo de vértices */

typedef struct No {
    int vertice;
    struct No *proximo;
} No;

typedef struct {
    int  num_vertices;
    No  *adj[MAX_V];   /* adj[u] = lista de vizinhos de u */
} Grafo;

int visitado[MAX_V];
int pai[MAX_V];
int d[MAX_V];    /* tempo de descoberta  */
int f[MAX_V];    /* tempo de finalização */
int tempo;

Grafo *cria_grafo(int num_vertices) {
    Grafo *G = malloc(sizeof(Grafo));
    G->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++)
        G->adj[i] = NULL;
    return G;
}

void adiciona_arco(Grafo *G, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->vertice = v;
    novo->proximo = G->adj[u];
    G->adj[u] = novo;
}

void DFS_visita(Grafo *G, int u) {
    visitado[u] = 1;
    tempo++;
    d[u] = tempo;
    printf("  Visitando vertice %d  (d=%d)\n", u, d[u]);

    for (No *w = G->adj[u]; w != NULL; w = w->proximo) {
        if (!visitado[w->vertice]) {
            pai[w->vertice] = u;
            DFS_visita(G, w->vertice);
        }
    }

    tempo++;
    f[u] = tempo;
    printf("  Finalizando vertice %d  (f=%d)\n", u, f[u]);
}

void DFS(Grafo *G) {
    /* inicializa todos os vertices */
    for (int u = 0; u < G->num_vertices; u++) {
        visitado[u] = 0;
        pai[u]      = -1;
        d[u]        = -1;
        f[u]        = -1;
    }
    tempo = 0;

    for (int u = 0; u < G->num_vertices; u++) {
        if (!visitado[u]) {
            pai[u] = u;   /* u e raiz da sua arborescencia */
            printf("\n--- Iniciando DFS-visita a partir do vertice %d ---\n", u);
            DFS_visita(G, u);
        }
    }
}

void mostra_caminho(int v) {
    /* guarda o caminho num vetor auxiliar */
    int caminho[MAX_V];
    int tam = 0;

    int u = v;
    while (pai[u] != u) {
        caminho[tam++] = u;
        u = pai[u];
    }
    caminho[tam++] = u;   /* adiciona a raiz */

    /* imprime na ordem correta (invertido) */
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void classifica_arco(int v, int w) {
    printf("  %d -> %d : ", v, w);

    if (d[v] < d[w] && d[w] < f[w] && f[w] < f[v]) {
        if (pai[w] == v)
            printf("Arborescencia\n");
        else
            printf("Descendente\n");
    } else if (d[w] < d[v] && d[v] < f[v] && f[v] < f[w]) {
        printf("Retorno\n");
    } else if (d[w] < f[w] && f[w] < d[v] && d[v] < f[v]) {
        printf("Cruzado\n");
    } else {
        printf("Desconhecido\n");
    }
}

int main(void) {
    Grafo *G = cria_grafo(6);

    adiciona_arco(G, 0, 1);
    adiciona_arco(G, 0, 4);
    adiciona_arco(G, 2, 0);
    adiciona_arco(G, 2, 3);
    adiciona_arco(G, 2, 4);
    adiciona_arco(G, 3, 4);
    adiciona_arco(G, 3, 5);
    adiciona_arco(G, 4, 1);
    adiciona_arco(G, 4, 5);
    adiciona_arco(G, 5, 1);

    printf("BUSCA EM PROFUNDIDADE (DFS)\n");
    DFS(G);
    printf("\nRESULTADO FINAL\n");

    printf("\nTempos por vertice:\n");
    printf("  %8s  %4s  %4s  %5s\n", "Vertice", "d", "f", "pai");
    printf("  ------------------------------\n");
    for (int v = 0; v < G->num_vertices; v++)
        printf("  %8d  %4d  %4d  %5d\n", v, d[v], f[v], pai[v]);

    printf("\nCaminhos da raiz ate cada vertice:\n");
    for (int v = 0; v < G->num_vertices; v++) {
        printf("  Ate %d: ", v);
        mostra_caminho(v);
    }

    printf("\nClassificacao dos arcos:\n");
    for (int v = 0; v < G->num_vertices; v++)
        for (No *w = G->adj[v]; w != NULL; w = w->proximo)
            classifica_arco(v, w->vertice);

    return 0;
}