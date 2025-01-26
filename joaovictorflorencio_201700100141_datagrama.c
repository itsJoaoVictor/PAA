#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACKETS 1000
#define MAX_DATA_SIZE 512

typedef struct {
    int numero;
    int tamanho;
    unsigned char dados[MAX_DATA_SIZE];
} Pacote;

void swap(Pacote *a, Pacote *b) {
    Pacote temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Pacote arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].numero > arr[largest].numero)
        largest = left;

    if (right < n && arr[right].numero > arr[largest].numero)
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapsort(Pacote arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* arquivoEntrada = fopen(argv[1], "r");
    FILE* arquivoSaida = fopen(argv[2], "w");

    if (!arquivoEntrada || !arquivoSaida) {
        perror("Erro ao abrir arquivo");
        return EXIT_FAILURE;
    }

    int totalPacotes, quantidadePacotes;
    fscanf(arquivoEntrada, "%d %d", &totalPacotes, &quantidadePacotes);

    Pacote pacotes[MAX_PACKETS];
    int numPacotes = 0;

    while (fscanf(arquivoEntrada, "%d %d", &pacotes[numPacotes].numero, &pacotes[numPacotes].tamanho) != EOF) {
        for (int i = 0; i < pacotes[numPacotes].tamanho; i++) {
            fscanf(arquivoEntrada, "%hhx", &pacotes[numPacotes].dados[i]);
        }
        numPacotes++;
    }

    heapsort(pacotes, numPacotes);

    for (int i = 0; i < numPacotes; i++) {
        fprintf(arquivoSaida, "|");
        for (int j = 0; j < pacotes[i].tamanho; j++) {
            fprintf(arquivoSaida, "%02X", pacotes[i].dados[j]);
            if (j < pacotes[i].tamanho - 1) {
                fprintf(arquivoSaida, ",");
            }
        }
        fprintf(arquivoSaida, "|");

        // Adiciona uma quebra de linha após cada bloco de 'quantidadePacotes'
        if ((i + 1) % quantidadePacotes == 0 && (i + 1) != numPacotes) {
            fprintf(arquivoSaida, "\n");
        }
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    return EXIT_SUCCESS;
}