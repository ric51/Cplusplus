#include "utilitariosAluno.h"

Analise *inicializaAnalise()
{
    Analise *analise1;
    analise1 = (Analise *)malloc(sizeof(Analise));
    analise1->tempo = 0.0;
    analise1->comparacao = 0;
    analise1->transferenciaL = 0;
    analise1->transferenciaE = 0;
    return analise1;
}

void imprimeAnalise(Analise *analise1)
{
    analise1->tempo = ((double)analise1->tempo) / (CLOCKS_PER_SEC);
    cout << "\tNumero de transferencias de leitura : " << analise1->transferenciaL << endl;
    cout << "\tNumero de transferencias de escrita : " << analise1->transferenciaE << endl;
    cout << "\tNumero de comparacoes :               " << analise1->comparacao << endl;
    cout << "\tTempo de execucao :                   " << fixed << setprecision(3) << analise1->tempo << " segundos " << endl;
}

void quicksort(Aluno valores[], int inicio, int fim, Analise *analise1)
{
    int i, j;
    Aluno aux;
    double pivo;
    i = inicio;
    j = fim - 1;
    pivo = valores[(inicio + fim) / 2].nota;
    while (i <= j)
    {
        analise1->comparacao++;
        while (valores[i].nota < pivo && i < fim)
        {
            i++;
            analise1->comparacao++;
        }
        analise1->comparacao++;
        while (valores[j].nota > pivo && j > inicio)
        {
            j--;
            analise1->comparacao++;
        }
        if (i <= j)
        {
            aux = valores[i];
            valores[i] = valores[j];
            valores[j] = aux;
            i++;
            j--;
        }
    }
    if (j > inicio)
        quicksort(valores, inicio, j + 1, analise1);
    if (i < fim)
        quicksort(valores, i, fim, analise1);
}

int getMenor(Aluno *regs[], int size)
{
    int menor = 0;
    // obtem a primeira posicao nao vazia
    while (regs[menor] == NULL)
        menor++;
    // compara as chaves do registros para encontrar o menor
    for (int i = menor + 1; i < size; i++)
        if (regs[i] != NULL && regs[i]->nota < regs[menor]->nota)
            menor = i;
    return menor;
}

int geraBinario()
{
    FILE *provao = fopen("PROVAO.TXT", "r");
    FILE *aleatorio = fopen("aleatorio.bin", "w+b");
    if (provao == NULL)
    {
        cout << "\n\tErro na Abertura do arquivo PROVAO.txt\n";
        fclose(aleatorio);
        return 0;
    }
    if (aleatorio == NULL)
    {
        cout << "\n\tErro na Abertura do arquivo aleatorio.bin\n";
        fclose(provao);
        return 0;
    }
    Aluno alu;
    while (fscanf(provao, "%ld %lf %3s %50[A-Z '-] %30[A-Z ]\n", &alu.matricula, &alu.nota, alu.estado, alu.cidade, alu.curso) != EOF)
        fwrite(&alu, sizeof(Aluno), 1, aleatorio);
    fclose(provao);
    fclose(aleatorio);
    return 1;
}

void inverteArquivo(int tam)
{
    Aluno iniAluno;
    Aluno fimAluno;
    FILE *inicioLeARQ = fopen("aleatorio.bin", "r+b");
    FILE *fimLeARQ = fopen("aleatorio.bin", "r+b");
    FILE *inicioEscreveARQ = fopen("aleatorio.bin", "r+b");
    FILE *fimEscreveARQ = fopen("aleatorio.bin", "r+b");
    //ponteiro de escrita do registro do inicio do arquivo no final
    int iniEsc = tam;
    //ponteiro de leitura no inicio do arquivo
    int iniLei = 1;
    //ponteiro de leitura no final do arquivo
    int fimLei = tam;
    //ponteiro de escrita do registro do final do arquivo no inicio
    int fimEsc = 1;
    if (inicioLeARQ == NULL || fimLeARQ == NULL || inicioEscreveARQ == NULL || fimEscreveARQ == NULL)
        return;
    //ponteiro de leitura inicial no inicio do arquivo
    rewind(inicioLeARQ);
    //ponteiro de leitura final no final do arquivo
    fseek(fimLeARQ, (tam * sizeof(Aluno)), SEEK_SET);
    //ponteiro de escrita inicial no final do arquivo
    fseek(inicioEscreveARQ, (tam * sizeof(Aluno)), SEEK_SET);
    //ponteiro de escrita final no inicio do arquivo
    rewind(fimEscreveARQ);
    for (int i = 0; i <= (tam / 2); i++)
    {
        //lê o primeiro registro
        fseek(inicioLeARQ, (iniLei - 1) * sizeof(Aluno), SEEK_SET);
        fread(&iniAluno, sizeof(Aluno), 1, inicioLeARQ);
        iniLei++;
        //lê o ultimo registro
        fseek(fimLeARQ, (fimLei - 1) * sizeof(Aluno), SEEK_SET);
        fread(&fimAluno, sizeof(Aluno), 1, fimLeARQ);
        fimLei--;
        //faz troca
        //escreve o primeiro registro no final
        fseek(inicioEscreveARQ, (iniEsc - 1) * sizeof(Aluno), SEEK_SET);
        fwrite(&iniAluno, sizeof(Aluno), 1, inicioEscreveARQ);
        iniEsc--;
        //escreve o ultimo registro no inicio
        fseek(fimEscreveARQ, (fimEsc - 1) * sizeof(Aluno), SEEK_SET);
        fwrite(&fimAluno, sizeof(Aluno), 1, fimEscreveARQ);
        fimEsc++;
    }
}

void fitas2txt(int n)
{
    char nome[30], aux[10];
    for (int i = 1; i <= n; i++)
    {
        strcpy(nome, "fitas-");
        itoa(i, aux, 10);
        strcat(nome, aux);
        strcat(nome, ".bin");
    }
}

void arqBinarioCrescente(int tamanho)
{
    FILE *ArqLEs;
    FILE *ArqLi;
    FILE *ArqEi;
    Analise *analisequick;
    analisequick = inicializaAnalise();
    ArqLi = fopen("aleatorio.bin", "r+b");
    if (ArqLi == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    ArqEi = fopen("aleatorio.bin", "r+b");
    if (ArqEi == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    ArqLEs = fopen("aleatorio.bin", "r+b");
    if (ArqLEs == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    rewind(ArqEi);
    rewind(ArqLEs);
    rewind(ArqLi);
    cout << "\n\tArquivo ordenado ASCENDENTEMENTE pelas notas\n"<< endl;
    //ordena antes de chamar o processo
    QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, tamanho, analisequick);
    rewind(ArqEi);
    rewind(ArqLEs);
    rewind(ArqLi);
}

void arqBinarioDecrescente(int tamanho)
{
    FILE *ArqLEs;
    FILE *ArqLi;
    FILE *ArqEi;
    Analise *analisequick;
    analisequick = inicializaAnalise();
    ArqLi = fopen("aleatorio.bin", "r+b");
    if (ArqLi == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    ArqEi = fopen("aleatorio.bin", "r+b");
    if (ArqEi == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    ArqLEs = fopen("aleatorio.bin", "r+b");
    if (ArqLEs == NULL)
    {
        printf("Arquivo nao pode ser aberto");
        return;
    }
    rewind(ArqEi);
    rewind(ArqLEs);
    rewind(ArqLi);
    cout << "\n\tArquivo ordenado DECRESCENTEMENTE pelas notas\n" << endl;
    //ordena e inverte o arquivo antes de chamar o processo
    QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, tamanho, analisequick);
    inverteArquivo(tamanho);
}

void arqBinarioAleatorio(){
    cout << "\n\tArquivo desordenado ALEATORIAMENTE por notas\n" << endl;
}

void toTxt(const char *arq)
{
    char *file = new char[strlen(arq) + 4]; // +4 para '.txt'
    strcpy(file, arq);
    FILE *bin = fopen(arq, "rb");
    FILE *txt = fopen(strcat(file, ".txt"), "w+");
    if (bin == NULL || txt == NULL)
        return;
    Aluno alu;
    while (fread(&alu, sizeof(Aluno), 1, bin) == 1)
        fprintf(txt, "%08ld %04.1f %3s %50s %30s\n", alu.matricula, alu.nota, alu.estado, alu.cidade, alu.curso);
    fclose(bin);
    fclose(txt);
}