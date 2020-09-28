#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cabecalho.hpp"

using namespace std;

//Funcao responsavel pela geracao do arquivo________________________________________________________________________________;

int gerador(int num, int opcao, int arg)
{
	FILE *output;
	Titem x;
	int *v;
	
	switch(opcao) {
		case 1:
			//Arquivo ascendentemente;
			if((output = fopen("dados.bin","wb")) == NULL) {
				cout<<"Erro ao criar o arquivo\n";
				return 0;
			}
			for(int i=0; i<num; i++) {
				x.chave = i;
				x.dado1 = rand() % 1000000;

				for(int j=0; j<500; j++)
				{
					x.dado2[j] = 'a'+rand() % 26;
				}
				if (arg){
					cout<< "Chave "<<x.chave<<"\n\n"<<endl;
					fflush (stdout);
				}
				fwrite(&x, sizeof(x), 1, output);
			}
			fclose(output);
		break;
		
		case 2:												
		//Arquivo ordenado descendentemente;
			if((output = fopen("dados.bin","wb")) == NULL) {
				cout<<"Erro ao criar o arquivo\n";
				return 0;
			}
			for(int i=num; i>0; i--) {
				x.chave = i;
				x.dado1 = rand() % 1000000;

				for(int j=0; j<500; j++)
				{
					x.dado2[j] = 'a' + rand() % 26;
				}
				if (arg){
					cout<< "Chave "<<x.chave<<"\n\n"<<endl;
					fflush (stdout);
				}
				fwrite(&x, sizeof(x), 1, output);
			}
			fclose(output);
		break;
		
		case 3:												//Arquivo aleatorio;
			
			v = new int[num];
			
			if((output = fopen("dados.bin","wb")) == NULL)
			{
				cout<<"Erro ao criar o arquivo\n";
				return 0;
			}
			for(int l=0; l<num; l++) 						//Inseri de 0 a "num" no vetor, em ordem.
			{
				v[l] = l;
			}
			for (int k = 0; k < num; k++) 					//"Embbaralha" as posicoes do vetor.
			{
				int r = rand() % num;
				int temp = v[k];
				
				v[k] = v[r];
				v[r] = temp;
			}
			for(int i=0; i<num; i++)
			{
				x.chave = v[i];
				x.dado1 = rand() % 1000000;
				
				for(int j=0; j<500; j++)
				{
					x.dado2[j] = 'a' + rand() % 26;
				}
				if (arg){
					cout<< "Chave "<<x.chave<<"\n\n"<<endl;
					fflush (stdout);
				}
				fwrite(&x, sizeof(x), 1, output);
			}
			
			delete[] v;
			
			fclose(output);
		break;
		
		default:
			cout<<"__________________\n";
			cout<<"|Opcao invalida! |\n";
			cout<<"__________________\n";
			break;
	}
	return 0;
}