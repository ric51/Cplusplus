
#include <iostream>
#include <cstdlib>
#include "utilitariosAluno.h"
#include "intercalaInterna.h"
#include "intercalaSub.h"
#include "quicksortExterno.h"

using namespace std;

int main(int argc, char *argv[])
{
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);

    Analise *analise1;
    analise1 = inicializaAnalise();

    if (quantidade > 471705 || quantidade < 1)
    {
        cout << "\n\tValor invalido" << endl;
        return 0;
    }

    //PRÉ-PROCESSAMENTO
    if (geraBinario() == 0)
    {
        return 0;
    }

    if (situacao == 1)
        arqBinarioCrescente(quantidade);
    else if (situacao == 2)
        arqBinarioDecrescente(quantidade);
    else
        arqBinarioAleatorio();

    switch (metodo)
    {
    case 1:
        cout << "\tIntercalacao balanceada de varios caminhos com ordenacao interna\n " << endl;
        intercalaInterna(analise1, quantidade);
        imprimeAnalise(analise1);
        break;
    case 2:
        cout << "\tIntercalacao balanceada de varios caminhos atraves de selecao por substituicao (2f fitas)\n" << endl;
        intercalaSub(analise1, quantidade);
        imprimeAnalise(analise1);
        break;
    case 3:
        cout << "\tQuicksort externo\n"<< endl;
        chamaQuick(analise1, quantidade);
        imprimeAnalise(analise1);
        break;
    }
    return 0;
}