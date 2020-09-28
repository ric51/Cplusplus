#include "ArvoreBstar.hpp"
#include "cabecalho.hpp"
#include <iostream>
#include <ctime>
#include <cstring>
#define f(inicio,fim) for(int i =inicio ; i<fim; i++)

using namespace std;

struct tipoitem
{
  
    long chave;
    long int dado1;
    char dado2[501];
};

struct TipoPagina 
{
    TipoIntExt tipo;
    union
    {
        struct Interna
        {
            int n_chaves=0;
            int chave[MM];
            TipoApontador Apontador[MM + 1];
        }interno;
        struct
        {
            int n_itens=0;
            Item Itens[MM2];
            TipoApontador prox; //ponteiro para a proxima página
        } externo;
    }total;
};

TipoApontador geraArvoreBstar()
{
    double final;

    FILE* arquivo;
    Item item_read; //variavel auxiliar para leitura em arquivo
    TipoApontador Arvore;
    Arvore = NULL;

 
    if ((arquivo = fopen("arquivo.bin","rb")) == NULL)
        return NULL;


    while(fread(&item_read,sizeof(Item), 1, arquivo)!=0)
    {
        //Insere na arvore
    
        
    }
    fclose(arquivo);
    return Arvore;
}

void InsereNaPagina(TipoApontador Apontador, int Registro, TipoApontador ApontadorDireita)
{
    short NaoAchouPosicao;
    int k;
    k = Apontador->total.interno.n_chaves;
    NaoAchouPosicao = (k > 0); //variavel que mantem o controle da posicao corrente
  
    while(NaoAchouPosicao)
    {
    
        if (Registro >= Apontador->total.interno.chave[k - 1])
            break;
        //coloca o registro e o apontador numa posição a frente
        //("arreda" para a direita)
        Apontador->total.interno.chave[k] = Apontador->total.interno.chave[k - 1];
        Apontador->total.interno.Apontador[k + 1] = Apontador->total.interno.Apontador[k];
        k--;
      
        if (k < 1)
            NaoAchouPosicao = false;
    }
    //achou posicao
    Apontador->total.interno.chave[k] = Registro;
    Apontador->total.interno.Apontador[k+1] = ApontadorDireita;
    Apontador->total.interno.n_chaves++;
    return;

}

void Ins(Item *Registro, TipoApontador Apontador, short *Cresceu, Item *RegRetorno, TipoApontador* ApRetorno, short *Cresceu_no)
{
    long i = 1;
    long j;
    TipoApontador ApTemporario;
    TipoApontador novo= nullptr;
    Item aux;

   
    if (Apontador->tipo == Externa)
    {
        //Se entrou quer dizer que chegou ao ultimo nivel
        //tentar inserir na página externa
        //verifica se tem espaço na página
       
        if(Apontador->total.externo.n_itens < MM2)
        {
          
            *Cresceu = false;
            *Cresceu_no = false;
            //Não precisa crescer nivel nem dividir
            return;
        }
        else
        {   //---------------------- SEPARA UM NÓ EM DOIS -------------------
            //tera que dividir
            //Sobe pro pai a copia da chave e divide
            //RegRetorno vai salvar a chave do pai
            //ApRetorno vai salvar o novo nó a direita
            novo = (TipoPagina*)malloc(sizeof(TipoPagina));
            novo->tipo=Externa;
            novo->total.externo.n_itens=0;
            aux = Apontador->total.externo.Itens[MM2-1];

            //coloca o ultimo elemento na pagina à direita
         
            //decrementa o numero de itens dessa página
            

         bool inseriu = InserenaFolha(Registro, Apontador);
            if(!inseriu)
            {
                Apontador->total.externo.n_itens++; //incrementa para voltar ao numero anterior
                delete novo;
                *Cresceu = false;
                *Cresceu_no= false;
                return;
            }

            //passa metade pra nova folha
            f((MM2/2),MM2)
            {
                
                InserenaFolha(&(Apontador->total.externo.Itens[i]), novo);
                Apontador->total.externo.n_itens--;
            }

            //A chave do primeiro elemento do novo vetor será o pai
            RegRetorno->chave = novo->total.externo.Itens[0].chave;
            *ApRetorno = novo;
            Apontador->total.externo.prox = novo;
            novo->total.externo.prox= nullptr;

            //Após inserir na arvore o novo registro, a chave que precisa subir
            //para o nó pai precisa ser inserida na arvore
            Registro->chave = novo->total.externo.Itens[0].chave;
            *Cresceu_no =true; //dividiu o nó folha
            return;
        }
    }
    else
    {
        // se a página for interna
        while (i < Apontador->total.interno.n_chaves && Registro->chave > Apontador->total.interno.chave[i - 1]) //acha a posição na página
        {
            
            i++;
        }

        
        if (Registro->chave == Apontador->total.interno.chave[i - 1])
        {
            //Verificar dentro do nó folha para ver se ainda existe
            *Cresceu = false;
            *Cresceu_no =false;
            return;
        }

        
        if (Registro->chave < Apontador->total.interno.chave[i - 1])
            i--;

        //Encontra o lugar onde a chave deve entrar
        //Entra se a proxima página for interna
        
        if(!*Cresceu_no)
            Ins(Registro, Apontador->total.interno.Apontador[i], Cresceu, RegRetorno, ApRetorno,Cresceu_no);

        
        if (!*Cresceu && !*Cresceu_no)
            return;

        
        if (Apontador->total.interno.n_chaves < MM) //pagina tem espaço
        {
            InsereNaPagina(Apontador, RegRetorno->chave, *ApRetorno);
            *Cresceu = false;
            *Cresceu_no =false;
            return;
        }

        //Se chegou até aqui, a página está cheia e deve ser divida
        ApTemporario = (TipoApontador) malloc(sizeof(TipoPagina));
        ApTemporario->tipo = Interna;
        ApTemporario->total.interno.n_chaves = 0;
        ApTemporario->total.interno.Apontador[0] = nullptr;

        //Insere metade dos registros da página cheia para a nova página criada
        
        if (i < MM / 2 + 1)
        {
            InsereNaPagina(ApTemporario, Apontador->total.interno.chave[MM - 1], Apontador->total.interno.Apontador[MM]);
            Apontador->total.interno.n_chaves--;
            InsereNaPagina(Apontador, RegRetorno->chave, *ApRetorno);
        }

        else
            InsereNaPagina(ApTemporario, RegRetorno->chave, *ApRetorno);

        for (j = MM / 2 + 2; j <= MM; j++)
        {
            InsereNaPagina(ApTemporario, Apontador->total.interno.chave[j - 1], Apontador->total.interno.Apontador[j]);
            
        }


        Apontador->total.interno.n_chaves = MM/2 ;
        ApTemporario->total.interno.Apontador[0] = Apontador->total.interno.Apontador[MM / 2 + 1];

        RegRetorno->chave = Apontador->total.interno.chave[MM / 2];

        *ApRetorno = ApTemporario;
        *Cresceu=true;
        //já alocou o novo no, então não precisa mais
        *Cresceu_no =false;
        return;
    }
}

void Insere(Item *Registro, TipoApontador* Apontador)
{
    short Cresceu =0;
    short Cresceu_no =0;

    Item RegRetorno;
    TipoPagina *ApRetorno = nullptr;
    TipoPagina *ApTemp = nullptr;
    bool nova=false;

    
    if(*Apontador== NULL)
    {
        //aloca a primeira página
        //Cria uma nova página externa
        ApTemp= (TipoPagina*)malloc(sizeof(TipoPagina));
        ApTemp->tipo= Externa;
        ApTemp->total.externo.n_itens = 0;
        ApTemp->total.externo.prox= nullptr;

        InserenaFolha(Registro,ApTemp);
        *Apontador = ApTemp;

        return;
    }
    else
    {
       
        //para controlar se o será criado um novo nó interno
        if((*Apontador)->tipo == Externa)
            nova=true;
        Ins(Registro, *Apontador, &Cresceu, &RegRetorno, &ApRetorno, &Cresceu_no);
    }
    

    //Se for o primeiro nó a ser criado ou não
    if ((Cresceu_no && nova) || Cresceu)
    {
        //Somente para o tipo interno
        //Arvore cresce na altura pela raiz
        ApTemp = (TipoPagina *) malloc(sizeof(TipoPagina));
        ApTemp->tipo = Interna;

        ApTemp->total.interno.n_chaves = 1;
        ApTemp->total.interno.chave[0] = RegRetorno.chave;

        ApTemp->total.interno.Apontador[0] = *Apontador;
        ApTemp->total.interno.Apontador[1] = ApRetorno;
        *Apontador = ApTemp;

        return;
    }
}

bool InserenaFolha(Item *reg, TipoApontador nova_pagina ) //Insere na página externa
{
    int numeroItens = nova_pagina->total.externo.n_itens;
    int k=0;
    int i=0;

    while(reg->chave > nova_pagina->total.externo.Itens[i].chave && i<numeroItens) //Enquanto a nova chave for maior que as do vetor
    {
        
        i++;
    }

   
    if(reg->chave == nova_pagina->total.externo.Itens[i].chave  && numeroItens!=0 )
        return false;

    
    if(i<numeroItens) // Empurra os registros para inserir
    {
        k=numeroItens;
        while(k>=0 && k>i)
        {
            nova_pagina->total.externo.Itens[k] = nova_pagina->total.externo.Itens[k-1];
            k--;
           
        }
        nova_pagina->total.externo.Itens[i] =  *reg; //insere na posição i
    }
    else //insere no final
        nova_pagina->total.externo.Itens[i] = *reg;

    nova_pagina->total.externo.n_itens++;

    return  true;
}


void ImprimeBSTAR(TipoApontador arvore)
{
    int i = 0;

    TipoApontador aux;
    if (arvore == NULL)
        return;

    if(arvore->tipo == Interna)
    {
        while (i <= arvore->total.interno.n_chaves) {
            ImprimeBSTAR(arvore->total.interno.Apontador[i]);
            if (i != arvore->total.interno.n_chaves)
                printf("%d  ", arvore->total.interno.chave[i]);
            i++;
        }
        printf("\n");
    }
    else
    {
        aux= arvore;
        for (int j = 0; j < aux->total.externo.n_itens; j++) {
            printf("(%d) %ld | ", j,aux->total.externo.Itens[j].chave);
        }
        printf("\n");
    }
}

bool PesquisaArvoreBstar(Item *x, TipoApontador *Ap)
{
    int i;
    cout << "chegou na BSTAR" << endl;
    double final;

    
    if((*Ap)->tipo == Interna)
    {
        i = 1;
        while(i<(*Ap)->total.interno.n_chaves && x->chave > (*Ap)->total.interno.chave[i-1])
        {
            i++;
           
        }

        

        if(x->chave < (*Ap)->total.interno.chave[i-1])
            return PesquisaArvoreBstar(x, &(*Ap)->total.interno.Apontador[i-1]);
        else
           return PesquisaArvoreBstar(x, &(*Ap)->total.interno.Apontador[i]);
    }

    i = 1;
    while(i<(*Ap)->total.externo.n_itens && x->chave>(*Ap)->total.externo.Itens[i-1].chave)
    {
        i++;
        
    }

    
    if(x->chave == (*Ap)->total.externo.Itens[i-1].chave)
    {
        x->chave = (*Ap)->total.externo.Itens[i - 1].chave;
        x->dado1 = (*Ap)->total.externo.Itens[i - 1].dado1;
        strcpy(x->dado2,(*Ap)->total.externo.Itens[i - 1].dado2);
        final = clock();
        

        return true;
    }
    else
    {
        x->chave= -1;
        final = clock();
        
        return false;
    }
}
