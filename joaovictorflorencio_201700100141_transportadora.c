#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PACKAGES 100000
#define MAX_VEHICLES 100000

typedef struct {
    char code[14];
    double value;
    int weight;
    int volume;
    int used;
    double efficiency;
} Package;

typedef struct {
    char plate[8];
    int max_weight;
    int max_volume;
    Package** packages;
    int package_count;
    double total_value;
    int total_weight;
    int total_volume;
} Vehicle;

// Calcula a eficiência baseada na lógica dada
double calculateEfficiency(Package* p, int remaining_weight, int remaining_volume) {
    if (p->weight > remaining_weight || p->volume > remaining_volume) return -1; // Excede limite
    double weight_ratio = (double)p->weight / remaining_weight;
    double volume_ratio = (double)p->volume / remaining_volume;
    return p->value / sqrt(weight_ratio * volume_ratio);
}

// Função de comparação para a heap (maior eficiência primeiro)
int comparePackages(const void* a, const void* b) {
    Package* p1 = *(Package**)a;
    Package* p2 = *(Package**)b;
    return (p2->efficiency > p1->efficiency) - (p2->efficiency < p1->efficiency);
}

// Lê os veículos do arquivo
void readVehicles(FILE* input, Vehicle** vehicles, int* n_vehicles) {
    fscanf(input, "%d", n_vehicles);
    *vehicles = malloc(*n_vehicles * sizeof(Vehicle));
    for (int i = 0; i < *n_vehicles; i++) {
        fscanf(input, "%s %d %d", (*vehicles)[i].plate, &(*vehicles)[i].max_weight, &(*vehicles)[i].max_volume);
        (*vehicles)[i].package_count = 0;
        (*vehicles)[i].packages = malloc(MAX_PACKAGES * sizeof(Package*));
        (*vehicles)[i].total_value = 0;
        (*vehicles)[i].total_weight = 0;
        (*vehicles)[i].total_volume = 0;
    }
}

// Lê os pacotes do arquivo
void readPackages(FILE* input, Package** packages, int* m_packages) {
    fscanf(input, "%d", m_packages);
    *packages = malloc(*m_packages * sizeof(Package));
    for (int i = 0; i < *m_packages; i++) {
        fscanf(input, "%s %lf %d %d", (*packages)[i].code, &(*packages)[i].value, &(*packages)[i].weight, &(*packages)[i].volume);
        (*packages)[i].used = 0;
    }
}

// Aloca pacotes nos veículos de forma otimizada
void allocatePackages(Vehicle* vehicles, int n_vehicles, Package* packages, int m_packages) {
    Package** packageHeap = malloc(m_packages * sizeof(Package*));

    for (int i = 0; i < n_vehicles; i++) {
        Vehicle* v = &vehicles[i];
        int remaining_weight = v->max_weight;
        int remaining_volume = v->max_volume;
        int heapSize = 0;

        // Calcula eficiência dos pacotes restantes e insere na heap
        for (int j = 0; j < m_packages; j++) {
            if (!packages[j].used) {
                packages[j].efficiency = calculateEfficiency(&packages[j], remaining_weight, remaining_volume);
                if (packages[j].efficiency > 0) {
                    packageHeap[heapSize++] = &packages[j];
                }
            }
        }

        // Ordena pacotes pela eficiência usando quicksort (poderia ser heap para maior eficiência em grande escala)
        qsort(packageHeap, heapSize, sizeof(Package*), comparePackages);

        // Alocar pacotes no veículo
        for (int j = 0; j < heapSize; j++) {
            Package* p = packageHeap[j];
            if (p->weight <= remaining_weight && p->volume <= remaining_volume) {
                v->packages[v->package_count++] = p;
                v->total_value += p->value;
                v->total_weight += p->weight;
                v->total_volume += p->volume;
                remaining_weight -= p->weight;
                remaining_volume -= p->volume;
                p->used = 1;
            }
        }
    }

    free(packageHeap);
}

// Escreve a saída no arquivo
void writeOutput(FILE* output, Vehicle* vehicles, int n_vehicles, Package* packages, int m_packages) {
    for (int i = 0; i < n_vehicles; i++) {
        Vehicle* v = &vehicles[i];
        int weight_percent = (v->total_weight * 100) / v->max_weight;
        int volume_percent = (v->total_volume * 100) / v->max_volume;
        fprintf(output, "[%s]R$%.2lf,%dKG(%d%%),%dL(%d%%)->", v->plate, v->total_value, v->total_weight, weight_percent, v->total_volume, volume_percent);
        for (int j = 0; j < v->package_count; j++) {
            if (j > 0) fprintf(output, ",");
            fprintf(output, "%s", v->packages[j]->code);
        }
        fprintf(output, "\n");
    }

    double pending_value = 0;
    int pending_weight = 0;
    int pending_volume = 0;
    int pending_count = 0;
    char** pending_codes = malloc(m_packages * sizeof(char*));

    for (int i = 0; i < m_packages; i++) {
        if (!packages[i].used) {
            pending_value += packages[i].value;
            pending_weight += packages[i].weight;
            pending_volume += packages[i].volume;
            pending_codes[pending_count++] = packages[i].code;
        }
    }

    if (pending_count > 0) {
        fprintf(output, "PENDENTE:R$%.2lf,%dKG,%dL->", pending_value, pending_weight, pending_volume);
        for (int i = 0; i < pending_count; i++) {
            if (i > 0) fprintf(output, ",");
            fprintf(output, "%s", pending_codes[i]);
        }
        fprintf(output, "\n");
    }

    free(pending_codes);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input output\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if (!input || !output) {
        perror("Failed to open file");
        return 1;
    }

    Vehicle* vehicles = NULL;
    int n_vehicles = 0;
    readVehicles(input, &vehicles, &n_vehicles);

    Package* packages = NULL;
    int m_packages = 0;
    readPackages(input, &packages, &m_packages);

    allocatePackages(vehicles, n_vehicles, packages, m_packages);

    writeOutput(output, vehicles, n_vehicles, packages, m_packages);

    for (int i = 0; i < n_vehicles; i++) {
        free(vehicles[i].packages);
    }
    free(vehicles);
    free(packages);

    fclose(input);
    fclose(output);
    return 0;
}
