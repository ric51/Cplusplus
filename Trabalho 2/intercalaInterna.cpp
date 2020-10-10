#include "intercalaInterna.h"
#include <cstdio>
#include <cstdlib>


void intercalaInterna(Analise *analise1, int TAMDADOS)
{
    double final;
    analise1->tempo = clock();
    Aluno vet[TAMMEM];        // armazena os registros (limitado pelo tamanho da memoria)
    Aluno *pVet[TAMMEM];      // armazena poteiro para cada registro em vet
    FILE *fitas[NFITAS];      // armazena os ponteiros dos arquivos das fitas
    int lidos[NFITAS / 2];    // armazeno o numeros e itens lidos de cada fita de entrada
    FILE *dados;              // arquivo com os registros a serem ordenados
    int fEntrada = 0;         // indice da primeira fita de entrada
    int fSaida = NFITAS / 2;  // indice da primeira fita de saida
    int fAtualE = 0;          // indice da fita de entrada em uso
    int fAtualS = NFITAS / 2; // indice da fita de saida em uso
    int nNaMemoria;           // numero de registros na memoria (vet)
    int nFEAtivas = 0;        // numero de fitas de entrada ativas
    int nFEAtivas_Prox = 0;   // numero de fitas de entrada ativas apos a proxima inversao
    char nome[30], aux[10];
    int inversoes = 0; // numero de vezes que as fitas de entrada e saida foram invertidas
    int tam_bloco;     // tamanho dos blocos de registros nas fitas de entrada
    int menor;         // posicao do do registro de menor chave
    int cont = 0;
    if ((dados = fopen("aleatorio.bin", "rb")) == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        exit(-1);
    }
    // abrir as fitas
    for (int i = 1; i <= NFITAS; i++)
    {
        strcpy(nome, "fitas-");
        itoa(i, aux, 10);
        strcat(nome, aux);
        strcat(nome, ".bin");
        if ((fitas[i - 1] = fopen(nome, "w+b")) == NULL)
        {
            printf("Erro ao abrir arquivo.\n");
            exit(-1);
        }
    }
    //Faz a primeira leitura para a memoria principal, pega n alunos que serÃ£o
    //ordenados para criaÃ§Ã£o de blocos nas fitas de entrada
    while (cont < TAMDADOS && (nNaMemoria = fread(vet, sizeof(Aluno), TAMMEM, dados)) > 0)
    {
        analise1->transferenciaL++;
        cont += TAMMEM;
        quicksort(vet, 0, nNaMemoria, analise1);
        fwrite(vet, sizeof(Aluno), nNaMemoria, fitas[fAtualE]);
        analise1->transferenciaE++;
        fflush(fitas[fAtualE]);
        // Atualiza a fita que esta recebendo os registros
        fAtualE = (fAtualE + 1 >= fEntrada + NFITAS / 2) ? fEntrada : fAtualE + 1;
        // Atualiza o numero de fitas de entrada ativas
        if (nFEAtivas < NFITAS / 2)
            nFEAtivas++;
    }
    fitas2txt(NFITAS);
    fclose(dados);
    // move os ponteiros dos arquivos para a primeira posicao
    // para poder fazer a leitura
    for (int i = 0; i < nFEAtivas; i++)
        fseek(fitas[fEntrada + i], 0, SEEK_SET);
    while (nFEAtivas > 1)
    {
        short processoTerminou = 0;
        nNaMemoria = 0;
        tam_bloco = pow(TAMMEM, inversoes + 1);
        nFEAtivas_Prox = 0;
        while (!processoTerminou)
        {
            // reseta os contadores de registros lidos
            for (int i = 0; i < nFEAtivas; i++)
                lidos[i] = 0;
            // le os primeiros registros das fitas de entrada e salva no vetor em memoria principal
            for (int i = 0; i < nFEAtivas; i++)
                if (fread(vet + i, sizeof(Aluno), 1, fitas[fEntrada + i]) == 1)
                {
                    analise1->transferenciaL++;
                    nNaMemoria++;
                    pVet[i] = vet + i;
                    lidos[i]++;
                }
            // se nenhum registro foi lido entao termina o processo
            if (nNaMemoria == 0)
            {
                processoTerminou = 1;
                break;
            }
            while (nNaMemoria > 0)
            {
                analise1->transferenciaE++;
                // obtem o indice do registro de menor chave
                menor = getMenor(pVet, TAMMEM);
                // calcula a fita que sera lido o proximo registro
                fAtualE = fEntrada + menor;
                // escreve o registro de menor chave na fita de saida
                fwrite(pVet[menor], sizeof(Aluno), 1, fitas[fAtualS]);
                fflush(fitas[fAtualS]);
                fitas2txt(NFITAS);
                // le um novo registro se for possivel
                if (lidos[fAtualE - fEntrada] < tam_bloco && fread(pVet[menor], sizeof(Aluno), 1, fitas[fAtualE]) == 1)
                {
                    analise1->transferenciaL++;
                    lidos[fAtualE - fEntrada]++; // numero de registros lidos da fita atual aumentou
                }
                else
                {
                    pVet[menor] = NULL; // nao foi lido um novo registro, logo a posicao fica vazia
                    nNaMemoria--;       // um item foi escrito na fita de saida mas outro nao foi lido
                }
            }
            // intercalacao de uma coluna de blocos terminou
            // atualiza a fita de saida (volta para a primeira caso a atual for a ultima)
            fAtualS = (fAtualS + 1 >= fSaida + NFITAS / 2) ? fSaida : fAtualS + 1;
            // Atualiza o numero de fitas de entrada que estarao ativas apos a proxima inversao
            if (nFEAtivas_Prox < NFITAS / 2)
                nFEAtivas_Prox++;
            fitas2txt(NFITAS);
        }
        // reabrir as fitas de entrada para que na proxima passada
        // elas estejam vazias como fitas de saida
        for (int i = 1; i <= nFEAtivas; i++)
        {
            strcpy(nome, "fitas-");
            itoa(i + fEntrada, aux, 10);
            strcat(nome, aux);
            strcat(nome, ".bin");
            if ((fitas[fEntrada + i - 1] = fopen(nome, "w+b")) == NULL)
            {
                printf("Erro ao abrir arquivo.\n");
                exit(-1);
            }
        }
        nFEAtivas = nFEAtivas_Prox; // numero de fitas de saida que receberam itens
        // inverte fitas de entrada e saida
        int faux = fEntrada;
        fEntrada = fSaida;
        fSaida = faux;
        // reseta a fita de saida
        fAtualS = fSaida;
        // move os ponteiros dos arquivos para a primeira posicao
        // para poder fazer a leitura
        for (int i = 0; i < nFEAtivas; i++)
            fseek(fitas[fEntrada + i], 0, SEEK_SET);
        inversoes++;
    }
    for (int i = 0; i < NFITAS; i++)
        fclose(fitas[i]);
    printf("\n\tUltima fita: %d\n", fEntrada + 1);
    final = clock();
    analise1->tempo = final - analise1->tempo;
}
