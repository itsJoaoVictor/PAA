#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char code[15];
    double value;
    int weight;
    int volume;
    bool used;
} Package;

typedef struct {
    char plate[8];
    int max_weight;
    int max_volume;
    int total_packages;
    int *package_indices;
    double total_value;
    int total_weight;
    int total_volume;
} Vehicle;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (!input || !output) {
        perror("Failed to open file");
        return 1;
    }

    int n;
    fscanf(input, "%d", &n);
    Vehicle *vehicles = malloc(n * sizeof(Vehicle));
    for (int i = 0; i < n; i++) {
        fscanf(input, "%7s %d %d", vehicles[i].plate, &vehicles[i].max_weight, &vehicles[i].max_volume);
        vehicles[i].total_packages = 0;
        vehicles[i].package_indices = NULL;
        vehicles[i].total_value = 0.0;
        vehicles[i].total_weight = 0;
        vehicles[i].total_volume = 0;
    }

    int m;
    fscanf(input, "%d", &m);
    Package *packages = malloc(m * sizeof(Package));
    for (int i = 0; i < m; i++) {
        fscanf(input, "%14s %lf %d %d", packages[i].code, &packages[i].value, &packages[i].weight, &packages[i].volume);
        packages[i].used = false;
    }

    for (int v_idx = 0; v_idx < n; v_idx++) {
        Vehicle *vehicle = &vehicles[v_idx];
        int W = vehicle->max_weight;
        int V = vehicle->max_volume;

        int *feasible = malloc(m * sizeof(int));
        int feasible_count = 0;
        for (int i = 0; i < m; i++) {
            if (!packages[i].used && packages[i].weight <= W && packages[i].volume <= V) {
                feasible[feasible_count++] = i;
            }
        }

        if (feasible_count == 0) {
            free(feasible);
            continue;
        }

        int dp_size = (W + 1) * (V + 1);
        double *dp = malloc(dp_size * sizeof(double));
        for (int i = 0; i < dp_size; i++) {
            dp[i] = -1.0;
        }
        dp[0] = 0.0;

        for (int i = 0; i < feasible_count; i++) {
            int p_idx = feasible[i];
            Package *p = &packages[p_idx];
            int p_weight = p->weight;
            int p_volume = p->volume;
            double p_value = p->value;

            for (int w = W; w >= p_weight; w--) {
                for (int v = V; v >= p_volume; v--) {
                    int current = w * (V + 1) + v;
                    int prev = (w - p_weight) * (V + 1) + (v - p_volume);
                    if (dp[prev] >= 0 && (dp[prev] + p_value > dp[current])) {
                        dp[current] = dp[prev] + p_value;
                    }
                }
            }
        }

        double max_val = -1.0;
        int max_w = 0;
        int max_v = 0;
        for (int w = 0; w <= W; w++) {
            for (int v = 0; v <= V; v++) {
                int idx = w * (V + 1) + v;
                if (dp[idx] > max_val) {
                    max_val = dp[idx];
                    max_w = w;
                    max_v = v;
                }
            }
        }

        if (max_val <= 0.0) {
            free(dp);
            free(feasible);
            continue;
        }

        int current_w = max_w;
        int current_v = max_v;
        int *selected = malloc(feasible_count * sizeof(int));
        int selected_count = 0;

        for (int i = feasible_count - 1; i >= 0; i--) {
            int p_idx = feasible[i];
            Package *p = &packages[p_idx];
            if (p->used) continue;

            int needed_w = current_w - p->weight;
            int needed_v = current_v - p->volume;
            if (needed_w >= 0 && needed_v >= 0) {
                int prev_idx = needed_w * (V + 1) + needed_v;
                if (dp[prev_idx] >= 0 && (dp[prev_idx] + p->value == dp[current_w * (V + 1) + current_v])) {
                    selected[selected_count++] = p_idx;
                    current_w = needed_w;
                    current_v = needed_v;
                    p->used = true;
                }
            }
        }

        for (int i = 0; i < selected_count / 2; i++) {
            int temp = selected[i];
            selected[i] = selected[selected_count - 1 - i];
            selected[selected_count - 1 - i] = temp;
        }

        vehicle->package_indices = malloc(selected_count * sizeof(int));
        memcpy(vehicle->package_indices, selected, selected_count * sizeof(int));
        vehicle->total_packages = selected_count;
        vehicle->total_value = max_val;
        vehicle->total_weight = max_w;
        vehicle->total_volume = max_v;

        free(selected);
        free(dp);
        free(feasible);
    }

    for (int i = 0; i < n; i++) {
        Vehicle *v = &vehicles[i];
        if (v->total_packages == 0) continue;

        int weight_percent = (int)((v->total_weight * 100.0) / v->max_weight + 0.5);
        int volume_percent = (int)((v->total_volume * 100.0) / v->max_volume + 0.5);
        fprintf(output, "[%s]R$%.2lf,%dKG(%d%%),%dL(%d%%)->", v->plate, v->total_value, v->total_weight, weight_percent, v->total_volume, volume_percent);

        for (int j = 0; j < v->total_packages; j++) {
            if (j > 0) fprintf(output, ",");
            fprintf(output, "%s", packages[v->package_indices[j]].code);
        }
        fprintf(output, "\n");
    }

    double pending_value = 0.0;
    int pending_weight = 0;
    int pending_volume = 0;
    int pending_count = 0;
    for (int i = 0; i < m; i++) {
        if (!packages[i].used) {
            pending_value += packages[i].value;
            pending_weight += packages[i].weight;
            pending_volume += packages[i].volume;
            pending_count++;
        }
    }

    if (pending_count > 0) {
        fprintf(output, "PENDENTE:R$%.2lf,%dKG,%dL->", pending_value, pending_weight, pending_volume);
        int first = 1;
        for (int i = 0; i < m; i++) {
            if (!packages[i].used) {
                if (!first) {
                    fprintf(output, ",");
                }
                fprintf(output, "%s", packages[i].code);
                first = 0;
            }
        }
        fprintf(output, "\n");
    }

    for (int i = 0; i < n; i++) {
        free(vehicles[i].package_indices);
    }
    free(vehicles);
    free(packages);

    fclose(input);
    fclose(output);

    return 0;
}