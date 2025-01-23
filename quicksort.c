#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funções para as variantes de Quicksort

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Lomuto Padrão
int lomuto(int arr[], int low, int high, int* numTrocas, int* numChamadas) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        (*numChamadas)++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            (*numTrocas)++;
        }
    }
    swap(&arr[i + 1], &arr[high]);
    (*numTrocas)++;
    return i + 1;
}

// Lomuto com Mediana de 3
int lomutoMediana3(int arr[], int low, int high, int* numTrocas, int* numChamadas) {
    int mid = low + (high - low) / 2;
    // Implementar a lógica da mediana de 3 para definir o pivô
    return lomuto(arr, low, high, numTrocas, numChamadas);
}

// Função recursiva do Quicksort
void quicksort(int arr[], int low, int high, int* numTrocas, int* numChamadas, int variant) {
    if (low < high) {
        int pi;
        // Verifica a variante do Quicksort
        if (variant == 0) {  // Lomuto padrão
            pi = lomuto(arr, low, high, numTrocas, numChamadas);
        } else if (variant == 1) {  // Lomuto com Mediana de 3
            pi = lomutoMediana3(arr, low, high, numTrocas, numChamadas);
        } else {
            // Implementar outras variantes (como Hoare)
        }

        // Recursão
        quicksort(arr, low, pi - 1, numTrocas, numChamadas, variant); // Lado esquerdo
        quicksort(arr, pi + 1, high, numTrocas, numChamadas, variant); // Lado direito
    }
}

// Função para processar cada vetor
void processarVetor(int* arr, int tamanho, FILE* output) {
    int numTrocas, numChamadas;

    fprintf(output, "N(%d),", tamanho);

    // Lomuto padrão (LP)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 0);
    fprintf(output, "LP(%d),", numTrocas);

    // Hoare Padrão (HP)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 1);
    fprintf(output, "HP(%d),", numTrocas);

    // Lomuto mediana de 3 (LM)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 2);
    fprintf(output, "LM(%d),", numTrocas);

    // Hoare mediana de 3 (HM)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 3);
    fprintf(output, "HM(%d),", numTrocas);

    // Hoare Aleatório (HA)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 4);
    fprintf(output, "HA(%d),", numTrocas);

    // Lomuto Aleatório (LA)
    numTrocas = numChamadas = 0;
    quicksort(arr, 0, tamanho - 1, &numTrocas, &numChamadas, 5);
    fprintf(output, "LA(%d)\n", numTrocas);
}

int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if (input == NULL || output == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    int n;
    fscanf(input, "%d", &n);  // Número total de vetores
    for (int i = 0; i < n; i++) {
        int tamanho;
        fscanf(input, "%d", &tamanho);
        int* arr = (int*)malloc(tamanho * sizeof(int));

        for (int j = 0; j < tamanho; j++) {
            fscanf(input, "%d", &arr[j]);
        }

        processarVetor(arr, tamanho, output);
        free(arr);
    }

    fclose(input);
    fclose(output);
    return 0;
}
