#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PLATE 10
#define MAX_CODE 20

// Estrutura para veículos
typedef struct {
    char plate[MAX_PLATE];
    int maxWeight;
    int maxVolume;
} Vehicle;

// Estrutura para pacotes
typedef struct {
    char code[MAX_CODE];
    double value;
    int weight;
    int volume;
    int assigned; // 0: disponível; 1: já carregado
} Package;

// Estrutura para armazenar os pacotes disponíveis com seu score para o veículo atual
typedef struct {
    int idx;      // índice do pacote no vetor global
    double score; // custo-benefício calculado
} PackageScore;

// Estrutura da célula da DP (para 2D knapsack)
typedef struct {
    double value;   // valor total máximo alcançado até esta célula
    int itemIndex;  // índice do pacote que foi adicionado para alcançar esse valor (-1 se nenhum)
    int prevW;      // peso acumulado anterior
    int prevV;      // volume acumulado anterior
} DPCell;

// Função de comparação para qsort (decrescente por score)
int cmpPackageScore(const void *a, const void *b) {
    PackageScore *p1 = (PackageScore*)a;
    PackageScore *p2 = (PackageScore*)b;
    if (p2->score > p1->score)
        return 1;
    else if (p2->score < p1->score)
        return -1;
    else
        return 0;
}

// Função de comparação para ordenar os índices dos pacotes na ordem de entrada (crescente)
int cmpInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
    // Verifica se os argumentos foram passados corretamente
    if(argc < 3){
        fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    // Abrindo arquivos de entrada e saída
    FILE* input = fopen(argv[1], "r");
    if(!input){
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }
    FILE* output = fopen(argv[2], "w");
    if(!output){
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        fclose(input);
        return 1;
    }
    
    int numVehicles;
    fscanf(input, "%d", &numVehicles);
    Vehicle *vehicles = (Vehicle*) malloc(numVehicles * sizeof(Vehicle));
    for (int i = 0; i < numVehicles; i++) {
        fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].maxWeight, &vehicles[i].maxVolume);
    }
    
    int numPackages;
    fscanf(input, "%d", &numPackages);
    Package *packages = (Package*) malloc(numPackages * sizeof(Package));
    for (int i = 0; i < numPackages; i++) {
        fscanf(input, "%s %lf %d %d", packages[i].code, &packages[i].value, &packages[i].weight, &packages[i].volume);
        packages[i].assigned = 0;
    }
    
    // Processa cada veículo na ordem de entrada
    for (int v = 0; v < numVehicles; v++) {
        Vehicle currVehicle = vehicles[v];
        int capW = currVehicle.maxWeight;
        int capV = currVehicle.maxVolume;
        
        // Cria um vetor com os índices dos pacotes disponíveis
        int availableCount = 0;
        for (int i = 0; i < numPackages; i++) {
            if (packages[i].assigned == 0) {
                availableCount++;
            }
        }
        if (availableCount == 0) {
            // Nenhum pacote disponível para este veículo
            fprintf(output, "[%s]R$0.00,0KG(0%%),0L(0%%)->\n", currVehicle.plate);
            continue;
        }
        
        PackageScore *availPackages = (PackageScore*) malloc(availableCount * sizeof(PackageScore));
        int j = 0;
        for (int i = 0; i < numPackages; i++) {
            if (packages[i].assigned == 0) {
                availPackages[j].idx = i;
                /* 
                   Calcula o score do pacote para este veículo.  
                   Quanto menor o consumo relativo (em peso e volume) para o caminhão, melhor será o score.
                */
                double relWeight = (double) packages[i].weight / currVehicle.maxWeight;
                double relVolume = (double) packages[i].volume / currVehicle.maxVolume;
                availPackages[j].score = packages[i].value / (relWeight + relVolume);
                j++;
            }
        }
        // Ordena os pacotes disponíveis por score decrescente
        qsort(availPackages, availableCount, sizeof(PackageScore), cmpPackageScore);
        
        // Aloca a tabela DP de tamanho (capW+1) x (capV+1)
        int dpSize = (capW + 1) * (capV + 1);
        DPCell *dp = (DPCell*) malloc(dpSize * sizeof(DPCell));
        for (int i = 0; i < dpSize; i++) {
            dp[i].value = 0.0;
            dp[i].itemIndex = -1;
            dp[i].prevW = 0;
            dp[i].prevV = 0;
        }
        
        double bestVal = 0.0;
        int bestW = 0, bestV_val = 0;
        
        // Para cada pacote disponível (na ordem definida pelo score) 
        // faz a atualização da tabela DP – 0/1 knapsack com duas restrições.
        for (int k = 0; k < availableCount; k++) {
            int pkgIdx = availPackages[k].idx;
            Package pkg = packages[pkgIdx];
            // Se o pacote não cabe de forma isolada, pode ser ignorado para este veículo
            if (pkg.weight > capW || pkg.volume > capV)
                continue;
            // Percorre a tabela em ordem decrescente para não reutilizar o mesmo pacote
            for (int w = capW; w >= pkg.weight; w--) {
                for (int v_cap = capV; v_cap >= pkg.volume; v_cap--) {
                    int currPos = w * (capV + 1) + v_cap;
                    int prevPos = (w - pkg.weight) * (capV + 1) + (v_cap - pkg.volume);
                    double candidate = dp[prevPos].value + pkg.value;
                    if (candidate > dp[currPos].value) {
                        dp[currPos].value = candidate;
                        dp[currPos].itemIndex = pkgIdx; // armazena o índice do pacote
                        dp[currPos].prevW = w - pkg.weight;
                        dp[currPos].prevV = v_cap - pkg.volume;
                        if (candidate > bestVal) {
                            bestVal = candidate;
                            bestW = w;
                            bestV_val = v_cap;
                        }
                    }
                }
            }
        }
        
        // Recupera os pacotes selecionados via backtracking a partir da célula com valor máximo
        int *chosenIdx = (int*) malloc(availableCount * sizeof(int));
        int chosenCount = 0;
        int curW = bestW, curV_cur = bestV_val;
        while (curW >= 0 && curV_cur >= 0) {
            int cellIndex = curW * (capV + 1) + curV_cur;
            if (dp[cellIndex].itemIndex == -1)
                break;
            int pkgIdx = dp[cellIndex].itemIndex;
            chosenIdx[chosenCount++] = pkgIdx;
            int newW = dp[cellIndex].prevW;
            int newV = dp[cellIndex].prevV;
            curW = newW;
            curV_cur = newV;
        }
        // Ordena os pacotes escolhidos na ordem de entrada (crescente)
        qsort(chosenIdx, chosenCount, sizeof(int), cmpInt);
        
        // Marca os pacotes selecionados como já carregados
        int totalWeight = 0, totalVolume = 0;
        for (int i = 0; i < chosenCount; i++) {
            int idx = chosenIdx[i];
            packages[idx].assigned = 1;
            totalWeight += packages[idx].weight;
            totalVolume += packages[idx].volume;
        }
        
        // Calcula as porcentagens de utilização do caminhão
        int percWeight = (int) round(((double) totalWeight * 100.0) / currVehicle.maxWeight);
        int percVolume = (int) round(((double) totalVolume * 100.0) / currVehicle.maxVolume);
        
        // Imprime a linha de saída para o veículo
        // Exemplo: [AAA1234]R$100.00,49KG(98%),10L(10%)->IJ777888999KL
        fprintf(output, "[%s]R$%.2f,%dKG(%d%%),%dL(%d%%)->", currVehicle.plate, bestVal, totalWeight, percWeight, totalVolume, percVolume);
        for (int i = 0; i < chosenCount; i++) {
            fprintf(output, "%s", packages[ chosenIdx[i] ].code);
            if (i < chosenCount - 1)
                fprintf(output, ",");
        }
        fprintf(output, "\n");
        
        // Libera memória utilizada na DP e na lista de pacotes disponíveis para este veículo
        free(dp);
        free(availPackages);
        free(chosenIdx);
    } // fim do loop dos veículos
    
    // Após processar todos os veículos, lista os pacotes pendentes
    double pendValue = 0.0;
    int pendWeight = 0, pendVolume = 0;
    int pendCount = 0;
    // Para manter a ordem de entrada, percorre os pacotes na ordem original
    for (int i = 0; i < numPackages; i++) {
        if (packages[i].assigned == 0) {
            pendValue += packages[i].value;
            pendWeight += packages[i].weight;
            pendVolume += packages[i].volume;
            pendCount++;
        }
    }
    if (pendCount > 0) {
        fprintf(output, "PENDENTE:R$%.2f,%dKG,%dL->", pendValue, pendWeight, pendVolume);
        int printed = 0;
        for (int i = 0; i < numPackages; i++) {
            if (packages[i].assigned == 0) {
                if (printed > 0)
                    fprintf(output, ",");
                fprintf(output, "%s", packages[i].code);
                printed++;
            }
        }
        fprintf(output, "\n");
    }
    
    // Fecha os arquivos e libera a memória
    free(vehicles);
    free(packages);
    fclose(input);
    fclose(output);
    
    return 0;
}
