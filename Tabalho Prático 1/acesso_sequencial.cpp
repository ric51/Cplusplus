#include "cabecalho.hpp"
#include "acesso_sequencial.hpp"

using namespace std;

int pesquisa_Sequencial (indice tab[], int tam, Titem* item, FILE *arq) {
	cout << "\nok\n";
	Titem pagina[ITENSPAGINA];
	int i, quantitens, comparacoes = 0;
	long desloc;

	// procura pela página onde o item pode se encontrar
	i = 0;
	
	while (i < tam && tab[i].chave <= item->chave) {
		i++;
	}
	//cout << "\ni ok\n";

	// caso a chave desejada seja menor que a 1a chave, o item
	// não existe no arquivo
	if (i == 0) {
		return 0;
	}else {
		// a ultima página pode não estar completa
		if (i < tam) {
			quantitens = ITENSPAGINA;
		}else{
			fseek (arq, 0, SEEK_END);
			quantitens = (ftell(arq)/sizeof(Titem))%ITENSPAGINA;
		}
		
		// lê a página desejada do arquivo
		desloc = (i-1) * ITENSPAGINA*sizeof(Titem);
		fseek (arq, desloc, SEEK_SET);
		fread (&pagina, sizeof(Titem), quantitens, arq);
	
		comparacoes = i;
	
		// pesquisa sequencial na página lida
		for (i=0; i < quantitens; i++) {
			if (pagina[i].chave == item->chave) {
				*item = pagina[i]; 
				return 1;
			}
		}
		return 0;
	}
}

int acesso_sequencial(FILE* arq, int chave, int tamanho) {
	//QUANTIDADE DE ACESSOS A MEMORIA
	int acessos = 0;
	
	
	indice *tabela;
	tabela = new indice[tamanho/ITENSPAGINA];
	
	Titem x; 
	int pos = 0, cont = 0;

	// gera a tabela de índice das páginas
	while (fread(&x, sizeof(x), 1, arq) == 1) {
		cont++;
		if (cont%ITENSPAGINA == 1) {
			tabela[pos].chave = x.chave;
			tabela[pos].posicao = pos+1;
			pos++;
		}
	}
	
	x.chave = chave;
	
	// ativa a função de pesquisa
	if (pesquisa_Sequencial (tabela, pos, &x, arq)) {
		printf ("Chave codigo (%d) foi localizado", x.chave);
		cout << "DADO 1: " << x.dado1 << endl;
		cout << "DADO 2: ";
		for (int c = 0; c < MAX; c++) {
			cout << x.dado2[c];
		}
		cout << endl;
	}else{
		printf ("Chave de codigo (%d) nao foi localizado",x.chave);
	}
	return 0;
}
	