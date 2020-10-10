#ifndef QUICKSORTEXTERNO_H
#define QUICKSORTEXTERNO_H
#include <climits>
#include <iomanip>
#include "utilitariosAluno.h"
#define TAM_AREA 10

typedef struct area TipoArea;
typedef struct analise Analise;
typedef struct alu Aluno;
void chamaQuick(Analise *analise1, int quantidade);

//OPERAÇÕES DA AREA (PIVO)
void fazVazia(TipoArea *area);
void InsereItem(Aluno &UltLido, TipoArea *Area);
void RetiraUltimo(TipoArea *Area, Aluno *R);
void RetiraPrimeiro(TipoArea *Area, Aluno *R);
int ObterNumCelOcupadas(TipoArea *Area);
void imprimeArea(TipoArea *Area);

//OPERAÇÕES DO QUICKSORT
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, Analise *analise1);
void EscreveMax(FILE **ArqLEs, Aluno &R, int *Es);
void EscreveMin(FILE **ArqEi, Aluno &R, int *Ei);
void RetiraMax(TipoArea *Area, Aluno *R, int *NRArea);
void RetiraMin(TipoArea *Area, Aluno *R, int *NRArea);
void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, short *OndeLer);
void InserirArea(TipoArea *Area, Aluno *UltiLido, int *NRArea);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea &Area, int Esq, int Dir, int *i, int *j, Analise *analise1);
#endif //QUICKSORTEXTERNO_H