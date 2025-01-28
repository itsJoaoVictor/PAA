#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numPacote;
    int tamanho;
    int* dados;
} Pacote;

// Função para realizar o Heapsort
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

void imprimirPacotes(Pacote pacotes[], int n, FILE* arquivoSaida) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            fprintf(arquivoSaida, "|");
        }

        for (int j = 0; j < pacotes[i].tamanho; j++) {
            fprintf(arquivoSaida, "%02X", pacotes[i].dados[j]);
            if (j < pacotes[i].tamanho - 1) {
                fprintf(arquivoSaida, ",");
            }
        }
    }
}

void agruparPacotes(Pacote pacotes[], int n, int pacotesPorLeitura, FILE* arquivoSaida) {
    int i = 0;
    int ultimoNumPacote = pacotes[0].numPacote; // Número do primeiro pacote
    int pacotesAgrupados = 0; // Contador de pacotes agrupados na linha
    int pacotesForaDeOrdem = 0; // Contador de pacotes fora de ordem
    Pacote pacotesFora[n]; // Armazena pacotes fora de sequência

    while (i < n) {
        fprintf(arquivoSaida, "|");

        // Agrupar pacotes sequenciais até o limite de pacotesPorLeitura
        while (i < n && pacotesAgrupados < pacotesPorLeitura) {
            if (pacotes[i].numPacote == ultimoNumPacote) {
                // Se o pacote está na sequência, agrupa e avança
                for (int j = 0; j < pacotes[i].tamanho; j++) {
                    fprintf(arquivoSaida, "%02X", pacotes[i].dados[j]);
                    if (j < pacotes[i].tamanho - 1) {
                        fprintf(arquivoSaida, ",");
                    }
                }
                fprintf(arquivoSaida, "|");
                pacotesAgrupados++; // Aumenta o contador de pacotes agrupados
                ultimoNumPacote++; // Avança para o próximo número de pacote
                i++; // Avança para o próximo pacote
            } else {
                // Se o pacote não está na sequência, armazena ele para agrupamento posterior
                pacotesFora[pacotesForaDeOrdem] = pacotes[i];
                pacotesForaDeOrdem++; // Incrementa o contador de pacotes fora de ordem
                i++; // Avança para o próximo pacote
            }
        }

        // Se houver pacotes fora de ordem, inclui eles no próximo agrupamento
        if (pacotesForaDeOrdem > 0) {
            // Agrupar pacotes fora de ordem que podem ser agrupados com pacotes da sequência
            while (pacotesForaDeOrdem > 0 && pacotesAgrupados < pacotesPorLeitura) {
                for (int j = 0; j < pacotesForaDeOrdem; j++) {
                    if (pacotesFora[j].numPacote == ultimoNumPacote) {
                        // Se o pacote está na sequência, agrupa
                        for (int k = 0; k < pacotesFora[j].tamanho; k++) {
                            fprintf(arquivoSaida, "%02X", pacotesFora[j].dados[k]);
                            if (k < pacotesFora[j].tamanho - 1) {
                                fprintf(arquivoSaida, ",");
                            }
                        }
                        fprintf(arquivoSaida, "|");
                        pacotesAgrupados++; // Aumenta o contador de pacotes agrupados
                        ultimoNumPacote++; // Avança para o próximo número de pacote
                        pacotesFora[j] = pacotesFora[pacotesForaDeOrdem - 1]; // Substitui o pacote processado pelo último
                        pacotesForaDeOrdem--; // Diminui o contador de pacotes fora de ordem
                        j--; // Revisa o pacote recém-movido
                    }
                }
            }
        }

        // Se algum pacote foi agrupado, pula para a próxima linha
        if (pacotesAgrupados > 0) {
            fprintf(arquivoSaida, "\n");
            pacotesAgrupados = 0; // Reseta o contador para o próximo agrupamento
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
    Pacote* pacotes = (Pacote*)malloc(512 * sizeof(Pacote));  // A alocação pode ser ajustada conforme necessário

    lerPacotes(arquivoEntrada, pacotes, &totalPacotes, &pacotesPorLeitura);

    // Ordenar os pacotes pela numPacote
    heapsort(pacotes, totalPacotes);

    // Agrupar pacotes conforme o limite
    agruparPacotes(pacotes, totalPacotes, pacotesPorLeitura, arquivoSaida);

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    free(pacotes);

    return EXIT_SUCCESS;
}
