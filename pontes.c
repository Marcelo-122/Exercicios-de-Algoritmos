#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXV 100

// Lista de adjacências
int adj[MAXV][MAXV];
int grau[MAXV];
int n, m; // vértices e arestas

int pre[MAXV];
int low[MAXV];
int pai[MAXV];
int tempo;

int min(int a, int b) {
    return a < b ? a : b;
}

void dfs_pontes(int v) {
    tempo++;
    pre[v] = tempo;
    low[v] = pre[v];

    for (int i = 0; i < grau[v]; i++) {
        int w = adj[v][i];
        if (pre[w] == -1) {
            pai[w] = v;
            dfs_pontes(w);
            low[v] = min(low[v], low[w]);

            // Condição: low[w] == pre[w] => {v, w} é ponte
            if (low[w] == pre[w]) {
                printf("Ponte: {%d, %d}\n", v, w);
            }
        } else if (w != pai[v]) {
            low[v] = min(low[v], pre[w]);
        }
    }
}

void detectar_pontes() {
    tempo = 0;
    for (int v = 0; v < n; v++) {
        pre[v] = -1;
        pai[v] = -1;
    }
    for (int v = 0; v < n; v++) {
        if (pre[v] == -1) {
            pai[v] = v;
            dfs_pontes(v);
        }
    }
}

void dfs_articulacoes(int v) {
    tempo++;
    pre[v] = tempo;
    low[v] = pre[v];

    int filhos = 0;
    int eh_articulacao = 0;

    for (int i = 0; i < grau[v]; i++) {
        int w = adj[v][i];
        if (pre[w] == -1) {
            pai[w] = v;
            filhos++;
            dfs_articulacoes(w);
            low[v] = min(low[v], low[w]);

            // Condição para não-raiz: low[w] >= pre[v]
            if (low[w] >= pre[v]) {
                eh_articulacao = 1;
            }
        } else if (w != pai[v]) {
            low[v] = min(low[v], pre[w]);
        }
    }

    // Raiz da DFS: articulação se tiver >= 2 filhos
    // Não-raiz: articulação se low[w] >= pre[v] para algum filho w
    if ((pai[v] == v && filhos > 1) || (pai[v] != v && eh_articulacao)) {
        printf("Articulacao: %d\n", v);
    }
}

void detectar_articulacoes() {
    tempo = 0;
    for (int v = 0; v < n; v++) {
        pre[v] = -1;
        pai[v] = -1;
    }
    for (int v = 0; v < n; v++) {
        if (pre[v] == -1) {
            pai[v] = v;
            dfs_articulacoes(v);
        }
    }
}

void adicionar_aresta(int u, int v) {
    adj[u][grau[u]++] = v;
    adj[v][grau[v]++] = u;
}

int main() {
    n = 6; // a=0, b=1, c=2, e=3, f=4, g=5
    memset(grau, 0, sizeof(grau));

    // Ciclo a-b-c-a
    adicionar_aresta(0, 1); // a-b
    adicionar_aresta(1, 2); // b-c
    adicionar_aresta(2, 0); // c-a

    // Ponte b-e
    adicionar_aresta(1, 3); // b-e

    // Ciclo e-f-g-e
    adicionar_aresta(3, 4); // e-f
    adicionar_aresta(4, 5); // f-g
    adicionar_aresta(5, 3); // g-e

    printf("=== PONTES ===\n");
    detectar_pontes();

    printf("\n=== ARTICULACOES ===\n");
    detectar_articulacoes();

    return 0;
}