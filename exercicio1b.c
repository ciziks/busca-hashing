#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE 1
#define FALSE 0

// Leitura do Arquivo de Inteiros
int *ler_inteiros(const char *arquivo, const int n)
{
    FILE *f = fopen(arquivo, "r");

    int *inteiros = (int *)malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double)(_fim - _ini)) / CLOCKS_PER_SEC;
}

// Método para Busca Sequencial com Realocação com método Mover-para-frente
bool busca_sequencial_mover_frente(int *tabela, int tamanho, int elemento)
{
    int index = 0, aux1, aux2;

    // Percorre a lista até encontrar o elemento
    while (tabela[index] != elemento && index < tamanho)
        index++;

    // Elemento não encontrado
    if (index == tamanho)
        return FALSE;

    // Realocação do elemento da posição i para a posição 0, caso i != 0
    if (index != 0)
    {
        aux1 = tabela[0];

        // Realocando elementos para frente
        for (int j = 0; j < index; j++)
        {
            aux2 = tabela[j + 1];
            tabela[j + 1] = aux1;
            aux1 = aux2;
        }

        tabela[0] = elemento;
    }

    return TRUE; // Encontrou o elemento
}

int main(int argc, char const *argv[])
{
    bool encontrou;
    const int N = 50000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("inteiros_busca.txt", N);

    inicia_tempo();

    for (int i = 0; i < N; i++)
    {
        encontrou = busca_sequencial_mover_frente(entradas, N, consultas[i]);
        if (encontrou)
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
