#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define TAM_STRING 100

// Estrutura para representar um produto
typedef struct {
    char numero_de_serie[TAM_STRING];
    int resistencia;
    int durabilidade;
} Produto;

// Função para ler os dados do arquivo CSV
int ler_dados(Produto produtos[], const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int num_produtos = 0;
    while (fscanf(arquivo, "%[^;];%d;%d\n", produtos[num_produtos].numero_de_serie,
                  &produtos[num_produtos].resistencia, &produtos[num_produtos].durabilidade) != EOF) {
        num_produtos++;
    }

    fclose(arquivo);
    return num_produtos;
}

// Função para calcular a média de uma coluna
float calcular_media(Produto produtos[], int num_produtos, int coluna) {
    int soma = 0;
    int i;
    for ( i = 0; i < num_produtos; i++) {
        if (coluna == 1)
            soma += produtos[i].resistencia;
        else
            soma += produtos[i].durabilidade;
    }
    return (float)soma / num_produtos;
}

// Função para encontrar o produto com maior e menor valor em uma coluna
void encontrar_extremos(Produto produtos[], int num_produtos, int coluna, Produto *maximo, Produto *minimo) {
    *maximo = produtos[0];
    *minimo = produtos[0];

	int i;
    for (i = 1; i < num_produtos; i++) {
        if (coluna == 1) {  // resistencia
            if (produtos[i].resistencia > maximo->resistencia)
                *maximo = produtos[i];
            if (produtos[i].resistencia < minimo->resistencia)
                *minimo = produtos[i];
        } else {  // durabilidade
            if (produtos[i].durabilidade > maximo->durabilidade)
                *maximo = produtos[i];
            if (produtos[i].durabilidade < minimo->durabilidade)
                *minimo = produtos[i];
        }
    }
}

int main() {
	setlocale(LC_ALL, "");
    Produto produtos[MAX_PRODUTOS];
    int num_produtos = ler_dados(produtos, "parametros.csv");

    // Calculando médias
    float media_resistencia = calcular_media(produtos, num_produtos, 1);
    float media_durabilidade = calcular_media(produtos, num_produtos, 2);

    // Encontrando produtos com maior e menor resistência
    Produto max_resistencia, min_resistencia;
    encontrar_extremos(produtos, num_produtos, 1, &max_resistencia, &min_resistencia);

    // Encontrando produtos com maior e menor durabilidade
    Produto max_durabilidade, min_durabilidade;
    encontrar_extremos(produtos, num_produtos, 2, &max_durabilidade, &min_durabilidade);

    // Contando produtos acima e abaixo das médias
    int acima_media = 0, abaixo_media = 0;
    int i;
    for (i = 0; i < num_produtos; i++) {
        if (produtos[i].resistencia > media_resistencia || produtos[i].durabilidade > media_durabilidade)
            acima_media++;
        else
            abaixo_media++;
    }

    // Escrevendo no arquivo
    FILE *arquivo_saida = fopen("qualidade.md", "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
    }

    fprintf(arquivo_saida, "# Análise da Qualidade dos Produtos\n\n");
    fprintf(arquivo_saida, "## Médias\n");
    fprintf(arquivo_saida, "- Média de resistência: %.2f\n", media_resistencia);
    fprintf(arquivo_saida, "- Média de durabilidade: %.2f\n\n", media_durabilidade);

    fprintf(arquivo_saida, "## Produtos com Maior e Menor Resistência\n");
    fprintf(arquivo_saida, "- Maior resistência: %s - Resistência: %d\n", max_resistencia.numero_de_serie, max_resistencia.resistencia);
    fprintf(arquivo_saida, "- Menor resistência: %s - Resistência: %d\n\n", min_resistencia.numero_de_serie, min_resistencia.resistencia);

    fprintf(arquivo_saida, "## Produtos com Maior e Menor Durabilidade\n");
    fprintf(arquivo_saida, "- Maior durabilidade: %s - Durabilidade: %d\n", max_durabilidade.numero_de_serie, max_durabilidade.durabilidade);
    fprintf(arquivo_saida, "- Menor durabilidade: %s - Durabilidade: %d\n\n", min_durabilidade.numero_de_serie, min_durabilidade.durabilidade);

    fprintf(arquivo_saida, "## Produtos Acima e Abaixo das Médias\n");
    fprintf(arquivo_saida, "- Produtos acima das médias: %d\n", acima_media);
    fprintf(arquivo_saida, "- Produtos abaixo das médias: %d\n", abaixo_media);

    fclose(arquivo_saida);

    return 0;
}

