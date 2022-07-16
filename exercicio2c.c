#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// Responsável: LUCAS
// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Definição do tipo string
typedef char *string;

#define MAX_STRING_LEN 20

// Definição da Lista Linear Encadeada
typedef struct
{
    string *elementos;
    long tamanho;
} Lista;

// Método para converter String em sua representação inteira
unsigned converter(string s)
{
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = h * 256 + s[i];
    return h;
}

// Leitura dos Arquivos
string *ler_strings(const char *arquivo, const int n)
{
    FILE *f = fopen(arquivo, "r");

    string *strings = (string *)malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++)
    {
        strings[i] = (string)malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

// Função Hash de divisão
unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

// Função Hash de multiplicação
unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

// Método para criar uma lista
void lista_criar(Lista *l)
{
    l = (Lista *)malloc(sizeof(Lista));
    l->tamanho = 0;
    l->elementos = NULL;
}

// Método para destruir uma lista
void lista_apagar(Lista *l)
{
    l->tamanho = 0;
    free(l->elementos);
}

// Método para inserir um elemento na lista
int lista_inserir(Lista *l, string elemento)
{
    l->elementos = realloc(l->elementos, ++l->tamanho * sizeof(string));
    l->elementos[l->tamanho - 1] = elemento;
    return 0;
}

// Método para buscar dentro da Lista
bool lista_busca(Lista *l, string elemento)
{
    if (l != NULL)
    {
        for (int i = 0; i < l->tamanho; i++)
        {
            // Verificando se elemento é o nó atual
            if (!strcmp(l->elementos[i], elemento))
                return TRUE;
        }
    }

    return FALSE;
}

// Verifica se a lista está vazia
bool lista_vazia(Lista *l)
{
    if (l->tamanho > 0)
        return FALSE;
    return TRUE;
}

// Método para criar a Tabela Hash Aberta
Lista *criar_hash(unsigned B)
{
    Lista *tabela_hash = malloc(B * sizeof(Lista));

    for (int i = 0; i < B; i++)
    {
        lista_criar(&tabela_hash[i]); // Criando Bucket com Lista vazia
    }
    return tabela_hash;
}

// Destrói Tabela Hash
int destroi_hash(Lista *tabela_hash, unsigned B)
{
    // for (int i = 0; i < B; i++)
    //     lista_apagar(&tabela_hash[i]);
}

// Método para inserir na Tabela Hash (Aberto)
int inserir_hash(Lista *tabela_hash, unsigned B, unsigned *colisoes, string elemento, bool is_div)
{
    // Convertendo String para representação inteira
    unsigned convertido = converter(elemento);

    // Calculando endereço a partir de função hash especificada
    int index;

    if (is_div)
        index = h_div(convertido, B); // Função de Divisão
    else
        index = h_mul(convertido, B); // Função de Multiplicação

    // Verificando se houve colisão
    if (lista_vazia(&tabela_hash[index]))
        (*colisoes)++;

    // Verificando se elemento é duplicata
    if (!lista_busca(&tabela_hash[index], elemento))
        lista_inserir(&tabela_hash[index], elemento); // Inserindo elemento na lista do Bucket

    return 0;
}

// Buscando na Tabela Hash
bool buscar_hash(Lista *tabela_hash, unsigned B, int *encontrados, string elemento, bool is_div)
{
    // Convertendo String para representação inteira
    unsigned convertido = converter(elemento);

    // Calculando endereço a partir de função hash especificada
    int index;

    if (is_div)
        index = h_div(convertido, B); // Função de Divisão
    else
        index = h_mul(convertido, B); // Função de Multiplicação

    // Procurando na Tabela Hash
    bool encontrou = lista_busca(&tabela_hash[index], elemento);

    // Somando encontrados, se encontrou na tabela
    if (encontrou)
        (*encontrados)++;

    return encontrou;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string *insercoes = ler_strings("strings_entrada.txt", N);
    string *consultas = ler_strings("strings_busca.txt", M);

    // cria tabela hash com hash por divisão
    Lista *tabela_hash_div = criar_hash(B);

    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // inserir insercoes[i] na tabela hash
        inserir_hash(tabela_hash_div, B, &colisoes_h_div, insercoes[i], TRUE);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++)
    {
        // buscar consultas[i] na tabela hash
        buscar_hash(tabela_hash_div, B, &encontrados_h_div, consultas[i], TRUE);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
    destroi_hash(tabela_hash_div, B);

    // cria tabela hash com hash por multiplicação
    Lista *tabela_hash_mul = criar_hash(B);

    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++)
    {
        // inserir insercoes[i] na tabela hash
        inserir_hash(tabela_hash_mul, B, &colisoes_h_mul, insercoes[i], FALSE);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++)
    {
        // buscar consultas[i] na tabela hash
        buscar_hash(tabela_hash_mul, B, &encontrados_h_mul, consultas[i], FALSE);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
    destroi_hash(tabela_hash_mul, B);

    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
