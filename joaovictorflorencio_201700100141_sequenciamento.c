#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_failure(const char *pattern, int pattern_len, int *failure) {
    failure[0] = 0;
    int j = 0;
    for (int i = 1; i < pattern_len; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = failure[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        failure[i] = j;
    }
}

int kmp_search(const char *text, const char *pattern, int pattern_len) {
    int n = strlen(text);
    int *failure = malloc(pattern_len * sizeof(int));
    compute_failure(pattern, pattern_len, failure);
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = failure[j - 1];
        }
        if (text[i] == pattern[j]) {
            j++;
            if (j == pattern_len) {
                free(failure);
                return 1;
            }
        }
    }
    free(failure);
    return 0;
}

struct Disease {
    char code[9];
    int percentage;
    int original_index;
};

int compare(const void *a, const void *b) {
    const struct Disease *d1 = (const struct Disease *)a;
    const struct Disease *d2 = (const struct Disease *)b;
    if (d1->percentage != d2->percentage) {
        return d2->percentage - d1->percentage;
    } else {
        return d1->original_index - d2->original_index;
    }
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int K;
    fscanf(input, "%d", &K);

    char DNA[1000001];
    fscanf(input, "%s", DNA);

    int M;
    fscanf(input, "%d", &M);

    struct Disease *diseases = malloc(M * sizeof(struct Disease));

    for (int i = 0; i < M; i++) {
        char code[9];
        int num_genes;
        fscanf(input, "%s %d", code, &num_genes);

        char genes[10][1001];
        for (int j = 0; j < num_genes; j++) {
            fscanf(input, "%s", genes[j]);
        }

        int detected_genes = 0;
        for (int j = 0; j < num_genes; j++) {
            char *gene = genes[j];
            int len_gene = strlen(gene);
            if (len_gene < K) {
                continue;
            }
            int total_substrings = len_gene - K + 1;
            int required = (9 * total_substrings + 9) / 10; // Ceiling of 90%
            int count = 0;

            for (int pos = 0; pos <= len_gene - K; pos++) {
                char substring[K + 1];
                strncpy(substring, gene + pos, K);
                substring[K] = '\0';
                if (kmp_search(DNA, substring, K)) {
                    count++;
                }
            }

            if (count >= required) {
                detected_genes++;
            }
        }

        int percentage = (detected_genes * 100 + num_genes / 2) / num_genes; // Arredondamento correto
        diseases[i].percentage = percentage;
        strcpy(diseases[i].code, code);
        diseases[i].original_index = i;
    }

    qsort(diseases, M, sizeof(struct Disease), compare);

    for (int i = 0; i < M; i++) {
        fprintf(output, "%s->%d%%\n", diseases[i].code, diseases[i].percentage);
    }

    free(diseases);
    fclose(input);
    fclose(output);
    return 0;
}