#include "quicksortExterno.h"

struct area
{
    Aluno vet[TAM_AREA];
    int tam;
    int topo;
};

void chamaQuick(Analise *analisequick, int quantidade)
{
    FILE *ArqLEs;
    FILE *ArqLi;
    FILE *ArqEi;
    double tempofinal = 0;
    ArqLi = fopen("aleatorio.bin", "r+b");
    if (ArqLi == NULL)
    {
        printf("\n\tErro na Abertura de arquivo\n");
        return;
    }
    ArqEi = fopen("aleatorio.bin", "r+b");
    if (ArqEi == NULL)
    {
        printf("\n\tErro na Abertura de arquivo\n");
        return;
    }
    ArqLEs = fopen("aleatorio.bin", "r+b");
    if (ArqLEs == NULL)
    {
        printf("\n\tErro na Abertura de arquivo\n");
        return;
    }
    rewind(ArqEi);
    rewind(ArqLEs);
    rewind(ArqLi);
    analisequick->tempo = clock();
    QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, quantidade, analisequick);
    tempofinal = clock();
    analisequick->tempo = tempofinal - analisequick->tempo;
    rewind(ArqLi);
    toTxt("aleatorio.bin");
    fflush(ArqLi);
    fflush(ArqEi);
    fflush(ArqLEs);
    fclose(ArqLi);
    fclose(ArqLEs);
    fclose(ArqEi);
    return;
}

void fazVazia(TipoArea *area)
{
    area->tam = TAM_AREA;
    area->topo = -1;
}

void InsereItem(Aluno &UltLido, TipoArea *Area)
{
    int numeroItens = Area->topo + 1;
    int i = 0;
    int k = 0;
    //Insere o item e atualiza o valor do topo
    while ((UltLido.nota > Area->vet[i].nota) && i < numeroItens)
        i++;
    if (i < numeroItens)
    {
        k = numeroItens;
        while (k > i)
        {
            Area->vet[k] = Area->vet[k - 1];
            k--;
        }
        Area->vet[i] = UltLido;
        Area->topo++;
    }
    else
    {
        Area->topo++;
        Area->vet[Area->topo] = UltLido;
    }
}

void RetiraUltimo(TipoArea *Area, Aluno *R)
{
    //Retira o ultimo elemento
    //Atualiza o valor de Topo
    *R = Area->vet[Area->topo--];
}

void RetiraPrimeiro(TipoArea *Area, Aluno *R)
{
    //Retira o primeiro elemento
    //Arreda os elementos do vetor
    //Atualiza valor de Topo
    *R = Area->vet[0];
    for (int i = 0; i <= Area->topo; i++)
        Area->vet[i] = Area->vet[i + 1];
    Area->topo--;
}

void imprimeArea(TipoArea *Area)
{
    for (int i = 0; i <= Area->topo; i++)
        cout << Area->vet[i].nota << " ";
    cout << endl;
}

int ObterNumCelOcupadas(TipoArea *Area)
{
    return Area->topo + 1;
}

void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, Analise *analise1)
{
    int i, j;
    TipoArea Area;
    //Condicao de parada
    if ((Dir - Esq) < 1)
        return;
    fazVazia(&Area);
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, analise1);
    if ((i - Esq) < (Dir - j))
    {
        //ordene primero o subarquivo menor
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, analise1);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, analise1);
    }
    else
    {
        //e depois o subarquivo maior
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, analise1);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, analise1);
    }
}

void EscreveMax(FILE **ArqLEs, Aluno &R, int *Es)
{
    fseek(*ArqLEs, ((*Es - 1) * sizeof(Aluno)), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, Aluno &R, int *Ei)
{
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    (*Ei)++;
}

void RetiraMax(TipoArea *Area, Aluno *R, int *NRArea)
{
    RetiraUltimo(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void RetiraMin(TipoArea *Area, Aluno *R, int *NRArea)
{
    RetiraPrimeiro(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, short *OndeLer)
{
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(UltLido, sizeof(Aluno), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = false;
}

void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, short *OndeLer)
{
    fread(UltLido, sizeof(Aluno), 1, *ArqLi);
    (*Li)++;
    *OndeLer = true;
}

void InserirArea(TipoArea *Area, Aluno *UltLido, int *NRArea)
{
    //Insere UltLido de forma ordenada na Area
    InsereItem(*UltLido, Area);
    *NRArea = ObterNumCelOcupadas(Area);
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea &Area, int Esq, int Dir, int *i, int *j, Analise *analise1)
{
    int Ls = Dir; //Apontador de leitura superior
    int Es = Dir; //Apontador de escrita superior
    int Li = Esq; //Apontador de leitura inferior
    int Ei = Esq; //Apontador de escrita inferior
    int NRArea = 0;
    double Linf = INT_MIN; //Limite inferior
    double Lsup = INT_MAX; //Limite superior
    short OndeLer = true;
    Aluno UltLido;
    Aluno R;
    fseek(*ArqLi, ((Li - 1) * sizeof(Aluno)), SEEK_SET);
    fseek(*ArqEi, ((Ei - 1) * sizeof(Aluno)), SEEK_SET);
    //inicializa i e j com posicoes invalidas
    *i = Esq - 1;
    *j = Dir + 1;
    while (Li <= Ls)
    {
        // Faz a leitura alternada do arquivo (fim e inicio), inserindo no pivo, ate sobrar uma
        // posicao vazia na area
        if (NRArea < TAM_AREA - 1)
        {
            if (OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            analise1->transferenciaL++;
            InserirArea(&Area, &UltLido, &NRArea);
            continue;
        }
        // Ate aqui, o pivo tem uma posicao vazia, e eh preciso ler mais um elemento.
        // Os dois primeiros if's garantem que o ponteiro de escrita nao ultrapasse
        // os ponteiros de leitura
        if (Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei)
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        else if (OndeLer)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        analise1->transferenciaL++;
        analise1->comparacao++;
        if (UltLido.nota > Lsup)
        {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            analise1->transferenciaE++;
            continue;
        }
        analise1->comparacao++;
        if (UltLido.nota < Linf)
        {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            analise1->transferenciaE++;
            continue;
        }
        InserirArea(&Area, &UltLido, &NRArea);
        //PIVO CHEIO - escolhe o lado (esquerda ou direita) que tem mais elementos
        //e escreve no arquivo ou o menor elemento do pivo ou o maior
        analise1->comparacao++;
        if ((Ei - Esq) < (Dir - Es))
        {
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        }
        else
        {
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }
        analise1->transferenciaE++;
    }
    while (Ei <= Es)
    {
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
        analise1->transferenciaE++;
    }
}