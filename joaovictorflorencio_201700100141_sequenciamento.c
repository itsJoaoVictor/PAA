#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DNA 10000
#define MAX_GENES 10
#define MAX_DOENCAS 100

// Função para calcular o prefixo do KMP
void calcularPrefixo(char *padrao, int *prefixo, int m) {
    int j = 0;
    prefixo[0] = 0;
    for (int i = 1; i < m; i++) {
        while (j > 0 && padrao[i] != padrao[j]) {
            j = prefixo[j - 1];
        }
        if (padrao[i] == padrao[j]) {
            j++;
        }
        prefixo[i] = j;
    }
}

// Função para buscar padrões usando KMP
int buscarKMP(char *dna, char *gene, int tamanho_subcadeia) {
    int n = strlen(dna);
    int m = strlen(gene);
    int *prefixo = (int *)malloc(m * sizeof(int));
    calcularPrefixo(gene, prefixo, m);

    int j = 0, correspondencias = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && dna[i] != gene[j]) {
            j = prefixo[j - 1];
        }
        if (dna[i] == gene[j]) {
            j++;
        }
        if (j >= tamanho_subcadeia) {
            correspondencias++;
            j = prefixo[j - 1];
        }
    }
    free(prefixo);
    return correspondencias;
}

// Estrutura para armazenar doenças
typedef struct {
    char codigo[20];
    int ocorrencias;
    int total_genes;
} Doenca;

// Função de comparação para ordenação
int compararDoencas(const void *a, const void *b) {
    Doenca *d1 = (Doenca *)a;
    Doenca *d2 = (Doenca *)b;
    double p1 = (100.0 * d1->ocorrencias) / d1->total_genes;
    double p2 = (100.0 * d2->ocorrencias) / d2->total_genes;
    if (p1 != p2) return (p2 - p1) > 0 ? 1 : -1;
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    
    int tamanho_subcadeia;
    fscanf(input, "%d", &tamanho_subcadeia);
    
    char dna[MAX_DNA];
    fscanf(input, "%s", dna);
    
    int num_doencas;
    fscanf(input, "%d", &num_doencas);
    
    Doenca doencas[MAX_DOENCAS];
    
    for (int i = 0; i < num_doencas; i++) {
        fscanf(input, "%s %d", doencas[i].codigo, &doencas[i].total_genes);
        doencas[i].ocorrencias = 0;
        
        for (int j = 0; j < doencas[i].total_genes; j++) {
            char gene[1000];
            fscanf(input, "%s", gene);
            if (buscarKMP(dna, gene, tamanho_subcadeia) > 0) {
                doencas[i].ocorrencias++;
            }
        }
    }
    
    qsort(doencas, num_doencas, sizeof(Doenca), compararDoencas);
    
    for (int i = 0; i < num_doencas; i++) {
        fprintf(output, "%s->%d%%\n", doencas[i].codigo, (int)((100.0 * doencas[i].ocorrencias) / doencas[i].total_genes));
    }
    
    fclose(input);
    fclose(output);
    return 0;
}
