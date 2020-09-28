#include "cabecalho.hpp"

using namespace std;

#ifndef acesso_sequencial_hpp
#define acesso_sequencial_hpp

//ESTRUTURA DE INDICES P/ PESQUISA SEQUENCIAL
typedef struct {
	int chave;
	int posicao;
}indice;

//FUNCOES P/ PESQUISA SEQUENCIAL INDEXADA:
int pesquisa_Sequencial (indice tab[], int tam, Titem* item, FILE *arq);
int acesso_sequencial(FILE* arq, int chave, int tamanho);

#endif