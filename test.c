#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME_LEN 12
#define MAX_NAME2_LEN 17

typedef struct {
    char name[MAX_NAME_LEN];
    char name2[MAX_NAME2_LEN];
    int peso;
    int diferenca;
    int percentual;
} Dados;

void merge(Dados arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Dados L[n1], R[n2];
    
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].percentual >= R[j].percentual) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(Dados arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int buscaBinaria(Dados arr[], int n, const char target[]) {
    int min = 0, max = n - 1;
    while (min <= max) {
        int meio = (min + max) / 2;
        int comparacao = strcmp(arr[meio].name, target);
        if (comparacao == 0) return meio;
        else if (comparacao < 0) min = meio + 1;
        else max = meio - 1;
    }
    return -1;
}

void trocar(Dados* a, Dados* b) {
    Dados temp = *a;
    *a = *b;
    *b = temp;
}

int hoare(Dados* V, int i, int j) {
    char* P = V[i].name;
    int x = i - 1, y = j + 1;
    while (1) {
        while (strcmp(V[--y].name, P) > 0);
        while (strcmp(V[++x].name, P) < 0);
        if (x < y) trocar(&V[x], &V[y]);
        else return y;
    }
}

void quicksort(Dados* V, int i, int j) {
    if (i < j) {
        int pivo = hoare(V, i, j);
        quicksort(V, i, pivo);
        quicksort(V, pivo + 1, j);
    }
}

// Função para calcular a diferença percentual e peso
void calcularPercentualEDiferenca(Dados* empresa1, Dados* empresa2) {
    float dif = (float)empresa2->peso / empresa1->peso;
    float por = 1 - dif;
    float pos = por * 100;
    int abc = round(pos);
    empresa1->percentual = abs(abc);
    empresa1->diferenca = abs(empresa2->peso - empresa1->peso);
}

int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    
    if (!input || !output) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    int quant, fisc;
    fscanf(input, "%d", &quant);
    Dados empresa[quant];
    
    for (int i = 0; i < quant; i++) {
        fscanf(input, "%s %s %d", empresa[i].name, empresa[i].name2, &empresa[i].peso);
    }

    fscanf(input, "%d", &fisc);
    Dados empresa2[fisc];
    
    for (int i = 0; i < fisc; i++) {
        fscanf(input, "%s %s %d", empresa2[i].name, empresa2[i].name2, &empresa2[i].peso);
    }
    
    quicksort(empresa2, 0, fisc - 1);
    
    for (int i = 0; i < quant; i++) {
        int ter = buscaBinaria(empresa2, fisc, empresa[i].name);
        if (ter != -1 && strcmp(empresa[i].name2, empresa2[ter].name2) != 0) {
            calcularPercentualEDiferenca(&empresa[i], &empresa2[ter]);
            fprintf(output, "%s: %s<->%s\n", empresa[i].name, empresa[i].name2, empresa2[ter].name2);
        } else {
            if (ter != -1) {
                calcularPercentualEDiferenca(&empresa[i], &empresa2[ter]);
            } else {
                empresa[i].percentual = 0;
                empresa[i].diferenca = 0;
            }
        }
    }
    
    mergeSort(empresa, 0, quant - 1);

    for (int i = 0; i < quant; i++) {
        if (empresa[i].percentual > 10) {
            int tam = buscaBinaria(empresa2, fisc, empresa[i].name);
            if (tam != -1 && strcmp(empresa[i].name2, empresa2[tam].name2) == 0) {
                fprintf(output, "%s: %dkg (%d%%)\n", empresa[i].name, empresa[i].diferenca, empresa[i].percentual);
            }
        }
    }
    
    fclose(input);
    fclose(output);
    return 0;
}
