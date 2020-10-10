#ifndef UTILITARIOSALUNO_H
#define UTILITARIOSALUNO_H
#include "quicksortExterno.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace std;

typedef struct analise Analise;

struct analise
{
    long double tempo;
    long long int comparacao;
    long long int transferenciaL;
    long long int transferenciaE;
};

typedef struct alu Aluno;

struct alu
{
    long int matricula;
    double nota;
    char estado[3];
    char cidade[51];
    char curso[31];
};

Analise *inicializaAnalise();
void imprimeAnalise(Analise *analise1);
void quicksort(Aluno values[], int began, int end, Analise *analise1);
int getMenor(Aluno *regs[], int size);
int geraBinario();
void inverteArquivo(int tam);
void fitas2txt(int n);
void arqBinarioCrescente(int tamanho);
void arqBinarioDecrescente(int tamanho);
void arqBinarioAleatorio();
void toTxt(const char *arq);

#endif 