//Exercício Prático: Árvore de Huffman
/*
a) fazer um algoritimo para construção de árvore de huffman
    ? utiliza uma lista encadeada ordenada
        ? ordem crescente
    ? calcular a frequencia das palavras em um frase
    ? arvore possui esq, simbolo, frequencia, dir
    ? lista possuir no,prox
    ? tabela de registros possui palavra, frequencia, codigoHuffman

b) 
    salvar a tabela em em arquivo binario
    salvar a palavra traduzida em arquivo texto 
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#define TF 256

struct TREE{
    struct TREE *esq,*dir;
    int simb;
    int freq;
};typedef struct TREE tree;

struct LISTA{
    tree *info;
    struct LISTA *prox;
};typedef struct LISTA lista;

struct TABELA{
    char palavra[TF], codigo[TF];
    int freq;
};typedef struct TABELA tabela;

int isPontuacao(char a){
    if (a == ' ' || a == ',' || a == '.' || a == '?' || a == '!') return 1;
    else return 0;
}

void calcFrequencia(char frase[],tabela tab[], int *TL){
    
    int i,j,TL2;
    char palavra[TF];
    for (i = 0; i < strlen(frase); i++)
    {
        palavra[0] = '\0';
        for (TL2 = 0; !isPontuacao(frase[i]) && frase[i] != '\0'; i++)
		{
            palavra[TL2] = frase[i];
            tolower(palavra[TL2++]);
        }
        if(TL2 != 0)
		{
            palavra[TL2] = '\0';
            //caminha ate achar a palavra ou chegar no final
            for (j = 0; j < (*TL) && stricmp(palavra, tab[j].palavra) != 0; j++);

            if(j == (*TL)){
                //nao achou, insere na tabela
                strcpy(tab[(*TL)].palavra, palavra);
                tab[(*TL)].freq = 1;
                (*TL)++;
                //printf("%s\t%d\n",tab[(*TL)-1].palavra,(*TL)-1);
            }
            else
                tab[j].freq++;    
        }
    }
}

void exibeTab(tabela tab[],int TL){
    printf("\n");
	printf("indice\t\tpalavra\t\tfrequencia\tcodigo\n");
    for(int i=0; i<TL; i++)
        printf("%d\t\t%s\t\t%d\t\t%s\n",i,tab[i].palavra,tab[i].freq,tab[i].codigo);
}

void ordenarTab(tabela tab[],int TL){
    
    char auxPal[TF];
    int auxFreq;

    for(int i=0; i<TL; i++)
	{
        for(int j = i+1; j<TL; j++)
		{
            //alterar essa função faz se crescente |<| ou decrescente |>|
            if (tab[j].freq > tab[i].freq)
			{
                auxFreq = tab[i].freq;
                tab[i].freq = tab[j].freq;
                tab[j].freq = auxFreq;
                strcpy(auxPal, tab[i].palavra);
                strcpy(tab[i].palavra, tab[j].palavra);
                strcpy(tab[j].palavra, auxPal);
            }
        }
    }
}

tree *criaNo(tree*esq,int simb,int freq,tree*dir){
	
    tree *no = (tree*)malloc(sizeof(tree));
    no->esq = esq;
    no->simb = simb;
    no->freq = freq;
    no->dir = dir;
    
}

lista *criaItem(tree *info,lista *prox){
    lista *item = (lista*)malloc(sizeof(lista));
    item->info = info;
    item->prox = prox;
    return item;
}

void inserir(lista **l,tree *no){
	
    lista *item = criaItem(no,NULL);
    if(*l == NULL)
        *l = item;
    else{
        lista *ant,*atual;
        ant = NULL;
        atual = *l;
        while(atual != NULL && atual->info->freq < item->info->freq){
            ant = atual;
            atual = atual->prox;
        }
        if(ant == NULL){
            item->prox = atual;
            *l = item;
        }
        else{
            if(atual == NULL){
                ant->prox = item;
            }else{
                ant->prox = item;
                item->prox = atual;
            }
        }
    }
}

void remover(lista **l,tree **no){
	
    lista *aux = *l;
    *no = aux->info;
    *l = aux->prox;
    free(aux);
}

void enqueue(lista **fila,tree *no){
	
    lista *item = criaItem(no,NULL);
    if(*fila == NULL){
        *fila = item;
    }else{
        lista *aux = *fila;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = item;
    }
} 

tree *dequeue(lista **f){
   	lista *a = *f;
   	*f = (*f)->prox;
   	tree *t = a->info;
   	free(a);
	return t;
} 

void exibeArvore(tree *raiz){
    lista *fila = NULL;
    tree *no;
    int twonull = 0;
    enqueue(&fila,raiz);
    enqueue(&fila,NULL);
    printf("\n");
	while(twonull != 2){
        no = dequeue(&fila);
        if(no == NULL){
            printf("\n");
            enqueue(&fila,NULL);
            twonull++;
        }else{
        	printf("(simb:%d,freq:%d)",no->simb,no->freq);
            if(no->esq != NULL)
                enqueue(&fila,no->esq);
            if(no->dir != NULL)
                enqueue(&fila,no->dir);
            twonull = 0;
        }
    }
}


void criaCodigo(tree *huff, tabela tab[]){
    static char statcod[TF];
    static int i = -1;
    if(huff != NULL){
        if(huff->esq == NULL && huff->dir == NULL){
            strcpy(tab[huff->simb].codigo,statcod);
            tab[huff->simb].codigo[i+1] = '\0';
        }else{
            i++;
            statcod[i] = '0'; criaCodigo(huff->esq,tab);
            statcod[i] = '1';criaCodigo(huff->dir,tab);
            i--;
        }
    }else{
        return;
    }

}

tree* criaArvore(char frase[],tabela tab[],int *TL){
    
	lista *l = NULL;

    calcFrequencia(frase,tab,&(*TL));
    ordenarTab(tab,(*TL)); //decrescente
    exibeTab(tab,(*TL));

    for(int i=0;i<(*TL);i++)
        inserir(&l,criaNo(NULL, i, tab[i].freq, NULL));
    
    tree *esq,*dir;
    while(l->prox != NULL){
    	remover(&l,&dir);
        remover(&l,&esq);
        inserir(&l,criaNo(esq,-1,esq->freq + dir->freq,dir));
    }

    tree *huff;
    remover(&l,&huff);
    exibeArvore(huff);
    
    return huff;
}

void codificaTexto(char f[], tabela t[], char c[],int TL){
    int i,j,TL2;
    char p[TF];
	c[0] = '\0';
    for (i = 0; i < strlen(f); i++)
    {
        p[0] = '\0';
        for (TL2 = 0; !isPontuacao(f[i]) && f[i] != '\0'; i++)
		{
            p[TL2] = f[i];
            tolower(p[TL2++]);
        }
        if(TL2 != 0){
	        p[TL2] = '\0';
	        
			for(j=0; j<TL && strcmp(t[j].palavra,p) !=0;j++);
	
	        if(j<TL)
	        	strcat(c,t[j].codigo);	  	
        }
    }
}

tabela* retTabLine(tabela tab[],int i){
    tabela *t = (tabela*)malloc(sizeof(tabela));
    strcpy(t->codigo,tab[i].codigo);
    strcpy(t->palavra,tab[i].palavra);
    t->freq = tab[i].freq;
    return t;
}

int main(){
	
    int TL = 0;
    char frase[TF*3]={"44 22 33 33 11 44 33 44 44 22"};
    char abra[TF*3]={"a b r a c a d a b r a"};
    char amo[TF*3]={"amo como ama o amor nao conheco nenhuma outra razao para amar senao amar que queres que te diga alem de que te amo se o que quero dizer-te eh que te amo"};
    tabela tab[TF];

    //a)
	tree *huff = criaArvore(amo,tab,&TL);
	criaCodigo(huff,tab);
    exibeTab(tab,TL);
    char codificada[TF*3];
    codificaTexto(amo,tab,codificada,TL);
    printf("\n");
	printf("frase: %s\n",amo);
    printf("\n");
    printf("frase codificada: %s\n",codificada);

    //b
    FILE *arq;
    //b.1
    arq = fopen("tabela.bin","wb");
    for(int i=0; i<TL;i++)
        fwrite(retTabLine(tab,i),1,sizeof(tabela),arq);
    fclose(arq);

    //b.2
    arq = fopen("codificada.txt","w");
    fputs(codificada,arq);
    fclose(arq);
    
	return 6969;
    
}
