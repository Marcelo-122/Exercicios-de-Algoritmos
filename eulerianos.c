#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 10

typedef struct {
    int adj[MAX_V][MAX_V];
    int V; /* número de vértices */
    int E; /* número de arestas  */
} Grafo;

void inicializa(Grafo *G, int V) {
    G->V = V;
    G->E = 0;
    memset(G->adj, 0, sizeof(G->adj));
}

void adiciona_aresta(Grafo *G, int u, int v) {
    if (!G->adj[u][v]) {
        G->adj[u][v]++;
        G->adj[v][u]++;
        G->E++;
    }
}

void remove_aresta(Grafo *G, int u, int v) {
    G->adj[u][v]--;
    G->adj[v][u]--;
    G->E--;
}

int grau(Grafo *G, int v) {
    int g = 0;
    for (int i = 0; i < G->V; i++)
        g += G->adj[v][i];
    return g;
}

void verifica_euleriano(Grafo *G) {
    int impares = 0;
    for (int v = 0; v < G->V; v++)
        if (grau(G, v) % 2 != 0)
            impares++;

    printf("=== Verificação Euleriana ===\n");
    if (impares == 0)
        printf("O grafo É EULERIANO (trilha fechada existe).\n");
    else if (impares == 2)
        printf("O grafo NÃO é euleriano, mas possui TRILHA EULERIANA.\n");
    else
        printf("O grafo NÃO possui trilha euleriana (%d vértices de grau ímpar).\n", impares);
    printf("\n");
}

int eh_conexo(Grafo *G) {
    int cor[MAX_V];
    int pilha[MAX_V * MAX_V], topo = -1;
    int contador = 0;

    /* Escolhe um vértice com pelo menos uma aresta */
    int inicio = -1;
    for (int v = 0; v < G->V; v++)
        if (grau(G, v) > 0) { inicio = v; break; }
    if (inicio == -1) return 1; /* grafo sem arestas – trivialmente conexo */

    memset(cor, 0, sizeof(cor)); /* 0 = BRANCO */

    pilha[++topo] = inicio;
    cor[inicio] = 1; /* CINZA */
    contador = 1;

    while (topo >= 0) {
        int v = pilha[topo--];
        for (int w = 0; w < G->V; w++) {
            if (G->adj[v][w] && cor[w] == 0) {
                pilha[++topo] = w;
                cor[w] = 1;
                contador++;
            }
        }
        cor[v] = 2; /* PRETO */
    }

    /* Conta apenas vértices com arestas */
    int com_arestas = 0;
    for (int v = 0; v < G->V; v++)
        if (grau(G, v) > 0) com_arestas++;

    return contador >= com_arestas;
}

int eh_ponte(Grafo *G, int u, int v) {
    /* Caso única aresta incidente em u: não é ponte */
    if (grau(G, u) == 1)
        return 0;

    remove_aresta(G, u, v);
    int conexo = eh_conexo(G);
    adiciona_aresta(G, u, v);

    return !conexo; /* se desconectou, é ponte */
}

void fleury_recursivo(Grafo *G, int v) {
    printf("%d", v);
    for (int w = 0; w < G->V; w++) {
        if (G->adj[v][w] > 0) {
            /* Prefere aresta que não seja ponte */
            if (!eh_ponte(G, v, w)) {
                printf(" -> ");
                remove_aresta(G, v, w);
                fleury_recursivo(G, w);
                return;
            }
        }
    }
    /* Se só há pontes, usa a única disponível */
    for (int w = 0; w < G->V; w++) {
        if (G->adj[v][w] > 0) {
            printf(" -> ");
            remove_aresta(G, v, w);
            fleury_recursivo(G, w);
            return;
        }
    }
}

void fleury(Grafo *G) {
    /* Cópia para não destruir o grafo original */
    Grafo copia = *G;

    printf("=== Algoritmo de Fleury ===\n");

    /* Conta vértices de grau ímpar */
    int impares[MAX_V], n_imp = 0;
    for (int v = 0; v < copia.V; v++)
        if (grau(&copia, v) % 2 != 0)
            impares[n_imp++] = v;

    if (n_imp > 2) {
        printf("Não existe trilha de Euler.\n\n");
        return;
    }

    /* Escolhe vértice inicial (slide 53) */
    int v0 = (n_imp == 0) ? 0 : impares[0];

    printf("Trilha: ");
    fleury_recursivo(&copia, v0);
    printf("\n\n");
}

void hierholzer(Grafo *G) {
    Grafo copia = *G;

    printf("=== Algoritmo de Hierholzer ===\n");

    int pilha[MAX_V * MAX_V], topo = -1;
    int trilha[MAX_V * MAX_V], t_tam = 0;

    /* Vértice inicial: qualquer vértice com grau > 0 */
    int inicio = 0;
    for (int v = 0; v < copia.V; v++)
        if (grau(&copia, v) > 0) { inicio = v; break; }

    pilha[++topo] = inicio;

    while (topo >= 0) {
        int u = pilha[topo]; /* topo sem desempilhar */
        int achou = 0;

        for (int w = 0; w < copia.V; w++) {
            if (copia.adj[u][w] > 0) {
                /* Marca a aresta removendo-a */
                remove_aresta(&copia, u, w);
                pilha[++topo] = w;
                achou = 1;
                break;
            }
        }

        /* Se u não tem mais arestas, vai para a trilha */
        if (!achou) {
            trilha[t_tam++] = pilha[topo--];
        }
    }

    printf("Trilha: ");
    for (int i = 0; i < t_tam; i++) {
        printf("%d", trilha[i]);
        if (i < t_tam - 1) printf(" -> ");
    }
    printf("\n\n");
}

int main(void) {
    printf("========================================\n");
    printf(" EXEMPLO 1 – Grafo com trilha euleriana\n");
    printf("========================================\n\n");

    Grafo G1;
    inicializa(&G1, 5);
    adiciona_aresta(&G1, 0, 1); /* a */
    adiciona_aresta(&G1, 1, 2); /* b */
    adiciona_aresta(&G1, 2, 3); /* c */
    adiciona_aresta(&G1, 3, 0); /* d */
    adiciona_aresta(&G1, 0, 2); /* e */
    adiciona_aresta(&G1, 2, 4); /* f */
    adiciona_aresta(&G1, 4, 3); /* g */
    adiciona_aresta(&G1, 3, 1); /* h */

    verifica_euleriano(&G1);
    fleury(&G1);
    hierholzer(&G1);

    printf("========================================\n");
    printf(" EXEMPLO 2 – Grafo Euleriano\n");
    printf("========================================\n\n");

    Grafo G2;
    inicializa(&G2, 7);
    adiciona_aresta(&G2, 0, 1); /* a */
    adiciona_aresta(&G2, 1, 2); /* b */
    adiciona_aresta(&G2, 0, 2); /* c */
    adiciona_aresta(&G2, 1, 3); /* d */
    adiciona_aresta(&G2, 2, 4); /* f (nome do slide) */
    adiciona_aresta(&G2, 3, 4); /* e */
    adiciona_aresta(&G2, 3, 5); /* h */
    adiciona_aresta(&G2, 4, 5); /* g */
    adiciona_aresta(&G2, 1, 4); /* f2 */

    verifica_euleriano(&G2);
    fleury(&G2);
    hierholzer(&G2);

    return 0;
}