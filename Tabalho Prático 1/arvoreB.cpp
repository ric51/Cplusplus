#include "cabecalho.hpp"
#include "arvoreB.hpp"

using namespace std;

void pesquisa_ArvoreB(int chave) {
	int Trans=0, Comp =0;
    TipoApontador arvore;
    Titem aux;
    FILE *arq = NULL;
    Inicializa(&arvore);


    if((arq = fopen("dados.bin", "rb")) == NULL){ // abertura do arquivo de dados
        cout<<"Erro ao abrir arquivo "<<endl;
        return;
    }

    //caminhar no arquivo pra acessar os dados e montar a árvore
    while (fread(&aux, sizeof (Titem), 1, arq)) {
        Trans++;
        Insere(aux, &arvore, Comp);
    }
    aux.chave = chave;
    Pesquisa_AB(&aux, arvore, Comp, Trans);

    fclose(arq);
}

void Inicializa(TipoApontador *Dicionario) {  
	*Dicionario = NULL; 
}

void Insere (Titem Reg, TipoApontador *Ap, int Comp) { 
	short Cresceu;
	Titem RegRetorno;
	TipoPagina *ApRetorno, *ApTemp;
	Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, Comp);
	if (Cresceu) {  /* Arvore cresce na altura pela raiz */
		ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
		ApTemp->n = 1; 
		ApTemp->r[0] = RegRetorno;
		ApTemp->p[1] = ApRetorno;
		ApTemp->p[0] = *Ap;  *Ap = ApTemp;
	}
}

void Ins (Titem Reg, TipoApontador Ap, short *Cresceu, Titem *RegRetorno,  TipoApontador *ApRetorno, int Comp){ 
	long i = 1; long j;
	TipoApontador ApTemp;
	
	if (Ap == NULL) { 
		*Cresceu = true; (*RegRetorno) = Reg; (*ApRetorno) = NULL;
		return;
	}
	
	while (i < Ap->n && Reg.chave > Ap->r[i-1].chave)  
		i++;
	if (Reg.chave == Ap->r[i-1].chave) { 
		
		*Cresceu = false;
		Comp++;
		return;
	}
	Comp++;
	Comp++;
	if (Reg.chave < Ap->r[i-1].chave) 
		i--;
	Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, Comp);
	
	if (!*Cresceu) 
		return;
		
	if (Ap->n < MM) {  /* Pagina tem espaco */
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno, Comp);
		*Cresceu = false;
		return;
	}
	/* Overflow: Pagina tem que ser dividida */
	ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
	ApTemp->n = 0;  ApTemp->p[0] = NULL;
	
	if (i < M + 1) {
		InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM], Comp);
		Ap->n--;
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno, Comp);
	} else 
		InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, Comp);
		
	for (j = M + 2; j <= MM; j++) 
		InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j], Comp);
		
	Ap->n = M;  ApTemp->p[0] = Ap->p[M+1];
	*RegRetorno = Ap->r[M];  *ApRetorno = ApTemp;
}

void InsereNaPagina (TipoApontador Ap, Titem Reg, TipoApontador ApDir, int Comp) { 
	short NaoAchouPosicao;
	int k;
	k = Ap->n;  
	NaoAchouPosicao = (k > 0);
	while (NaoAchouPosicao) { 
		Comp++;
		if (Reg.chave >= Ap->r[k-1].chave) { 
			NaoAchouPosicao = false;
			break;
		}
		Ap->r[k] = Ap->r[k-1];
		Ap->p[k+1] = Ap->p[k];
		k--;
		if (k < 1) 
			NaoAchouPosicao = false;
	}
	
	Ap->r[k] = Reg; 
	Ap->p[k+1] = ApDir;
	Ap->n++;
} 

void Pesquisa_AB(Titem *x, TipoApontador Ap, int Comp, int Trans){ 
	long i = 1;
	if (Ap == NULL) { 
	
		cout<<"O Registro nao esta presente na arvore "<<endl;
        
		return;
	}
	
	while (i < Ap->n && x->chave > Ap->r[i-1].chave) {
		i++;
		Comp++;
	}
	Comp++;
	Comp++;
	
	if (x->chave == Ap->r[i-1].chave) { 
		*x = Ap->r[i-1];
		
		cout<<"Registro de codigo "<< x->chave<< " foi localizado" <<endl;
		
		cout<<"Numero de tranferencias: "<<(Trans/MM)<<endl;
		
		cout<<"Numero de comparacoes: "<< Comp <<endl;
		
		cout<< " Dado 1 : " << x->dado1 << endl;
		cout<< " Dado 2 : " << x->dado2 << endl;
		return;
	}
	
	if (x->chave < Ap->r[i-1].chave) 
		Pesquisa_AB(x, Ap->p[i-1], Comp, Trans);
	else 
		Pesquisa_AB(x, Ap->p[i], Comp, Trans);
} 



