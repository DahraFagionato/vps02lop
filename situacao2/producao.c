#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE_LENGTH 100
#define MAX_DATE_LENGTH 11
#define MAX_DAYS 31
#define MAX_MACHINES 5

typedef struct {
    char date[MAX_DATE_LENGTH];
    int machine_count[MAX_MACHINES];
} DailyProduction;

int find_or_add_date(DailyProduction productions[], int *num_days, const char *date) {
	int i;
    for (i = 0; i < *num_days; i++) {
        if (strcmp(productions[i].date, date) == 0) {
            return i;
        }
    }
    strcpy(productions[*num_days].date, date);
    memset(productions[*num_days].machine_count, 0, sizeof(int) * MAX_MACHINES);
    return (*num_days)++;
}

int main() {
	setlocale(LC_ALL, "");
    FILE *file;
    char line[MAX_LINE_LENGTH];
    DailyProduction productions[MAX_DAYS];
    int num_days = 0;

    // Abrir o arquivo CSV
    file = fopen("producao.csv", "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Ignorar a primeira linha (cabeçalho)
    fgets(line, MAX_LINE_LENGTH, file);

    // Ler cada linha do arquivo
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        int machine_number;
        char date[MAX_DATE_LENGTH], time[20];

        // Ler a data, hora e número da máquina
        sscanf(line, "%[^;];%[^;];%d", date, time, &machine_number);

        // Encontrar ou adicionar a data atual
        int day_index = find_or_add_date(productions, &num_days, date);

        // Contar o número de explosivos produzidos por cada máquina
        if (machine_number >= 1 && machine_number < MAX_MACHINES) {
            productions[day_index].machine_count[machine_number]++;
        }
    }

    // Fechar o arquivo
    fclose(file);

    // Abrir o arquivo de relatório Markdown
    file = fopen("explosivos.md", "w");
    if (file == NULL) {
        perror("Erro ao criar o arquivo de relatório");
        return 1;
    }

    // Escrever o cabeçalho da tabela
    fprintf(file, "| Data       | Máquina 1 | Máquina 2 | Máquina 3 | Máquina 4 |\n");
    fprintf(file, "|------------|-----------|-----------|-----------|-----------|\n");

    // Escrever os dados de produção por dia e por máquina
    int i, j;
    for (i = 0; i < num_days; i++) {
        fprintf(file, "| %s |", productions[i].date);
        for (j = 1; j < MAX_MACHINES; j++) {
            fprintf(file, "     %d    |", productions[i].machine_count[j]);
        }
        fprintf(file, "\n");
    }

    // Fechar o arquivo de relatório
    fclose(file);

    // Exibir a contagem de explosivos por máquina
    for (i = 1; i < MAX_MACHINES; i++) {
        int total = 0;
        for (j = 0; j < num_days; j++) {
            total += productions[j].machine_count[i];
        }
        printf("Máquina %d: %d explosivos produzidos\n", i, total);
    }

    return 0;
}
