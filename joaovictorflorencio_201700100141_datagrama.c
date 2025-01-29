#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numPacote;
    int tamanho;
    int* dados;
} Pacote;

void heapify(Pacote pacotes[], int n, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < n && pacotes[esquerda].numPacote > pacotes[maior].numPacote)
        maior = esquerda;

    if (direita < n && pacotes[direita].numPacote > pacotes[maior].numPacote)
        maior = direita;

    if (maior != i) {
        Pacote temp = pacotes[i];
        pacotes[i] = pacotes[maior];
        pacotes[maior] = temp;
        heapify(pacotes, n, maior);
    }
}

void heapsort(Pacote pacotes[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(pacotes, n, i);

    for (int i = n - 1; i > 0; i--) {
        Pacote temp = pacotes[0];
        pacotes[0] = pacotes[i];
        pacotes[i] = temp;
        heapify(pacotes, i, 0);
    }
}

void merge_sorted(Pacote** buffer, int* buffer_size, Pacote batch[], int batch_size) {
    Pacote* merged = malloc((*buffer_size + batch_size) * sizeof(Pacote));
    int i = 0, j = 0, k = 0;

    while (i < *buffer_size && j < batch_size) {
        if ((*buffer)[i].numPacote <= batch[j].numPacote)
            merged[k++] = (*buffer)[i++];
        else
            merged[k++] = batch[j++];
    }

    while (i < *buffer_size) merged[k++] = (*buffer)[i++];
    while (j < batch_size) merged[k++] = batch[j++];

    free(*buffer);
    *buffer = merged;
    *buffer_size += batch_size;
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

    int totalPacotes, pacotesPorLeitura;
    if (fscanf(arquivoEntrada, "%d %d", &totalPacotes, &pacotesPorLeitura) != 2) {
        perror("Erro ao ler cabeçalho");
        return EXIT_FAILURE;
    }

    Pacote* pacotes = malloc(totalPacotes * sizeof(Pacote));
    for (int i = 0; i < totalPacotes; i++) {
        if (fscanf(arquivoEntrada, "%d %d", &pacotes[i].numPacote, &pacotes[i].tamanho) != 2) {
            perror("Erro ao ler pacote");
            return EXIT_FAILURE;
        }
        pacotes[i].dados = malloc(pacotes[i].tamanho * sizeof(int));
        for (int j = 0; j < pacotes[i].tamanho; j++) {
            if (fscanf(arquivoEntrada, "%X", &pacotes[i].dados[j]) != 1) {
                perror("Erro ao ler dados");
                return EXIT_FAILURE;
            }
        }
    }

    Pacote* buffer = NULL;
    int buffer_size = 0;
    int next_expected = 0;

    for (int i = 0; i < totalPacotes; i += pacotesPorLeitura) {
        int batch_size = (i + pacotesPorLeitura <= totalPacotes) 
                        ? pacotesPorLeitura 
                        : totalPacotes - i;

        heapsort(pacotes + i, batch_size);
        merge_sorted(&buffer, &buffer_size, pacotes + i, batch_size);

        int output_count = 0;
        while (output_count < buffer_size && 
               buffer[output_count].numPacote == next_expected + output_count) {
            output_count++;
        }

        if (output_count > 0) {
            fprintf(arquivoSaida, "|");
            for (int k = 0; k < output_count; k++) {
                Pacote p = buffer[k];
                for (int j = 0; j < p.tamanho; j++) {
                    fprintf(arquivoSaida, "%02X", p.dados[j]);
                    if (j < p.tamanho - 1) fprintf(arquivoSaida, ",");
                }
                if (k < output_count - 1) fprintf(arquivoSaida, "|");
            }
            fprintf(arquivoSaida, i + pacotesPorLeitura < totalPacotes ? "|\n" : "|");

            buffer_size -= output_count;
            memmove(buffer, buffer + output_count, buffer_size * sizeof(Pacote));
            buffer = realloc(buffer, buffer_size * sizeof(Pacote));
            next_expected += output_count;
        }
    }

    free(buffer);
    for (int i = 0; i < totalPacotes; i++) free(pacotes[i].dados);
    free(pacotes);

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    return EXIT_SUCCESS;
}