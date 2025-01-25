#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definindo o tamanho máximo dos vetores
#define MAX_TAM 1000
int arrOriginal[MAX_TAM]; // Lista original

// Estrutura para armazenar os resultados de cada técnica
typedef struct {
    char nome[50]; // Nome da técnica
    int trocasChamadas; // Soma de trocas + chamadas
    int trocas; // Número de trocas
    int chamadas; // Número de chamadas
} Resultado;

// Função de troca
void troca(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para restaurar a lista original
void restaurarLista(int arr[], int tamanho) {
    memcpy(arr, arrOriginal, sizeof(int) * tamanho);
}

// Função Lomuto Padrão
int particionaLP(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    int pivo = arr[alto];
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (arr[j] <= pivo) {
            i++;
            troca(&arr[i], &arr[j]);
            (*trocas)++;
        }
    }
    troca(&arr[i + 1], &arr[alto]);
    (*trocas)++;
    return i + 1;
}

void quicksortLP(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaLP(arr, baixo, alto, trocas, chamadas);
        quicksortLP(arr, baixo, pivo - 1, trocas, chamadas);
        quicksortLP(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função Lomuto por Mediana de 3
int particionaLM(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    int n = alto - baixo + 1;
    
    // Definir os índices V1, V2 e V3
    int V1 = baixo + n / 4;
    int V2 = baixo + n / 2;
    int V3 = baixo + 3 * n / 4;
    
    // Ordenar V1, V2, V3
    if (arr[V1] > arr[V2]) troca(&arr[V1], &arr[V2]);
    if (arr[V2] > arr[V3]) troca(&arr[V2], &arr[V3]);
    if (arr[V1] > arr[V2]) troca(&arr[V1], &arr[V2]);
    
    // Agora, a mediana está em V2, e será o pivô
    int pivo = arr[V2];
    
    // Troca o pivô com o último elemento
    troca(&arr[V2], &arr[alto]);
    (*trocas)++;
    
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (arr[j] <= pivo) {
            i++;
            troca(&arr[i], &arr[j]);
            (*trocas)++;
        }
    }
    troca(&arr[i + 1], &arr[alto]);
    (*trocas)++;
    
    return i + 1;
}


void quicksortLM(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaLM(arr, baixo, alto, trocas, chamadas);
        quicksortLM(arr, baixo, pivo - 1, trocas, chamadas);
        quicksortLM(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função Lomuto por Pivô Aleatório
int particionaLA(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    int pivoIndice = baixo + rand() % (alto - baixo + 1);
    troca(&arr[alto], &arr[pivoIndice]);
    return particionaLP(arr, baixo, alto, trocas, chamadas);
}

void quicksortLA(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaLA(arr, baixo, alto, trocas, chamadas);
        quicksortLA(arr, baixo, pivo - 1, trocas, chamadas);
        quicksortLA(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função Hoare Padrão
int particionaHP(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    int pivo = arr[baixo];
    int i = baixo - 1;
    int j = alto + 1;
    while (1) {
        do { i++; } while (arr[i] < pivo);
        do { j--; } while (arr[j] > pivo);
        if (i >= j) return j;
        troca(&arr[i], &arr[j]);
        (*trocas)++;
    }
}

void quicksortHP(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaHP(arr, baixo, alto, trocas, chamadas);
        quicksortHP(arr, baixo, pivo, trocas, chamadas);
        quicksortHP(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função Hoare por Mediana de 3
int particionaHM(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    int meio = baixo + (alto - baixo) / 2;

    // Ordenar baixo, meio e alto
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);
    if (arr[meio] > arr[alto]) troca(&arr[meio], &arr[alto]);
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);

    // Usar a mediana como pivô
    int pivo = arr[meio];

    int i = baixo - 1;
    int j = alto + 1;
    while (1) {
        do { i++; } while (arr[i] < pivo);
        do { j--; } while (arr[j] > pivo);
        if (i >= j) return j;
        troca(&arr[i], &arr[j]);
        (*trocas)++;
    }
}

void quicksortHM(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaHM(arr, baixo, alto, trocas, chamadas);
        quicksortHM(arr, baixo, pivo - 1, trocas, chamadas);
        quicksortHM(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função Hoare por Pivô Aleatório
int particionaHA(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    // Escolhe um índice aleatório para o pivô
    int pivoIndice = baixo + rand() % (alto - baixo + 1);
    // Troca o pivô aleatório com o último elemento
    troca(&arr[alto], &arr[pivoIndice]);
    (*trocas)++;  // Contabilizando a troca

    // Agora a partição pode ser feita normalmente
    return particionaHP(arr, baixo, alto, trocas, chamadas);
}

void quicksortHA(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        // Chama a partição
        int pivo = particionaHA(arr, baixo, alto, trocas, chamadas);
        // Recursão nos sub-arrays
        quicksortHA(arr, baixo, pivo, trocas, chamadas);
        quicksortHA(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função para ordenação de técnicas
int comparaResultados(const void* a, const void* b) {
    return ((Resultado*)a)->trocasChamadas - ((Resultado*)b)->trocasChamadas;
}

// Função principal
int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    // Abrindo arquivos
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    // Verificando se os arquivos foram abertos corretamente
    if (!input || !output) {
        fprintf(stderr, "Erro ao abrir arquivos\n");
        return 1;
    }

    int n;
    fscanf(input, "%d", &n);

    for (int i = 0; i < n; i++) {
        int tamanho;
        fscanf(input, "%d", &tamanho);
        for (int j = 0; j < tamanho; j++) {
            fscanf(input, "%d", &arrOriginal[j]); // Carregar a lista original
        }

        int arr[MAX_TAM]; // Lista temporária para cada método

        // Inicializa os resultados
        Resultado resultados[6];

        // Lomuto Padrão
        restaurarLista(arr, tamanho);
        int trocasLP = 0, chamadasLP = 0;
        quicksortLP(arr, 0, tamanho - 1, &trocasLP, &chamadasLP);
        resultados[0] = (Resultado){"LP", trocasLP + chamadasLP, trocasLP, chamadasLP};

        // Hoare Padrão
        restaurarLista(arr, tamanho);
        int trocasHP = 0, chamadasHP = 0;
        quicksortHP(arr, 0, tamanho - 1, &trocasHP, &chamadasHP);
        resultados[1] = (Resultado){"HP", trocasHP + chamadasHP, trocasHP, chamadasHP};

        // Lomuto com Mediana de 3
        restaurarLista(arr, tamanho);
        int trocasLM = 0, chamadasLM = 0;
        quicksortLM(arr, 0, tamanho - 1, &trocasLM, &chamadasLM);
        resultados[2] = (Resultado){"LM", trocasLM + chamadasLM, trocasLM, chamadasLM};

        // Hoare com Mediana de 3
        restaurarLista(arr, tamanho);
        int trocasHM = 0, chamadasHM = 0;
        quicksortHM(arr, 0, tamanho - 1, &trocasHM, &chamadasHM);
        resultados[3] = (Resultado){"HM", trocasHM + chamadasHM, trocasHM, chamadasHM};

        // Hoare com Pivô Aleatório
        restaurarLista(arr, tamanho);
        int trocasHA = 0, chamadasHA = 0;
        quicksortHA(arr, 0, tamanho - 1, &trocasHA, &chamadasHA);
        resultados[4] = (Resultado){"HA", trocasHA + chamadasHA, trocasHA, chamadasHA};

        // Lomuto com Pivô Aleatório
        restaurarLista(arr, tamanho);
        int trocasLA = 0, chamadasLA = 0;
        quicksortLA(arr, 0, tamanho - 1, &trocasLA, &chamadasLA);
        resultados[5] = (Resultado){"LA", trocasLA + chamadasLA, trocasLA, chamadasLA};

        // Ordena os resultados pela soma de trocas + chamadas
        qsort(resultados, 6, sizeof(Resultado), comparaResultados);

        // Escrevendo a saída no formato correto
        fprintf(output, "%d:N(%d)", i, tamanho);
        for (int j = 0; j < 6; j++) {
            fprintf(output, ",%s(%d)", resultados[j].nome, resultados[j].trocasChamadas);
        }
        fprintf(output, "\n");
    }

    // Fechando arquivos
    fclose(input);
    fclose(output);

    return 0;
}
