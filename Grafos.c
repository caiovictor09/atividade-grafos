#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_VERTICES 250  // Limite máximo de vértices
#define NUM_EXECUCOES 10  // Número de execuções para calcular o tempo médio

// Função para criar um grafo completo não direcionado com distâncias aleatórias entre 1 e 100
void criarGrafoCompleto(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {  // Apenas itera na metade superior
            if (i == j) {
                grafo[i][j] = 0;  // Sem laços
            } else {
                int distancia = rand() % 100 + 1;  // Distância entre 1 e 100
                grafo[i][j] = distancia;
                grafo[j][i] = distancia;  // Garantir simetria
            }
        }
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

// Heurística Construtiva: Método de Inserção Mais Barata
void heuristicaInsercaoMaisBarata(int percurso[MAX_VERTICES], int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    int visitado[MAX_VERTICES] = {0}; // Vetor para rastrear vértices visitados
    int numVerticesNoCiclo = 3;      // Começaremos com um ciclo inicial de 3 vértices

    // Inicializar o ciclo inicial com os três primeiros vértices
    percurso[0] = 0;
    percurso[1] = 1;
    percurso[2] = 2;
    visitado[0] = visitado[1] = visitado[2] = 1;

    // Preencher o restante dos vértices
    for (int k = 3; k < n; k++) {
        int melhorCusto = INT_MAX;
        int melhorPosicao = -1;
        int melhorVertice = -1;

        // Encontrar o vértice não visitado que minimiza o custo de inserção
        for (int v = 0; v < n; v++) {
            if (!visitado[v]) {
                for (int i = 0; i < numVerticesNoCiclo; i++) {
                    int j = (i + 1) % numVerticesNoCiclo; // Próximo vértice no ciclo
                    int custoAtual = grafo[percurso[i]][v] + grafo[v][percurso[j]] - grafo[percurso[i]][percurso[j]];
                    if (custoAtual < melhorCusto) {
                        melhorCusto = custoAtual;
                        melhorPosicao = j;
                        melhorVertice = v;
                    }
                }
            }
        }

        // Inserir o melhor vértice na melhor posição
        for (int i = numVerticesNoCiclo; i > melhorPosicao; i--) {
            percurso[i] = percurso[i - 1];
        }
        percurso[melhorPosicao] = melhorVertice;
        visitado[melhorVertice] = 1;
        numVerticesNoCiclo++;
    }
}

// Heurística de Refinamento: 2-opt
void heuristicaRefinamento2Opt(int percurso[MAX_VERTICES], int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    int melhorou = 1;
    while (melhorou) {
        melhorou = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                // Evitar a troca do mesmo segmento
                if (i == 0 && j == n - 1) continue;

                // Calcular o custo antes da troca
                int custoAntes = calcularCusto(percurso, grafo, n);

                // Inverter o segmento entre i e j
                for (int k = 0; k <= (j - i) / 2; k++) {
                    int temp = percurso[i + 1 + k];
                    percurso[i + 1 + k] = percurso[j - k];
                    percurso[j - k] = temp;
                }

                // Calcular o custo após a troca
                int custoDepois = calcularCusto(percurso, grafo, n);

                // Verificar se a troca resultou em melhoria
                if (custoDepois < custoAntes) {
                    melhorou = 1;  // Indica que houve melhoria
                } else {
                    // Reverter a troca se não houve melhoria
                    for (int k = 0; k <= (j - i) / 2; k++) {
                        int temp = percurso[i + 1 + k];
                        percurso[i + 1 + k] = percurso[j - k];
                        percurso[j - k] = temp;
                    }
                }
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

// Função para imprimir o grafo
void imprimirGrafo(int grafo[MAX_VERTICES][MAX_VERTICES], int n) {
    printf("Grafo (Matriz de adjacência):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", grafo[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n;
    int grafo[MAX_VERTICES][MAX_VERTICES];  // Matriz de adjacência
    int percurso[MAX_VERTICES];  // Vetor de percurso

    printf("Digite o numero de vertices do grafo (maximo %d): ", MAX_VERTICES);
    scanf("%d", &n);

    if (n > MAX_VERTICES || n <= 0) {
        printf("Erro: numero de vertices invalido.\n");
        return 1;
    }

    criarGrafoCompleto(grafo, n);

    while (1) {
        int opcao;
        printf("\nMenu:\n");
        printf("1. Mostrar execucao detalhada\n");
        printf("2. Mostrar tempo medio de 10 execucoes\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Execução detalhada
            heuristicaInsercaoMaisBarata(percurso, grafo, n);
            printf("\n--- Heuristica de Insercao Mais Barata ---\n");
            imprimirPercurso(percurso, n);
            int custoConstrutiva = calcularCusto(percurso, grafo, n);
            printf("Custo: %d\n", custoConstrutiva);

            heuristicaRefinamento2Opt(percurso, grafo, n);
            printf("\n--- Heuristica de Refinamento (2-opt) ---\n");
            imprimirPercurso(percurso, n);
            int custoRefinamento = calcularCusto(percurso, grafo, n);
            printf("Custo: %d\n", custoRefinamento);

            printf("\n--- Grafo Criado ---\n");
            imprimirGrafo(grafo, n);
        } else if (opcao == 2) {
            // Tempo médio de 10 execuções
            double tempoConstrutivaTotal = 0;
            for (int i = 0; i < NUM_EXECUCOES; i++) {
                clock_t inicioConstrutiva = clock();
                heuristicaInsercaoMaisBarata(percurso, grafo, n);
                clock_t fimConstrutiva = clock();

                tempoConstrutivaTotal += (double)(fimConstrutiva - inicioConstrutiva) / CLOCKS_PER_SEC;
            }
            double tempoConstrutivaMedio = tempoConstrutivaTotal / NUM_EXECUCOES;
            printf("Tempo medio da heuristica de insercao mais barata: %.6f segundos\n", tempoConstrutivaMedio);

            double tempoRefinamentoTotal = 0;
            for (int i = 0; i < NUM_EXECUCOES; i++) {
                clock_t inicioRefinamento = clock();
                heuristicaRefinamento2Opt(percurso, grafo, n);
                clock_t fimRefinamento = clock();

                tempoRefinamentoTotal += (double)(fimRefinamento - inicioRefinamento) / CLOCKS_PER_SEC;
            }
            double tempoRefinamentoMedio = tempoRefinamentoTotal / NUM_EXECUCOES;
            printf("Tempo medio da heuristica de refinamento (2-opt): %.6f segundos\n", tempoRefinamentoMedio);
        } else if (opcao == 3) {
            // Sair
            printf("Encerrando o programa.\n");
            break;
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
