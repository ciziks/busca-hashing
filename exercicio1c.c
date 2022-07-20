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

bool busca_sequencial_transposicao(int tabela[], int tamanho, int elemento)
{
    int aux, posicao_anterior;
    for (int i = 0; i < tamanho; i++)
    {
        if (tabela[i] == elemento) // Encontrou o elemento
        {
            // Coletando posição anterior
            posicao_anterior = i - 1;

            // Se não for a primeira posição, realiza Transposição
            if (posicao_anterior != -1)
            {
                // Realizando Troca com elemento anterior
                aux = tabela[i];
                tabela[i] = tabela[posicao_anterior];
                tabela[posicao_anterior] = aux;
            }
            return TRUE;
        }
    }
    return FALSE; // Não Encontrou
}

int main(int argc, char const *argv[])
{
    bool encontrou;
    const int N = 50000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("inteiros_busca.txt", N);

    // realiza busca sequencia com realocação
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // buscar o elemento consultas[i] na entrada
        encontrou = busca_sequencial_transposicao(entradas, N, consultas[i]);
        if (encontrou)
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
