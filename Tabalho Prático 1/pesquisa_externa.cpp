#include "cabecalho.hpp"
#include "pesquisa_externa.hpp"

using namespace std;

int Arvore_Binaria_Memoria_Externa(int chave){
	
	//DECLARACAO DE VARIAVEIS
	Titem registro;
	Titem2 registro2, reg_Escrita;
	FILE *Arq, *arvore;
	int tamanho, acessos = 0;
	bool regulador = false;
	long deslocamento_Reverso = sizeof(registro2) * -1;
	
	//CONSTRUCAO DE ARQUIVOS DE ENTRADA E DE SAIDA
	if((Arq = fopen("dados.bin", "rb")) == NULL) {
		cout<<"Erro na abertura do arquivo binario\n";
		return 0;
	}
	
	//CRIANDO A ARVORE BINARIA DE PESQUISA EM MEMORIA EXTERNA
	while(fread(&registro, sizeof(registro), 1, Arq)  == 1){
		
		//CRIANDO O ARQUIVO DE ARVORE BINARIA
		if(regulador == false){
			if((arvore = fopen("Arvore_Binaria.bin", "wb")) == NULL) {
				cout<<"Erro na abertura do arquivo de saida\n";
				return 0;
			}
		}
		
		//INICIANDO O BLOCO DE INSCRICAO COM PONTEIROS NULOS
		reg_Escrita.pEsq = -1;
		reg_Escrita.pDir = -1;
		reg_Escrita.chave = registro.chave;
		
		for (int j = 0; j < 500; j++) {
			reg_Escrita.dado2[j] = registro.dado2[j];
		}
		
		//SE O ARQUIVO ESTIVER VAZIO OS DADOS SERAO ESCRITOS, CASO CONTRARIO OS REGISTROS EXISTENTES SERAO LIDOS E COMPARADOS COM O REGISTRO A SER ESCRITO
		
		if(ftell(arvore) == 0){
			fwrite(&reg_Escrita, sizeof(reg_Escrita), 1, arvore);
			fclose(arvore);
			
			//CONCLUIDO O PROCESSO DE INSERCAO
			regulador = true;
		}else{
			
			//ABRINDO O ARQUIVO ARVORE BINARIA EM MODO DE LEITURA E ESCRITA P/ DEFINIR O DESLOCAMENTO DOS PONTEIROS DURANTE A PESQUISA
			if((arvore = fopen("Arvore_Binaria.bin", "r+b")) == NULL) {
				cout<<"Erro na abertura do arquivo de saida\n";
				return 0;
			}
			acessos++;
			
			//INICIANDO A VARREDURA DA ARVORE ANTES DA ADICAO DE DADO
			while(fread(&registro2, sizeof(registro2), 1, arvore) == 1){
				
				//CONDICIONAL P/ PÓNTEIRO DA ESQUERDA
				if (reg_Escrita.chave < registro2.chave && registro2.pEsq == -1) {
					tamanho = ftell(arvore);
					registro2.pEsq = tamanho;
					fseek(arvore, deslocamento_Reverso, SEEK_CUR);
					//DESLOCAMENTO DO PONTEIRO PARA O REGISTRO A SER ATUALIZADO
					
					fwrite(&registro2, sizeof(registro2), 1, arvore);
					//ATUALIZANDO O PONTEIRO PARA ENCONTRAR O DADO
					
					fseek(arvore, 0, SEEK_END);
					//DESLOCANDO O PONTEIRO P/ FINAL DO ARQUIVO ONDE SERA INSERIDO O DADO
					
					fwrite(&reg_Escrita, sizeof(reg_Escrita), 1, arvore);
					//ESCRITA NO FINAL DO ARQUIVO;
					
					rewind(arvore);
					//DESLOCAMENTO P/ INICIO DO ARQUIVO
					
				//CONDICIONAL P/ PONTEIRO DA DIREITA	
				}else if (reg_Escrita.chave > registro2.chave && registro2.pDir == -1) {
					tamanho = ftell(arvore);
					registro2.pDir = tamanho;
					fseek(arvore, deslocamento_Reverso, SEEK_CUR);
					//DESLOCAMENTO DO PONTEIRO PARA O REGISTRO A SER ATUALIZADO
					
					fwrite(&registro2, sizeof(registro2), 1, arvore);
					//ATUALIZANDO O PONTEIRO PARA ENCONTRAR O DADO
					
					fseek(arvore, 0, SEEK_END);
					//DESLOCANDO O PONTEIRO P/ FINAL DO ARQUIVO ONDE SERA INSERIDO O DADO
					
					fwrite(&reg_Escrita, sizeof(reg_Escrita), 1, arvore);
					//ESCRITA NO FINAL DO ARQUIVO;
					
					rewind(arvore);
					//DESLOCAMENTO P/ INICIO DO ARQUIVO
				}
			}
			fclose(arvore);
		}
	}
		pesquisa(chave);
		cout<<"ACESSOS: "<<acessos<<endl;
return 0;
}

void pesquisa(int chave){

	//DECLARAO DE VARIAVIES
	FILE* arvore;
	Titem2 registro2;
	int comparacoes = 0, acumulador = 0;
	bool regulador = false;
	
	if((arvore = fopen("Arvore_Binaria.bin", "rb")) == NULL) {
		cout<<"Erro na abertura do arquivo 'Arvore_binaria.bin'\n";
	}
	
	//INICIANDO A VARREDURA DA ARVORE A PROCURA DO DADO
	while( fread(&registro2, sizeof(registro2), 1, arvore) == 1 && regulador == false){
		
		//NUMERO DE COMPARACOES DE CHAVES
		comparacoes++;
		
		if(registro2.chave == chave){
			cout << "DADO ENCONTRADO: " << endl << "CHAVE: " << registro2.chave << endl << "DADO 1: " << registro2.dado1 << endl << "DADO 2: " << registro2.dado2 << endl;
			
			cout << "TOTAL DE COMPARACOES: " << comparacoes << endl;
			regulador = true;
		}else if(chave < registro2.chave) {
			acumulador = registro2.pEsq;
			regulador = false;
		}else if (chave > registro2.chave) {
			acumulador = registro2.pDir;
			regulador = false;
		}
		fseek(arvore, acumulador, SEEK_SET);
	}
	fclose(arvore);
}
