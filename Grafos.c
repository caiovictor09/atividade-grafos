#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_VERTICES 100  // Limite máximo de vértices

// Função para criar um grafo completo com distâncias aleatórias entre 1 e 100
void criarGrafoCompleto(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                grafo[i][j] = rand() % 100 + 1;  // Distância entre 1 e 100
            } else {
                grafo[i][j] = 0;  // Sem laços
            }
        }
    }
}

// Função para imprimir o grafo
void imprimirGrafo(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    printf("Matriz de Adjacência (distâncias):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", grafo[i][j]);
        }
        printf("\n");
    }
}

// Função para calcular o custo total de um percurso
int calcularCusto(int percurso[MAX_VERTICES], int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    int custo = 0;
    for (int i = 0; i < n - 1; i++) {
        custo += grafo[percurso[i]][percurso[i + 1]];
    }
    custo += grafo[percurso[n - 1]][percurso[0]];  // Retorno ao ponto de partida
    return custo;
}

// Heurística Construtiva: Gera uma solução inicial simples (sequência sequencial)
void heuristicaConstrutiva(int percurso[MAX_VERTICES], int n) {
    for (int i = 0; i < n; i++) {
        percurso[i] = i;
    }
}

// Heurística de Refinamento: Aplica Hill Climbing para melhorar o percurso
void heuristicaRefinamento(int percurso[MAX_VERTICES], int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    int melhorou = 1;
    while (melhorou) {
        melhorou = 0;
        for (int i = 0; i < n - 1; i++) {
            // Calcula o custo antes e depois de trocar dois vértices adjacentes
            int custoAtual = grafo[percurso[i]][percurso[i + 1]] + grafo[percurso[i + 1]][percurso[(i + 2) % n]];
            int custoNovo = grafo[percurso[i]][percurso[(i + 2) % n]] + grafo[percurso[i + 1]][percurso[i]];

            if (custoNovo < custoAtual) {
                // Troca os vértices adjacentes
                int temp = percurso[i + 1];
                percurso[i + 1] = percurso[(i + 2) % n];
                percurso[(i + 2) % n] = temp;
                melhorou = 1; // Indica que houve uma melhoria
            }
        }
    }
}

// Função para imprimir o percurso
void imprimirPercurso(int percurso[MAX_VERTICES], int n) {
    printf("Percurso: ");
    for (int i = 0; i < n; i++) {
        printf("%d -> ", percurso[i]);
    }
    printf("%d\n", percurso[0]);  // Retorno ao início
}

int main() {
    int n;
    int grafo[MAX_VERTICES][MAX_VERTICES];  // Matriz de adjacência
    int percurso[MAX_VERTICES];  // Vetor de percurso

    printf("Digite o número de vértices do grafo (máximo %d): ", MAX_VERTICES);
    scanf("%d", &n);

    if (n > MAX_VERTICES || n <= 0) {
        printf("Erro: número de vértices inválido.\n");
        return 1;
    }

    criarGrafoCompleto(grafo, n);
    imprimirGrafo(grafo, n);

    // Heurística Construtiva
    clock_t inicioConstrutiva = clock();
    heuristicaConstrutiva(percurso, n);
    clock_t fimConstrutiva = clock();

    printf("\nPercurso inicial gerado pela heurística construtiva:\n");
    imprimirPercurso(percurso, n);
    int custoInicial = calcularCusto(percurso, grafo, n);
    printf("Custo inicial: %d\n", custoInicial);
    double tempoConstrutiva = (double)(fimConstrutiva - inicioConstrutiva) / CLOCKS_PER_SEC;
    printf("Tempo da heurística construtiva: %.6f segundos\n", tempoConstrutiva);

    // Heurística de Refinamento
    clock_t inicioRefinamento = clock();
    heuristicaRefinamento(percurso, grafo, n);
    clock_t fimRefinamento = clock();

    printf("\nPercurso refinado pela heurística de refinamento:\n");
    imprimirPercurso(percurso, n);
    int custoRefinado = calcularCusto(percurso, grafo, n);
    printf("Custo refinado: %d\n", custoRefinado);
    double tempoRefinamento = (double)(fimRefinamento - inicioRefinamento) / CLOCKS_PER_SEC;
    printf("Tempo da heurística de refinamento: %.6f segundos\n", tempoRefinamento);

    return 0;
}
