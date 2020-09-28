/*
	ESSE TRABALHO CONSISTE EM APLICAR AS ESTRUTURAS DE DADOS APRENDIDAS EM 
AULA, SENDO ELAS, ALÉM DO PROCESSO DE MANIPULAÇÃO DE ARQUIVOS: ACESSO 
SEQUENCIAL INDEXADO, ARVORE DE PESQUISA, ARVORE B E ARVORE B*.
	SEU PROCESSO SE CONSTITUI DOS PROCESSOS:
		CODIGO GERADOR DE 3 ARQUIVOS DE 2 MILHÕES DE DADOS EM ORDEM CRESCE-
NTE, DESCRESCENTE E ALEATORIA.
		INICIO DO MAIN.
		OPCAO DE ENTRADA DE ACORDO COM O ENUNCIADO:
		<METODO> <QUANTIDADE> <SITUACAO> <CHAVE> <[-P]>
*/


// teste

#include "cabecalho.hpp"
#include "acesso_sequencial.hpp"
#include "arvoreB.hpp"
#include "ArvoreBstar.hpp"
#include "pesquisa_externa.hpp"

using namespace std;

int main (int argc, char *argv[]) {
	
	//DECLARACAO DE VARIAVEIS
	FILE *Arquivo, *T_Execucao;
	
	int metodo, quantidade, situacao, chave, arg = 0;
	
	char* arg_Opcional;
	
	clock_t tempoinicial, tempofinal;
	
	float tempo;
	
	if(argc < 4){
		perror("Numero de argumentos insuficiente\n");
		exit(1);
	}
	
	//VARIAVEIS RECEBEM VALORES DOS ARGUMENTOS
	metodo = atoi(argv[1]); 
	quantidade = atoi(argv[2]);
	situacao = atoi(argv[3]);
	chave = atoi(argv[4]);
	
	//LEITURA DO ARGUMENTO OPCIONAL [-P]
	if(argc > 4) {
		arg_Opcional = argv[5];
		if (strcmp(arg_Opcional, "-P") == 0) {
			arg = 1;
		}
	}
	
	
	if((T_Execucao = fopen("TExecucaoMS.txt","w")) == NULL) {
		cout<<"Erro na abertura do arquivo\n";
		return 0;
	}
	
	if ((Arquivo = fopen("dados.bin","rb")) == NULL) {
		cout << "Erro na abertura do arquivo\n"; 
		return 0;
	}
	
	
	gerador(quantidade, situacao, arg);
	
	
	switch (metodo) {
		case 1:
			//ACESSO SEQUENCIAL INDEXADO.
			if (situacao == 1) {									
				acesso_sequencial(Arquivo, chave, quantidade);	
			}else{
				cout << "ACESSO SEQUENCIAL FUNCIONA APENAS EM ARQUIVOS DE ORDEM CRESCENTE.\n";
			}
			fclose(Arquivo);
		break;
		
		case 2:
			//ARVORE BINARIA ADEQUADA A PSQ EXTERNA.
			//FUNCAO P/ CRIACAO DA ARVORE:
			Arvore_Binaria_Memoria_Externa(chave);

		break;
		
		case 3:
			//ARVORE B DE RDEM M.
			pesquisa_ArvoreB(chave);
		break;
		
		case 4:
			//ARVORE B* 
      TipoPagina k;
      k.n = chave;
      TipoApontador apt = &k;
      Titem h;
      h.chave = chave;
      Item *q = (Item*) &h;
      PesquisaArvoreBstar(q, &apt);
		break;
		
		 //default:
			//cout << "\nMETODO INVALIDO. TENTE NOVAMENTE" << endl;
	}
	
	tempofinal = clock();
	tempo = ( (tempofinal - tempoinicial) * 1000 / CLOCKS_PER_SEC);
	fprintf(T_Execucao, "%.2f \n", tempo);	
	fclose (T_Execucao);
	
	return 0;
}