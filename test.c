#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo o tamanho máximo dos vetores
#define MAX_TAM 1000

// Função de troca
void troca(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
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
    int meio = baixo + (alto - baixo) / 2;
    // Mediana de 3: baixo, meio, alto
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);
    if (arr[meio] > arr[alto]) troca(&arr[meio], &arr[alto]);
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);

    int pivo = arr[meio];
    troca(&arr[meio], &arr[alto]);

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
    // Mediana de 3: baixo, meio, alto
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);
    if (arr[meio] > arr[alto]) troca(&arr[meio], &arr[alto]);
    if (arr[baixo] > arr[meio]) troca(&arr[baixo], &arr[meio]);

    int pivo = arr[meio];
    troca(&arr[meio], &arr[alto]);

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
    int pivoIndice = baixo + rand() % (alto - baixo + 1);
    troca(&arr[alto], &arr[pivoIndice]);
    return particionaHP(arr, baixo, alto, trocas, chamadas);
}

void quicksortHA(int arr[], int baixo, int alto, int* trocas, int* chamadas) {
    (*chamadas)++;
    if (baixo < alto) {
        int pivo = particionaHA(arr, baixo, alto, trocas, chamadas);
        quicksortHA(arr, baixo, pivo - 1, trocas, chamadas);
        quicksortHA(arr, pivo + 1, alto, trocas, chamadas);
    }
}

// Função principal
int main(int argc, char* argv[]) {
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
        int arr[MAX_TAM];

        for (int j = 0; j < tamanho; j++) {
            fscanf(input, "%d", &arr[j]);
        }

        int trocasLP = 0, chamadasLP = 0;
        int trocasHP = 0, chamadasHP = 0;
        int trocasLM = 0, chamadasLM = 0;
        int trocasHM = 0, chamadasHM = 0;
        int trocasHA = 0, chamadasHA = 0;
        int trocasLA = 0, chamadasLA = 0;

        // Criando uma cópia inicial da lista
        int arrLP[MAX_TAM], arrHP[MAX_TAM], arrLM[MAX_TAM], arrHM[MAX_TAM], arrHA[MAX_TAM], arrLA[MAX_TAM];
        for (int j = 0; j < tamanho; j++) {
            arrLP[j] = arrHP[j] = arrLM[j] = arrHM[j] = arrHA[j] = arrLA[j] = arr[j];
        }

        // Escrevendo o estado da lista antes de cada ordenação no arquivo
        fprintf(output, "Lista antes do Lomuto Padrão: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrLP[j]);
        }
        fprintf(output, "\n");

        quicksortLP(arrLP, 0, tamanho - 1, &trocasLP, &chamadasLP);

        fprintf(output, "Lista antes do Hoare Padrão: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrHP[j]);
        }
        fprintf(output, "\n");

        quicksortHP(arrHP, 0, tamanho - 1, &trocasHP, &chamadasHP);

        fprintf(output, "Lista antes do Lomuto com Mediana de 3: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrLM[j]);
        }
        fprintf(output, "\n");

        quicksortLM(arrLM, 0, tamanho - 1, &trocasLM, &chamadasLM);

        fprintf(output, "Lista antes do Hoare com Mediana de 3: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrHM[j]);
        }
        fprintf(output, "\n");

        quicksortHM(arrHM, 0, tamanho - 1, &trocasHM, &chamadasHM);

        fprintf(output, "Lista antes do Hoare com Pivô Aleatório: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrHA[j]);
        }
        fprintf(output, "\n");

        quicksortHA(arrHA, 0, tamanho - 1, &trocasHA, &chamadasHA);

        fprintf(output, "Lista antes do Lomuto com Pivô Aleatório: ");
        for (int j = 0; j < tamanho; j++) {
            fprintf(output, "%d ", arrLA[j]);
        }
        fprintf(output, "\n");

        quicksortLA(arrLA, 0, tamanho - 1, &trocasLA, &chamadasLA);

        // Escrevendo o resumo das trocas e chamadas
        fprintf(output, "%d:N(%d),LP(%d:%d),HP(%d:%d),LM(%d:%d),HM(%d:%d),HA(%d:%d),LA(%d:%d)\n\n",
                i, tamanho,
                trocasLP + chamadasLP, trocasLP, // Soma de trocas e chamadas, e somente trocas para Lomuto Padrão
                trocasHP + chamadasHP, trocasHP, // Soma de trocas e chamadas, e somente trocas para Hoare Padrão
                trocasLM + chamadasLM, trocasLM, // Soma de trocas e chamadas, e somente trocas para Lomuto com Mediana de 3
                trocasHM + chamadasHM, trocasHM, // Soma de trocas e chamadas, e somente trocas para Hoare com Mediana de 3
                trocasHA + chamadasHA, trocasHA, // Soma de trocas e chamadas, e somente trocas para Hoare com Pivô Aleatório
                trocasLA + chamadasLA, trocasLA  // Soma de trocas e chamadas, e somente trocas para Lomuto com Pivô Aleatório
        );
    }

    // Fechando arquivos
    fclose(input);
    fclose(output);

    return 0;
}
