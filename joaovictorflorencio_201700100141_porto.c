#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAMANHO_MAX_NOME 12
#define TAMANHO_MAX_NOME2 17

typedef struct {
    char nome[TAMANHO_MAX_NOME];
    char nome2[TAMANHO_MAX_NOME2];
    int peso;
    int diferenca;
    int percentual;
} Dados;

void intercala(Dados arr[], int esquerda, int meio, int direita) {
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;
    Dados arrayEsquerda[n1], arrayDireita[n2];

    for (int i = 0; i < n1; i++) arrayEsquerda[i] = arr[esquerda + i];
    for (int j = 0; j < n2; j++) arrayDireita[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = esquerda;
    while (i < n1 && j < n2) {
        if (arrayEsquerda[i].percentual >= arrayDireita[j].percentual) {
            arr[k++] = arrayEsquerda[i++];
        } else {
            arr[k++] = arrayDireita[j++];
        }
    }

    while (i < n1) arr[k++] = arrayEsquerda[i++];
    while (j < n2) arr[k++] = arrayDireita[j++];
}

void mergeSort(Dados arr[], int esquerda, int direita) {
    if (esquerda < direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        mergeSort(arr, esquerda, meio);
        mergeSort(arr, meio + 1, direita);
        intercala(arr, esquerda, meio, direita);
    }
}

int buscaBinaria(Dados arr[], int tamanho, const char* alvo) {
    int baixo = 0, alto = tamanho - 1;
    while (baixo <= alto) {
        int meio = (baixo + alto) / 2;
        int comparacao = strcmp(arr[meio].nome, alvo);
        if (comparacao == 0) return meio;
        if (comparacao < 0) baixo = meio + 1;
        else alto = meio - 1;
    }
    return -1;
}

void troca(Dados* a, Dados* b) {
    Dados temp = *a;
    *a = *b;
    *b = temp;
}

int particiona(Dados arr[], int baixo, int alto) {
    char* pivo = arr[baixo].nome;
    int esquerda = baixo - 1, direita = alto + 1;
    while (1) {
        do { direita--; } while (strcmp(arr[direita].nome, pivo) > 0);
        do { esquerda++; } while (strcmp(arr[esquerda].nome, pivo) < 0);
        if (esquerda < direita) troca(&arr[esquerda], &arr[direita]);
        else return direita;
    }
}

void quickSort(Dados arr[], int baixo, int alto) {
    if (baixo < alto) {
        int indicePivo = particiona(arr, baixo, alto);
        quickSort(arr, baixo, indicePivo);
        quickSort(arr, indicePivo + 1, alto);
    }
}

void calculaMetricas(Dados* empresa1, Dados* empresa2) {
    float proporcao = (float)empresa2->peso / empresa1->peso;
    empresa1->percentual = abs(round((1 - proporcao) * 100));
    empresa1->diferenca = abs(empresa2->peso - empresa1->peso);
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

    int quantidadeEmpresas, quantidadeFiscais;
    fscanf(arquivoEntrada, "%d", &quantidadeEmpresas);
    Dados empresas[quantidadeEmpresas];

    for (int i = 0; i < quantidadeEmpresas; i++) {
        fscanf(arquivoEntrada, "%s %s %d", empresas[i].nome, empresas[i].nome2, &empresas[i].peso);
    }

    fscanf(arquivoEntrada, "%d", &quantidadeFiscais);
    Dados fiscais[quantidadeFiscais];

    for (int i = 0; i < quantidadeFiscais; i++) {
        fscanf(arquivoEntrada, "%s %s %d", fiscais[i].nome, fiscais[i].nome2, &fiscais[i].peso);
    }

    quickSort(fiscais, 0, quantidadeFiscais - 1);

    for (int i = 0; i < quantidadeEmpresas; i++) {
        int indiceEncontrado = buscaBinaria(fiscais, quantidadeFiscais, empresas[i].nome);
        if (indiceEncontrado != -1 && strcmp(empresas[i].nome2, fiscais[indiceEncontrado].nome2) != 0) {
            calculaMetricas(&empresas[i], &fiscais[indiceEncontrado]);
            fprintf(arquivoSaida, "%s:%s<->%s\n", empresas[i].nome, empresas[i].nome2, fiscais[indiceEncontrado].nome2);
        } else if (indiceEncontrado != -1) {
            calculaMetricas(&empresas[i], &fiscais[indiceEncontrado]);
        } else {
            empresas[i].percentual = 0;
            empresas[i].diferenca = 0;
        }
    }

    mergeSort(empresas, 0, quantidadeEmpresas - 1);

    for (int i = 0; i < quantidadeEmpresas; i++) {
        if (empresas[i].percentual > 10) {
            int indiceCorrespondente = buscaBinaria(fiscais, quantidadeFiscais, empresas[i].nome);
            if (indiceCorrespondente != -1 && strcmp(empresas[i].nome2, fiscais[indiceCorrespondente].nome2) == 0) {
                fprintf(arquivoSaida, "%s:%dkg(%d%%)\n", empresas[i].nome, empresas[i].diferenca, empresas[i].percentual);
            }
        }
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    return EXIT_SUCCESS;
}
