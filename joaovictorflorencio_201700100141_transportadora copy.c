#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura para Veículo
typedef struct {
    char placa[10];       
    float peso_max, volume_max; 
    float peso_atual, volume_atual; 
} Veiculo;

// Estrutura para Pacote
typedef struct {
    char codigo[20];    
    float valor, peso, volume;  
    int alocado;  
    int input_index;  
} Pacote;

// Estrutura para armazenar a alocação
typedef struct {
    Veiculo veiculo;    
    int *pacotes_indices; // Usando ponteiro para permitir tamanho dinâmico
    int num_pacotes;    
    float total_valor;  
} Alocacao;

// Função de comparação para ordenar pacotes
int comparePacote(const void *a, const void *b) {
    Pacote *pa = (Pacote*) a;
    Pacote *pb = (Pacote*) b;

    if (pb->valor != pa->valor) 
        return (pb->valor > pa->valor) - (pb->valor < pa->valor);

    double epv_a = pa->valor / sqrt((pa->peso) * (pa->volume));
    double epv_b = pb->valor / sqrt((pb->peso) * (pb->volume));

    return (epv_b > epv_a) - (epv_b < epv_a);
}

// Função de troca
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if(!input) { perror("Erro ao abrir arquivo de entrada"); return 1; }
    FILE* output = fopen(argv[2], "w");
    if(!output) { perror("Erro ao abrir arquivo de saída"); fclose(input); return 1; }

    int n_veiculos;
    fscanf(input, "%d", &n_veiculos);
    Veiculo *veiculos = malloc(n_veiculos * sizeof(Veiculo));
    for(int i = 0; i < n_veiculos; i++) {
        fscanf(input, "%s %f %f", veiculos[i].placa, &veiculos[i].peso_max, &veiculos[i].volume_max);
        veiculos[i].peso_atual = 0;
        veiculos[i].volume_atual = 0;
    }

    int n_pacotes;
    fscanf(input, "%d", &n_pacotes);
    Pacote *pacotes = malloc(n_pacotes * sizeof(Pacote));
    for(int i = 0; i < n_pacotes; i++) {
        fscanf(input, "%s %f %f %f", pacotes[i].codigo, &pacotes[i].valor, &pacotes[i].peso, &pacotes[i].volume);
        pacotes[i].alocado = 0;
        pacotes[i].input_index = i;
    }

    // Ordena pacotes pela estratégia definida
    qsort(pacotes, n_pacotes, sizeof(Pacote), comparePacote);

    Alocacao *alocs = malloc(n_veiculos * sizeof(Alocacao));
    for(int i = 0; i < n_veiculos; i++) {
        alocs[i].veiculo = veiculos[i];
        alocs[i].num_pacotes = 0;
        alocs[i].total_valor = 0;
        alocs[i].pacotes_indices = malloc(n_pacotes * sizeof(int)); // Aloca dinamicamente
        for(int j = 0; j < n_pacotes; j++) {
            alocs[i].pacotes_indices[j] = -1;
        }
    }

    // Alocação eficiente dos pacotes nos veículos
    for(int i = 0; i < n_veiculos; i++) {
        Veiculo *v = &veiculos[i];

        int alocou;
        do {
            alocou = 0;
            for (int j = 0; j < n_pacotes; j++) {
                if(pacotes[j].alocado == 0) {
                    if (v->peso_atual + pacotes[j].peso <= v->peso_max &&
                        v->volume_atual + pacotes[j].volume <= v->volume_max) {
                        
                        v->peso_atual += pacotes[j].peso;
                        v->volume_atual += pacotes[j].volume;
                        pacotes[j].alocado = 1;

                        if (alocs[i].num_pacotes < n_pacotes) {
                            alocs[i].pacotes_indices[alocs[i].num_pacotes++] = j;
                        }
                        alocs[i].total_valor += pacotes[j].valor;
                        alocou = 1;

                        if (alocs[i].num_pacotes >= 10) break;
                    }
                }
            }
        } while(alocou);
    }

    // Reordenar os pacotes alocados pela ordem de entrada original
    for (int i = 0; i < n_veiculos; i++) {
        for (int a = 0; a < alocs[i].num_pacotes - 1; a++) {
            for (int b = 0; b < alocs[i].num_pacotes - a - 1; b++) {
                int idx1 = alocs[i].pacotes_indices[b];
                int idx2 = alocs[i].pacotes_indices[b+1];
                if(pacotes[idx1].input_index > pacotes[idx2].input_index) {
                    swap(&alocs[i].pacotes_indices[b], &alocs[i].pacotes_indices[b+1]);
                }
            }
        }
    }

    // Escrever a saída formatada
    for (int i = 0; i < n_veiculos; i++) {
        Veiculo v = veiculos[i];
        Alocacao a = alocs[i];
        int percPeso = (int)round((v.peso_atual / v.peso_max) * 100);
        int percVol  = (int)round((v.volume_atual / v.volume_max) * 100);
        fprintf(output, "[%s]R$%.2f,%.0fKG(%d%%),%.0fL(%d%%)->", 
                v.placa, a.total_valor, v.peso_atual, percPeso, v.volume_atual, percVol);
        for (int j = 0; j < a.num_pacotes; j++) {
            int idx = a.pacotes_indices[j];
            fprintf(output, "%s", pacotes[idx].codigo);
            if(j < a.num_pacotes - 1)
                fprintf(output, ",");
        }
        fprintf(output, "\n");
    }

    // Impressão de pacotes pendentes
    int primeiroPendente = 1;
    for(int i = 0; i < n_pacotes; i++) {
        if(pacotes[i].alocado == 0) {
            if(primeiroPendente) {
                fprintf(output, "PENDENTE:");
                primeiroPendente = 0;
            } else {
                fprintf(output, ",");
            }
            fprintf(output, "R$%.2f,%.0fKG,%.0fL->%s", 
                    pacotes[i].valor, pacotes[i].peso, pacotes[i].volume, pacotes[i].codigo);
        }
    }
    if(!primeiroPendente)
        fprintf(output, "\n");

    fclose(input);
    fclose(output);
    free(veiculos);
    free(pacotes);
    for (int i = 0; i < n_veiculos; i++) {
        free(alocs[i].pacotes_indices);  // Libera memória alocada dinamicamente
    }
    free(alocs);

    return 0;
}
