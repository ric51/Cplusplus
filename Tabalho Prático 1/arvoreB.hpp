#include "cabecalho.hpp"
using namespace std;

#ifndef arvoreB_hpp
#define arvoreB_hpp


//ESTRUTURAS PARA ARVORE B DE ORDEM M
typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
	short n;
	Titem r[MM];
	TipoApontador p[MM + 1];
} TipoPagina;

//FUNCOES P/ ARVORE B
void pesquisa_ArvoreB(int chave);
void Inicializa(TipoApontador * );
void Insere(Titem, TipoApontador *, int);
void Ins(Titem, TipoApontador, short *, Titem *,  TipoApontador *, int);
void InsereNaPagina(TipoApontador, Titem , TipoApontador, int);
void Pesquisa_AB(Titem *, TipoApontador, int, int);


#endif