//Exercício Prático: Árvore de Huffman
/*
    monte uma arvore usando a tabela
    decodifica a palavra usando a arvore
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

void exibeTab(tabela tab[],int TL){
    printf("\n");
	printf("indice\t\tpalavra\t\tfrequencia\tcodigo\n");
    for(int i=0; i<TL; i++)
        printf("%d\t\t%s\t\t%d\t\t%s\n",i,tab[i].palavra,tab[i].freq,tab[i].codigo);
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

tree *criaNo(tree*esq,int simb,int freq,tree*dir){
	
    tree *no = (tree*)malloc(sizeof(tree));
    no->esq = esq;
    no->simb = simb;
    no->freq = freq;
    no->dir = dir;
    
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

tree* criaArvoreTab(tabela tab[],int TL){
    
	lista *l = NULL;

    //exibeTab(tab,TL);

    for(int i=0;i<TL;i++)
        inserir(&l,criaNo(NULL, i, tab[i].freq, NULL));
    
    tree *esq,*dir;
    while(l->prox != NULL){
    	remover(&l,&dir);
        remover(&l,&esq);
        inserir(&l,criaNo(esq,-1,esq->freq + dir->freq,dir));
    }

    tree *huff;
    remover(&l,&huff);
    //exibeArvore(huff);
    
    return huff;
}


void decode(char dec[],char cod[],tree *huf,tabela tab[],int TL){
	
	tree *no = huf;
	
	dec[0] = '\0';
	for(int i=0; cod[i]!='\0';i++){
		if(no->esq == NULL && no->dir==NULL){
			strcat(dec,tab[no->simb].palavra);
			strcat(dec," \0");
			i--;
			no = huf;
		}else{
			if(cod[i] == '1'){
				//cod[i] = 'x';
				no = no->dir;
			}
			else if(cod[i] == '0'){
				//cod[i] = 'x';
				no = no->esq;
			}
		}
	}
	strcat(dec,tab[no->simb].palavra);
	strcat(dec,"\0");				
}


int main(){

    //monta arvore
    tabela *taux = (tabela*)malloc(sizeof(tabela));
    tabela tab[TF];
    int TL = 0;
	tree *huff = NULL;
	FILE *arq;
    arq = fopen("tabela.bin","rb");
    if(arq != NULL){
    	fread(taux,1,sizeof(tabela),arq);
		for(TL=0; !feof(arq); TL++){ 
		    strcpy(tab[TL].palavra,taux->palavra);
		    strcpy(tab[TL].codigo,taux->codigo);
		    tab[TL].freq = taux->freq;
		    fread(taux,1,sizeof(tabela),arq);
		}
		huff = criaArvoreTab(tab,TL);
    }
    fclose(arq);
    
    //decodifica
    char codificada[TF*3], decodificada[TF*3];
    arq = fopen("codificada.txt","r");
    if(arq != NULL){
    	codificada[0] = '\0';
    	fgets(codificada,TF*3,arq);
        decode(decodificada,codificada,huff,tab,TL);
        printf("\n");
		printf("codificada: %s\n",codificada);
    	printf("\n");
		printf("decodificada: %s\n",decodificada);
    }
	fclose(arq);


    return 0;
}