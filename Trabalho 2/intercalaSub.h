#ifndef INTERCALASUB_H
#define INTERCALASUB_H
#define NFITAS 40
#define TAMMEM NFITAS / 2
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "utilitariosAluno.h"

typedef struct doheap doHeap;
void fitas2txt(int n);
void heapRefaz(doHeap *v, int esq, int dir, Analise *analise1);
void heapConstroi(doHeap *v, int n, Analise *analise1);
bool estaoTodosMarcados(doHeap *vet, int n);
void desmarca(doHeap *vet, int n);
void intercalaSub(Analise *analise1, int TAMDADOS);

#endif //INTERCALASUB