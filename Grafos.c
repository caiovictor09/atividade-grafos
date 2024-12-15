#include <stdio.h>

#define MAX_VERTICES 100  // Limite máximo de vértices

void criarGrafoCompleto(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                grafo[i][j] = 1;  // Aresta presente
            } else {
                grafo[i][j] = 0;  // Sem laços
            }
        }
    }
}

void imprimirGrafo(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", grafo[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n;
    int grafo[MAX_VERTICES][MAX_VERTICES];  // Matriz de adjacência

    printf("Digite o número de vértices do grafo (máximo %d): ", MAX_VERTICES);
    scanf("%d", &n);

    if (n > MAX_VERTICES || n <= 0) {
        printf("Erro: número de vértices inválido.\n");
        return 1;
    }

    criarGrafoCompleto(grafo, n);
    imprimirGrafo(grafo, n);

    return 0;
}
