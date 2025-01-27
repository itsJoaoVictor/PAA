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

void imprimirPacotes(Pacote pacotes[], int n, FILE* arquivoSaida, int adicionarDelimitadorFinal) {
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            fprintf(arquivoSaida, "|");  // Coloca o delimitador no início da linha
        } else {
            fprintf(arquivoSaida, "|");  // Coloca o delimitador entre os pacotes
        }

        for (int j = 0; j < pacotes[i].tamanho; j++) {
            fprintf(arquivoSaida, "%02X", pacotes[i].dados[j]);
            if (j < pacotes[i].tamanho - 1) {
                fprintf(arquivoSaida, ",");
            }
        }

        if (adicionarDelimitadorFinal && i == n - 1) {
            fprintf(arquivoSaida, "|");  // Adiciona o delimitador final na última linha
        }
    }
}

void agruparPacotes(Pacote pacotes[], int n, FILE* arquivoSaida) {
    int i = 0;
    int totalBytes = 0;
    int pacotesAgrupados = 0;

    while (i < n) {
        if (pacotesAgrupados > 0) {
            fprintf(arquivoSaida, "\n");  // Nova linha para cada agrupamento
        }

        int j = 0;
        totalBytes = 0;
        pacotesAgrupados = 0;

        // Agrupar pacotes até que o totalBytes atinja um limite de 16 bytes
        while (i + j < n && totalBytes + pacotes[i + j].tamanho <= 16) {
            totalBytes += pacotes[i + j].tamanho;
            j++;
            pacotesAgrupados++;
        }

        // Verifica se a soma dos pacotes ultrapassaria 16 bytes
        if (totalBytes > 16) {
            pacotesAgrupados--;  // Retira o último pacote do agrupamento
            totalBytes -= pacotes[i + j - 1].tamanho;
        }

        // Imprime os pacotes agrupados com a adição do delimitador final
        imprimirPacotes(pacotes + i, pacotesAgrupados, arquivoSaida, 1);
        i += pacotesAgrupados;
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

    for (int i = 0; i < totalPacotes; i += pacotesPorLeitura) {
        int limite = (i + pacotesPorLeitura <= totalPacotes) ? pacotesPorLeitura : totalPacotes - i;

        // Ordena parcialmente os pacotes
        heapsort(pacotes + i, limite);

        // Imprime a saída dos pacotes ordenados
        if (i > 0) {
            fprintf(arquivoSaida, "\n");
        }

        agruparPacotes(pacotes + i, limite, arquivoSaida);
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    free(pacotes);

    return EXIT_SUCCESS;
}
