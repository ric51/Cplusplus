#ifndef cabecalho_hpp
#define cabecalho_hpp

//BIBLIOTECAS
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <time.h>
//#include <windows.h>

//DEFINICAO DE CONSTANTES
#define ITENSPAGINA 4
#define MAXTABELA 100
#define MAX 1000
#define M 4
#define MM (M*2)
#define MM2 (M*2)
#define PAGES 200


//ESTRUTURA DE ARQUIVO DE ENTRADA
typedef struct {
	int chave;
	long int dado1;
	char dado2[MAX];
}Titem;

//ESTRUTURA P/ ARVORE BINARIA DE PESQUISA EM MEMORIA EXTERNA
typedef struct {
	int chave;
	long int dado1;
	char dado2[MAX];
	int pDir, pEsq;
}Titem2;

//GERADOR DE ARQUIVOS BINARIOS:
int gerador(int num, int opcao, int arg);

#endif
