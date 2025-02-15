#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char placa[8];
    int max_peso;
    int max_volume;
} Veiculo;

typedef struct {
    char codigo[13];
    int valor;
    int peso;
    int volume;
    int usado;
} Pacote;

int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int n_vehicles;
    fscanf(input, "%d", &n_vehicles);
    Veiculo* veiculos = malloc(n_vehicles * sizeof(Veiculo));
    for (int i = 0; i < n_vehicles; i++) {
        fscanf(input, "%7s %d %d", veiculos[i].placa, &veiculos[i].max_peso, &veiculos[i].max_volume);
    }

    int m_packages;
    fscanf(input, "%d", &m_packages);
    Pacote* pacotes = malloc(m_packages * sizeof(Pacote));
    for (int i = 0; i < m_packages; i++) {
        double valor_double;
        fscanf(input, "%12s %lf %d %d", pacotes[i].codigo, &valor_double, &pacotes[i].peso, &pacotes[i].volume);
        pacotes[i].valor = (int)(valor_double * 100 + 0.5);
        pacotes[i].usado = 0;
    }

    for (int v_idx = 0; v_idx < n_vehicles; v_idx++) {
        Veiculo veiculo = veiculos[v_idx];
        int max_peso = veiculo.max_peso;
        int max_volume = veiculo.max_volume;

        int* available_indices = malloc(m_packages * sizeof(int));
        int num_available = 0;
        for (int i = 0; i < m_packages; i++) {
            if (pacotes[i].usado) continue;
            if (pacotes[i].peso > max_peso || pacotes[i].volume > max_volume) continue;
            available_indices[num_available++] = i;
        }

        if (num_available == 0) {
            fprintf(output, "[%s]R$0.00,0KG(0%%),0L(0%%)->\n", veiculo.placa);
            free(available_indices);
            continue;
        }

        int** dp = (int**)malloc((max_peso + 1) * sizeof(int*));
        for (int w = 0; w <= max_peso; w++) {
            dp[w] = (int*)calloc(max_volume + 1, sizeof(int));
        }

        for (int i = 0; i < num_available; i++) {
            int pkg_idx = available_indices[i];
            Pacote pkg = pacotes[pkg_idx];
            int pkg_peso = pkg.peso;
            int pkg_volume = pkg.volume;
            int pkg_valor = pkg.valor;

            for (int w = max_peso; w >= pkg_peso; w--) {
                for (int v = max_volume; v >= pkg_volume; v--) {
                    if (dp[w - pkg_peso][v - pkg_volume] + pkg_valor > dp[w][v]) {
                        dp[w][v] = dp[w - pkg_peso][v - pkg_volume] + pkg_valor;
                    }
                }
            }
        }

        int max_valor = 0;
        int best_w = 0;
        int best_v = 0;
        for (int w = 0; w <= max_peso; w++) {
            for (int v = 0; v <= max_volume; v++) {
                if (dp[w][v] > max_valor) {
                    max_valor = dp[w][v];
                    best_w = w;
                    best_v = v;
                }
            }
        }

        int current_w = best_w;
        int current_v = best_v;
        int* selected = malloc(num_available * sizeof(int));
        int selected_count = 0;

        for (int i = num_available - 1; i >= 0; i--) {
            int pkg_idx = available_indices[i];
            Pacote pkg = pacotes[pkg_idx];
            if (current_w >= pkg.peso && current_v >= pkg.volume) {
                int prev_w = current_w - pkg.peso;
                int prev_v = current_v - pkg.volume;
                if (dp[prev_w][prev_v] + pkg.valor == dp[current_w][current_v]) {
                    selected[selected_count++] = pkg_idx;
                    current_w = prev_w;
                    current_v = prev_v;
                }
            }
        }

        for (int i = 0; i < selected_count / 2; i++) {
            int temp = selected[i];
            selected[i] = selected[selected_count - 1 - i];
            selected[selected_count - 1 - i] = temp;
        }

        for (int i = 0; i < selected_count; i++) {
            pacotes[selected[i]].usado = 1;
        }

        int sum_peso = 0;
        int sum_volume = 0;
        for (int i = 0; i < selected_count; i++) {
            sum_peso += pacotes[selected[i]].peso;
            sum_volume += pacotes[selected[i]].volume;
        }

        int percent_peso = 0;
        if (veiculo.max_peso > 0) {
            percent_peso = (sum_peso * 100) / veiculo.max_peso;
        }
        int percent_volume = 0;
        if (veiculo.max_volume > 0) {
            percent_volume = (sum_volume * 100) / veiculo.max_volume;
        }

        fprintf(output, "[%s]R$%.2f,%dKG(%d%%),%dL(%d%%)->", veiculo.placa, (double)max_valor / 100.0, sum_peso, percent_peso, sum_volume, percent_volume);
        for (int i = 0; i < selected_count; i++) {
            if (i > 0) fprintf(output, ",");
            fprintf(output, "%s", pacotes[selected[i]].codigo);
        }
        fprintf(output, "\n");

        free(selected);
        for (int w = 0; w <= max_peso; w++) {
            free(dp[w]);
        }
        free(dp);
        free(available_indices);
    }

    int pending_count = 0;
    double pending_valor = 0.0;
    int pending_peso = 0;
    int pending_volume = 0;
    char** pending_codigos = malloc(m_packages * sizeof(char*));
    for (int i = 0; i < m_packages; i++) {
        if (!pacotes[i].usado) {
            pending_valor += (double)pacotes[i].valor / 100.0;
            pending_peso += pacotes[i].peso;
            pending_volume += pacotes[i].volume;
            pending_codigos[pending_count] = pacotes[i].codigo;
            pending_count++;
        }
    }

    if (pending_count > 0) {
        fprintf(output, "PENDENTE:R$%.2f,%dKG,%dL->", pending_valor, pending_peso, pending_volume);
        for (int i = 0; i < pending_count; i++) {
            if (i > 0) fprintf(output, ",");
            fprintf(output, "%s", pending_codigos[i]);
        }
        fprintf(output, "\n");
    }

    free(pending_codigos);
    free(veiculos);
    free(pacotes);

    fclose(input);
    fclose(output);
    return 0;
}