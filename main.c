#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 1000
#define MAX_POSICOES 1000

typedef struct
{
    char palavra[100];
    int posicoes[MAX_POSICOES];
    int quantidade;
} Palavra;

Palavra palavras[MAX_PALAVRAS];
int numPalavras = 0;

void quickSort(Palavra *lista, int iniVet, int fimVet);
void AdicionarPalavra(const char *palavra, int posicao);
void ArmazenarPalavras(const char linha[1000], int posicaoBase);
void ImprimirPalavras();
void ForcaBruta(char T[], long n, char P[], long m);
void lerArquivo(const char *nomeArquivo, char *conteudo);
void ProcurarPalavra(char *palavra, char *texto, int *posicaoBase);
void imprimirPedaço(char *texto, int posicao);

void AdicionarPalavra(const char *palavra, int posicao)
{
    for (int i = 0; i < numPalavras; i++)
    {
        if (strcmp(palavras[i].palavra, palavra) == 0)
        {                                                           // se encontrar a palavra na posicao i
            palavras[i].posicoes[palavras[i].quantidade] = posicao; // adiociona mais uma occorencia da palavra
            palavras[i].quantidade++;
            return;
        }
    }

    // Se não encontrou a palavra, adicionar uma nova
    strcpy(palavras[numPalavras].palavra, palavra);
    palavras[numPalavras].posicoes[0] = posicao;
    palavras[numPalavras].quantidade = 1;
    numPalavras++;
}

void ArmazenarPalavras(const char linha[1000], int posicaoBase)
{
    const char *delimitadores = " ,.?:!\n";
    char *copiaLinha = strdup(linha);
    char *palavra = strtok(copiaLinha, delimitadores);

    int posicao = posicaoBase;
    while (palavra != NULL)
    {
        // Armazenar a palavra apenas se não for um delimitador
        if (strcmp(palavra, "-") != 0)
        {
            AdicionarPalavra(palavra, posicao);
        }
        posicao += strlen(palavra) + 1; // +1 para pular o delimitador
        palavra = strtok(NULL, delimitadores);
    }

    free(copiaLinha);
}

void ImprimirPalavras()
{
    for (int i = 0; i < numPalavras; i++)
    {
        printf("%s ", palavras[i].palavra);
        for (int j = 0; j < palavras[i].quantidade; j++)
        {
            printf("%d ", palavras[i].posicoes[j]);
        }
        printf("\n");
    }
}

void ForcaBruta(char T[], long n, char P[], long m)
{
    int opcao = 0;
    int cont = 0;
    long i, j, k;
    for (i = 1; i <= (n - m + 1); i++)
    {
        k = i;
        j = 1;
        while (T[k - 1] == P[j - 1] && j <= m)
        {
            j++;
            k++;
        }
        if (j > m)
        {
            printf("Casamento na posição %3ld: ", i);

            for (long l = i - 20; l <= i + 20; l++)
            {
                printf("%c", T[l]);
            }
            printf("\n");
            cont++;
        }
    }

    if (cont == 0)
    {
        printf("Padrão não encontrado\n");
    }
    else
    {
        printf("Padrão encontrado %d vezes\n", cont);
    }
}

void lerArquivo(const char *nomeArquivo, char *conteudo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    char Linha[1000];
    char *result;

    if (arquivo == NULL)
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }

    conteudo[0] = '\0'; // Inicializa o conteúdo

    int posicaoBase = 1;

    while ((result = fgets(Linha, 1000, arquivo)) != NULL)
    {
        strcat(conteudo, Linha);
        ArmazenarPalavras(Linha, posicaoBase);
        posicaoBase += strlen(Linha);
    }
    fclose(arquivo);
    quickSort(palavras, 0, numPalavras - 1);
}

void quickSort(Palavra *lista, int iniVet, int fimVet)
{
    if (iniVet < fimVet)
    {
        Palavra pivo = lista[fimVet];
        int i = iniVet - 1;

        for (int j = iniVet; j < fimVet; j++)
        {

            if (strcmp(lista[j].palavra, pivo.palavra) < 0)
            {
                i++;
                Palavra temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }

        Palavra temp = lista[i + 1];
        lista[i + 1] = lista[fimVet];
        lista[fimVet] = temp;

        int pivoIndex = i + 1;

        quickSort(lista, iniVet, pivoIndex - 1);
        quickSort(lista, pivoIndex + 1, fimVet);
    }
}

void ProcurarPalavra(char *palavra, char *texto, int *posicaoBase)
{
    int ini = 0;
    int fim = numPalavras - 1;
    int meio;

    while (ini <= fim)
    {
        meio = (ini + fim) / 2;

        if (strcmp(palavras[meio].palavra, palavra) == 0)
        {
            if (*posicaoBase == 0)
            {
                printf("Ocorrências: %d \n", palavras[meio].quantidade);
            }
            printf("Palavra encontrada na posição: %d\n", palavras[meio].posicoes[*posicaoBase]);
            imprimirPedaço(texto, palavras[meio].posicoes[*posicaoBase]);
            (*posicaoBase)++;
            if (*posicaoBase == palavras[meio].quantidade)
            {
                *posicaoBase = 0;
            }
            return;
        }
        else if (strcmp(palavras[meio].palavra, palavra) < 0)
        {
            ini = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    printf("Palavra não encontrada\n");
}

void imprimirPedaço(char *texto, int posicao)
{
    for (int i = posicao - 20; i <= posicao + 20; i++)
    {
        printf("%c", texto[i]);
    }
    printf("\n");
}

int main()
{
    int opcao;
    char texto[10000];
    char palavra[100];
    int posicaoBase = 0;

    do
    {
        printf("\nSelecione uma opção:\n");
        printf("1. Abrir arquivo e exibir conteúdo\n");
        printf("2. Exibir palavras e posições armazenadas\n");
        printf("3. Força Bruta\n");
        printf("4. Procurar uma palavra no arquivo invertido\n");
        printf("5. Procurar próxima ocorrencia\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            lerArquivo("Historia.txt", texto);
            printf("Conteúdo do arquivo:\n%s\n", texto);
            break;
        case 2:
            ImprimirPalavras();
            break;
        case 3:
            printf("Digite a palavra a ser buscada: ");
            fgets(palavra, 100, stdin);
            palavra[strcspn(palavra, "\n")] = '\0'; // Remove o '\n'
            ForcaBruta(texto, strlen(texto), palavra, strlen(palavra));
            break;
        case 4:
            posicaoBase = 0;
            printf("Digite a palavra a ser buscada: ");
            fgets(palavra, 100, stdin);
            palavra[strcspn(palavra, "\n")] = '\0'; // Remove o '\n'
            printf("\n\n----------------------------------------------\n");
            ProcurarPalavra(palavra, texto, &posicaoBase);
            printf("----------------------------------------------\n\n");
            break;
        case 5:
            if (palavra[0] == '\0')
            {
                printf("Nenhuma palavra foi buscada\n");
                break;
            }
            printf("\n\n----------------------------------------------\n");
            ProcurarPalavra(palavra, texto, &posicaoBase);
            printf("----------------------------------------------\n\n");
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}
