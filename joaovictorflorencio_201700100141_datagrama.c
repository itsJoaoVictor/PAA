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

    if (esquerda < n && pacotes[esquerda].numPacote > pacotes[maior].numPacote) {
        maior = esquerda;
    }

    if (direita < n && pacotes[direita].numPacote > pacotes[maior].numPacote) {
        maior = direita;
    }

    if (maior != i) {
        Pacote temp = pacotes[i];
        pacotes[i] = pacotes[maior];
        pacotes[maior] = temp;

        heapify(pacotes, n, maior);
    }
}

void heapsort(Pacote pacotes[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(pacotes, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        Pacote temp = pacotes[0];
        pacotes[0] = pacotes[i];
        pacotes[i] = temp;

        heapify(pacotes, i, 0);
    }
}

void lerPacotes(FILE* arquivoEntrada, Pacote pacotes[], int* totalPacotes, int* pacotesPorLeitura) {
    fscanf(arquivoEntrada, "%d %d", totalPacotes, pacotesPorLeitura);

    for (int i = 0; i < *totalPacotes; i++) {
        fscanf(arquivoEntrada, "%d %d", &pacotes[i].numPacote, &pacotes[i].tamanho);
        pacotes[i].dados = (int*)malloc(pacotes[i].tamanho * sizeof(int));

        for (int j = 0; j < pacotes[i].tamanho; j++) {
            fscanf(arquivoEntrada, "%X", &pacotes[i].dados[j]);
        }
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

    int totalPacotes, pacotesPorLeitura;
    Pacote* pacotes = (Pacote*)malloc(512 * sizeof(Pacote));

    lerPacotes(arquivoEntrada, pacotes, &totalPacotes, &pacotesPorLeitura);

    int next_expected = 0;
    Pacote* buffer = NULL;
    int buffer_size = 0;

    for (int i = 0; i < totalPacotes; i += pacotesPorLeitura) {
        int batch_size = pacotesPorLeitura;
        if (i + batch_size > totalPacotes) {
            batch_size = totalPacotes - i;
        }

        // Adiciona o lote atual ao buffer
        for (int j = 0; j < batch_size; j++) {
            Pacote p = pacotes[i + j];
            buffer = realloc(buffer, (buffer_size + 1) * sizeof(Pacote));
            buffer[buffer_size] = p;
            buffer_size++;
        }

        // Ordena o buffer pelo número do pacote
        heapsort(buffer, buffer_size);

        // Encontra pacotes contíguos a partir do next_expected
        int current_expected = next_expected;
        int output_count = 0;
        for (int k = 0; k < buffer_size; k++) {
            if (buffer[k].numPacote == current_expected) {
                output_count++;
                current_expected++;
            } else if (buffer[k].numPacote > current_expected) {
                break;
            }
        }

        if (output_count > 0) {
            // Escreve os pacotes de saída
            fprintf(arquivoSaida, "|");
            for (int k = 0; k < output_count; k++) {
                Pacote p = buffer[k];
                for (int j = 0; j < p.tamanho; j++) {
                    fprintf(arquivoSaida, "%02X", p.dados[j]);
                    if (j < p.tamanho - 1) {
                        fprintf(arquivoSaida, ",");
                    }
                }
                if (k < output_count - 1) {
                    fprintf(arquivoSaida, "|");
                }
            }
            if (i + pacotesPorLeitura < totalPacotes)
            {
                fprintf(arquivoSaida, "|\n");
            }
            else
            {
                fprintf(arquivoSaida, "|");
            }

            // Remove os pacotes processados do buffer
            int new_buffer_size = buffer_size - output_count;
            for (int k = 0; k < new_buffer_size; k++) {
                buffer[k] = buffer[k + output_count];
            }
            buffer_size = new_buffer_size;
            buffer = realloc(buffer, new_buffer_size * sizeof(Pacote));

            // Atualiza o próximo número esperado
            next_expected = current_expected;
        }
    }

    free(buffer);

    // Libera a memória alocada para os dados dos pacotes
    for (int i = 0; i < totalPacotes; i++) {
        free(pacotes[i].dados);
    }
    free(pacotes);

    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    return EXIT_SUCCESS;
}