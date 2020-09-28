#ifndef ArvoreBstar_hpp
#define ArvoreBstar_hpp

#include "cabecalho.hpp"
#include <iostream>
#include <ctime>
#include <cstring>
#define f(inicio,fim) for(int i =inicio ; i<fim; i++)
using namespace std;
//#define MM 4
//#define MM2 6


typedef struct tipoitem Item;
typedef enum {Interna, Externa} TipoIntExt;
typedef struct TipoPagina TipoPagina;
typedef struct TipoPagina* TipoApontador;
typedef struct tipoindice Indice;
typedef struct arvore Arvore;

TipoApontador geraArvoreBstar();
void InsereNaPagina(TipoApontador Apontador, int Registro, TipoApontador ApontadorDireita);
void Ins(Item *Registro, TipoApontador Apontador, short *Cresceu, Item *RegRetorno, TipoApontador* ApRetorno, short *Cresceu_no);
void Insere(Item *Registro, TipoApontador* Apontador);
bool InserenaFolha(Item *reg, TipoApontador nova_pagina);
void ImprimeBSTAR(TipoApontador arvore);
bool PesquisaArvoreBstar(Item *x, TipoApontador *Ap);


#endif //TP1_ARVOREBSTAR_H
