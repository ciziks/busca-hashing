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

// Método Quicksort para Ordenar a Lista
void quicksort(int v[], int ini, int fim)
{

    int pivo = v[(ini + fim) / 2];

    int j = fim;
    int i = ini;
    int aux;

    if (ini >= fim)
        return;

    while (i < j)
    {
        // Acha elemento maior que pivo na metade inferior
        while (v[i] < pivo)
            i++;

        // Acha elemento menor que o pivo na metade superior
        while (v[j] > pivo)
            j--;

        aux = v[i];
        v[i] = v[j];
        v[j] = aux; // Troca v[i] e v[j]
    }

    quicksort(v, ini, i - 1);
    quicksort(v, i + 1, fim);
}

int *criar_indice(int *v, int index_size, int n)
{

    int *indice = malloc(sizeof(int) * index_size);

    for (int i = 0; i < index_size; i++)
    {
        indice[i] = v[i * (n / index_size)];
    }

    return indice;
}

bool busca_sequencial_index(int *entrada, int tamanho_entrada, int *indice, int index_size, int buscado)
{

    int i = 0;
    while (buscado >= indice[i] && i < index_size)
        i++;

    if (i == 0)
    {
        if (buscado = entrada[0])
        {
            return TRUE;
        }
        else
            return FALSE;
    }

    for (int j = (i - 1) * tamanho_entrada / index_size; j < i * tamanho_entrada / index_size; j++)
    {

        if (entrada[j] == buscado)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("inteiros_busca.txt", N);

    // ordenar entrada
    quicksort(entradas, 0, N - 1);

    // criar tabela de indice
    int *indice = criar_indice(entradas, index_size, N);

    // realizar consultas na tabela de indices
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // buscar o elemento consultas[i] na entrada
        if (busca_sequencial_index(entradas, N, indice, index_size, consultas[i]))
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}