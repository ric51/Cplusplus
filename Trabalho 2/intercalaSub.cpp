#include "intercalaSub.h"

struct doheap
{
    Aluno alu;
    bool marc;
    int fita;
};

void heapRefaz(doHeap *v, int esq, int dir, Analise *analise1)
{
    int i = esq;
    int j = i * 2 + 1; // j = primeiro filho de i
    doHeap aux = v[i]; // aux = no i (pai de j)
    while (j <= dir)
    {
        analise1->comparacao += 2;
        if (j < dir && v[j].alu.nota > v[j + 1].alu.nota)
            j++; // j recebe o outro filho de i
        if (aux.alu.nota <= v[j].alu.nota)
            break; // heap foi refeito corretamente
        v[i] = v[j];
        i = j;
        j = i * 2 + 1; // j = primeiro filho de i
    }
    v[i] = aux;
}

void heapConstroi(doHeap *v, int n, Analise *analise1)
{
    int esq;
    esq = (n / 2) - 1; // esq = nó anterior ao primeiro nó folha do heap
    while (esq >= 0)
    {
        heapRefaz(v, esq, n - 1, analise1);
        esq--;
    }
}

bool estaoTodosMarcados(doHeap *vet, int n)
{
    for (int i = 0; i < n; i++)
        if (!vet[i].marc)
            return false;
    return true;
}

void desmarca(doHeap *vet, int n)
{
    for (int i = 0; i < n; i++)
    {
        vet[i].marc = false;
        vet[i].alu.nota -= 200;
    }
}

void intercalaSub(Analise *analise1, int TAMDADOS)
{
    double final;
    analise1->tempo = clock();
    doHeap vetAux[TAMMEM];    // vetor de registros + flags
    FILE *fitas[NFITAS];      // armazena os ponteiros dos arquivos das fitas
    FILE *dados;              // arquivo com os registros a serem ordenados
    int fEntrada = 0;         // indice da primeira fita de entrada
    int fSaida = NFITAS / 2;  // indice da primeira fita de saida
    int fAtualE = 0;          // indice da fita de entrada em uso
    int fAtualS = NFITAS / 2; // indice da fita de saida em uso
    int nNaMemoria;           // numero de registros na memoria (vet)
    int nFEAtivas = 1;        // numero de fitas de entrada ativas
    int nFEAtivas_Prox = 0;   // numero de fitas de entrada ativas apos a proxima inversao
    char nome[30], aux[10];
    int cont = 0;   // contador para auxiliar o limite de registros a serem lidos
    int inicio = 0; // primeira posicao de vetAux com registro valido
    // armazena a nota do ultimo registro escrito durante a construcao dos primeirosblocos
    double notaSaiu;
    Aluno fimDoBloco; // registro auxilar para indicar o fim de um bloco
    fimDoBloco.nota = -1;
    fimDoBloco.matricula = 0;
    fimDoBloco.cidade[0] = '\0';
    fimDoBloco.estado[0] = '\0';
    fimDoBloco.curso[0] = '\0';
    // Abre o arquivo com os registros
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
    // CRIACAO DOS PRIMEIROS BLOCOS NAS FITAS DE ENTRADA
    //lê TAMMEM itens para construir o heap pela primeira vez
    nNaMemoria = 0;
    while (cont < TAMDADOS && nNaMemoria < TAMMEM &&
           fread(&(vetAux[nNaMemoria].alu), sizeof(Aluno), 1, dados) == 1)
    {
        vetAux[nNaMemoria].marc = false;
        nNaMemoria++;
        cont++;
        analise1->transferenciaL++;
    }
    heapConstroi(vetAux, nNaMemoria, analise1);
    // escreve o registro de menor chave na fita
    notaSaiu = vetAux[0].alu.nota;
    fwrite(&vetAux[0].alu, sizeof(Aluno), 1, fitas[fAtualE]);
    analise1->transferenciaE++;
    while (inicio < nNaMemoria)
    {
        int lido = 0;
        if (cont < TAMDADOS && (lido = fread(&(vetAux[0].alu), sizeof(Aluno), 1, dados)) == 1)
        {
            analise1->transferenciaL++;
            cont++;
            // Se a chave do registro lido for menor que a chave do último registro escrito,
            // então ele é marcado
            if (vetAux[inicio].alu.nota < notaSaiu)
            {
                vetAux[inicio].marc = true;
                vetAux[inicio].alu.nota += 200;
            }
        }
        else if (inicio < nNaMemoria - 1)
        {
            inicio++;
            heapConstroi(vetAux + inicio, nNaMemoria - inicio, analise1);
        }
        else
            break;
        if (estaoTodosMarcados(vetAux + inicio, nNaMemoria - inicio))
        {
            desmarca(vetAux, nNaMemoria);
            //escreve um registro a mais para marcar o fim do bloco
            fwrite(&fimDoBloco, sizeof(Aluno), 1, fitas[fAtualE]);
            analise1->transferenciaE++;
            // Atualiza a fita que esta recebendo os registros
            fAtualE = (fAtualE + 1 >= fEntrada + NFITAS / 2) ? fEntrada : fAtualE + 1;
            // Atualiza o numero de fitas de entrada ativas
            if (nFEAtivas < NFITAS / 2)
                nFEAtivas++;
        }
        heapRefaz(vetAux + inicio, 0, nNaMemoria - inicio - 1, analise1);
        notaSaiu = vetAux[inicio].alu.nota;
        fwrite(&vetAux[inicio].alu, sizeof(Aluno), 1, fitas[fAtualE]);
        analise1->transferenciaE++;
    }
    fclose(dados);
    // move os ponteiros dos arquivos para a primeira posicao
    // para poder fazer a leitura
    for (int i = 0; i < nFEAtivas; i++)
        fseek(fitas[fEntrada + i], 0, SEEK_SET);
    // INTERCALAÇÃO
    while (nFEAtivas > 1)
    {
        short processoTerminou = 0;
        nFEAtivas_Prox = 0;
        while (!processoTerminou)
        {
            nNaMemoria = 0;
            inicio = 0;
            // le os primeiros registros das fitas de entrada e salva no vetor em memoria principal
            for (int i = 0; i < nFEAtivas; i++)
                if (fread(&(vetAux[i].alu), sizeof(Aluno), 1, fitas[fEntrada + i]) == 1)
                {
                    vetAux[i].fita = fEntrada + i;
                    nNaMemoria++;
                    analise1->transferenciaL++;
                }
            // se nenhum registro foi lido entao termina o processo
            if (nNaMemoria == 0)
            {
                processoTerminou = 1;
                break;
            }
            heapConstroi(vetAux + inicio, nNaMemoria - inicio, analise1);
            while (inicio < nNaMemoria)
            {
                // calcula a fita que sera lido o proximo registro
                fAtualE = vetAux[inicio].fita;
                // escreve o registro de menor chave na fita de saida
                fwrite(&(vetAux[inicio].alu), sizeof(Aluno), 1, fitas[fAtualS]);
                analise1->transferenciaE++;
                // le um novo registro se for possivel
                if (fread(&(vetAux[inicio].alu), sizeof(Aluno), 1, fitas[fAtualE]) == 1)
                {
                    analise1->transferenciaL++;
                    // Verifica se o registro lido é o marcador de fim de bloco
                    if (vetAux[inicio].alu.nota == -1)
                    {
                        fseek(fitas[fAtualE], -sizeof(Aluno), SEEK_CUR);
                        inicio++;
                        heapConstroi(vetAux + inicio, nNaMemoria - inicio, analise1);
                    }
                    else
                        vetAux[inicio].fita = fAtualE;
                }
                else
                    inicio++; // um item foi escrito na fita de saida mas outro nao foi lido
                heapRefaz(vetAux + inicio, 0, nNaMemoria - 1 - inicio, analise1);
            }
            // intercalacao de uma coluna de blocos terminou
            // atualiza a fita de saida (volta para a primeira caso a atual for a ultima)
            fwrite(&fimDoBloco, sizeof(Aluno), 1, fitas[fAtualS]);
            analise1->transferenciaE++;
            fAtualS = (fAtualS + 1 >= fSaida + NFITAS / 2) ? fSaida : fAtualS + 1;
            // O próximo registro de cada uma das fitas de entrada é o indicador
            // de fim de bloco, então faz um seek para os poteiros apontarem
            // para o primeiros registro do proximos bloco
            for (int i = fEntrada; i < fEntrada + nFEAtivas; i++)
                fseek(fitas[i], sizeof(Aluno), SEEK_CUR);
            // Atualiza o numero de fitas de entrada que estarao ativas apos a proximainversao
            if (nFEAtivas_Prox < NFITAS / 2)
                nFEAtivas_Prox++;
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
    }
    for (int i = 0; i < NFITAS; i++)
        fclose(fitas[i]);
    strcpy(nome, "fitas-");
    itoa(fEntrada + 1, aux, 10);
    strcat(nome, aux);
    strcat(nome, ".bin");
    toTxt(nome);
    printf("\n\tUltima fita: %d\n", fEntrada + 1);
    final = clock();
    analise1->tempo = final - analise1->tempo;
}