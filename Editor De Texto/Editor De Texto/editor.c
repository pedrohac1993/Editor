#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>
#define tam 256

//estrutura para preencher o Nó com caracter
typedef struct caracter{
	char letra;
	struct caracter * ant;
	struct caracter * prox;
}Tcaracter;
//estrutura para manipular as linhas
typedef struct linha{
	struct linha * ant;
	struct linha * prox;
	//tecnica avançada do jutso da sombra
	struct caracter * linha;
	int qtd_caracter;
}Tlinha;
//descritor para linhas
typedef struct descritor{
	Tlinha  * prim, * ult;
	int qtd_de_linhas;
}Ldescritor;

FILE * criarArquivo (char nome[]);
//inserir
void inserir_linha(Ldescritor **l, Tlinha ** Py, int y);
void criar_no_linha(Tlinha ** novo);
void inserir_caracter_linha (Tcaracter *px,Tlinha **py, char c, int y);
void criar_no_caracter(Tcaracter **novo, char letra );
void inserir(Ldescritor **l, FILE * arq, char nome_arquivo[]);
//exibir
void exibir_texto(Ldescritor *l);
void exibir(Tcaracter *l);
//arquivo
void salvar(Ldescritor *l, FILE * arq, char nome_arquivo[]);
void carregar(Ldescritor **l, FILE * arq);
void txt(char nome_arquivo[tam]);
//curso para alterar o texto
void gotoxy(int x, int y);
void mover_px(int qtd, Tcaracter ** Px);
int remover(Tcaracter ** Px, Tlinha **Py);

int main(){

	char nome_arquivo[tam];
	FILE * arq=NULL;
	Ldescritor *l=NULL;
	Tlinha * Py; 
	int y=-1;
	setlocale(LC_ALL, "Portuguese");

	printf("\t\tBem vindo\n\t   Editor de Texto 1.0\n\nDigite O Nome Do Texto: ");
	gets(nome_arquivo) ;fflush(stdin);
	txt(nome_arquivo);
	system("cls");

	//inicializa o descritor
	inserir_linha(&l,&Py,y);
	arq = fopen (nome_arquivo, "r+b");
	if (arq == NULL) 
		arq = fopen (nome_arquivo, "w+b");
	else{
		carregar(&l,arq);
		exibir_texto(l);
	}
	inserir(&l,arq,nome_arquivo);
	return 0;
}
//OK
void txt(char nome_arquivo[tam]){

	int i,qtd=0,k=0;
	char tipo[5]={".txt"};

	qtd=strlen(nome_arquivo);
	for (i = qtd; i < qtd+5; i++){
		nome_arquivo[i]=tipo[k];
		k++;
	}
}
//OK
FILE * criarArquivo (char nome[]) { 
	FILE * arq;
	arq = fopen (nome, "r+b");
	if (arq == NULL) {
		arq = fopen (nome, "w+b");
	}
	return arq;
}
//OK
void criar_no_linha(Tlinha ** novo){
	(*novo)=(Tlinha*)malloc(sizeof(Tlinha));
	(*novo)->ant=NULL;
	(*novo)->linha=NULL;
	(*novo)->prox=NULL;
	(*novo)->qtd_caracter=0;
}
//OK
void inserir_linha(Ldescritor **l, Tlinha ** Py, int x){

	Tlinha * novo=NULL, *aux=NULL;

	//insere no inicio
	if((*l)==NULL){
		//cria a primeira linha
		criar_no_linha(&novo);		
		//cria o descridor das linhas
		(*l)=(Ldescritor*)malloc(sizeof(Ldescritor));
		(*l)->prim=novo;
		(*l)->ult=novo;
		(*l)->qtd_de_linhas=0;
	}else {		
		//insere inicio
		if (x==0){
			criar_no_linha(&novo);
			(*l)->prim=novo;
			novo->prox=(*Py);
			(*Py)->ant=novo;
			(*l)->qtd_de_linhas++;			
			//insere no fim
		}else if((*Py)->prox == NULL){				
			criar_no_linha(&novo);
			(*Py)->prox=novo;
			novo->ant=(*Py);			
			(*l)->qtd_de_linhas++;
			(*l)->ult=novo;
			//insere no meio
		}else{		
			criar_no_linha(&novo);
			aux=(*Py)->prox;
			(*Py)->prox=novo;
			novo->ant=(*Py);
			aux->ant=novo;
			novo->prox=aux;
			(*l)->qtd_de_linhas++;
		}
	}
}

void criar_no_caracter(Tcaracter **novo, char letra ){
	(*novo)=(Tcaracter*)malloc(sizeof(Tcaracter));
	(*novo)->ant=NULL;
	(*novo)->prox=NULL;
	(*novo)->letra=letra;
}

void gotoxy(int x, int y) {

	COORD c;
	c.X = y;
	c.Y = x;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void inserir_caracter_linha (Tcaracter *px,Tlinha **py, char c, int y){

	Tcaracter * novo, *aux;

	//insere no inicio
	if((*py)->linha==NULL){
		if(c=='\n'){
			criar_no_caracter(&novo,c);		
			(*py)->linha=novo;
			(*py)->qtd_caracter=0;		
		}else if(c=='\0'){
			(*py)->linha=px;
			(*py)->qtd_caracter=y;		
		}else{
			criar_no_caracter(&novo,c);		
			(*py)->linha=novo;
			(*py)->qtd_caracter=1;			
		}
	}
	else {
		
		//insere no fim
		if (px->prox == NULL && y!=0){		
			criar_no_caracter(&novo,c);
			px->prox=novo;
			novo->ant=px;
			novo->prox=NULL;
			if(c!='\n')
				(*py)->qtd_caracter++;
			//insere no inicio com nos já inseridos
		}else if(px->ant==NULL && y==0){
			criar_no_caracter(&novo,c);
			(*py)->linha=novo;
			novo->prox=px;		
			novo->ant=NULL;
			px->ant=novo;		
			(*py)->qtd_caracter++;	
			//insere no meio
		}else{
			criar_no_caracter(&novo,c);
			aux=px->prox;
			px->prox=novo;
			novo->ant=px;
			aux->ant=novo;
			novo->prox=aux;
			(*py)->qtd_caracter++;
		}
	}
}

void inserir(Ldescritor **l, FILE * arq, char nome_arquivo[]){
	/*
	cima 72
	baixo 80
	direita 77
	esquerda 75
	del 83
	enter 13
	ctrl+s 19
	ctrl+e 5
	backspace 8
	*/
	char c;
	//X: LINHA Y: COLUNA
	int x=0,y=0;
	Tlinha * Py=(*l)->prim, *aux1=NULL,*aux2=NULL;
	Tcaracter * Px=Py->linha, * aux;
	int qtd_caracter_proxima_linha=0;
	gotoxy(0,0);//inicio do texto
	do{
		c=_getch();fflush(stdin);
		//ctrl+s
		if(c==19){

			salvar(*l,arq,nome_arquivo);
			arq=fopen(nome_arquivo,"r+b");
			if (arq==NULL)
				printf("Erro De Abertura do Arquivo");
			//ctrl+e
		}else if(c==5){
			return;
			//enter
		}else if(c==13){
			if(Px==NULL && y==0){// insere \n no inicio  da linha
					inserir_caracter_linha(Px,&Py,'\n',y);
					inserir_linha(l,&Py,y);
					Py=Py->prox;
					Px=Py->linha;
					system("cls");
					exibir_texto(*l);
					x++;					
					gotoxy(x,y);	
			}else if(y==Py->qtd_caracter){// insere \n no fim da linha
				inserir_caracter_linha(Px,&Py,'\n',y);
				inserir_linha(l,&Py,y);
				Py=Py->prox;
				Px=Py->linha;
				system("cls");
				exibir_texto(*l);
				x++;
				y=0;
				gotoxy(x,y);			
			}else{//quebra de linha
				if(y==0){
					if(Py->ant!=NULL){
						Py=Py->ant;
						inserir_linha(l,&Py,x);
						Py=Py->prox;
					}else{
						inserir_linha(l,&Py,x);
						Py=Py->ant;
					}				
				inserir_caracter_linha(Px,&Py,'\n',y);
				Py=Py->prox;
				Px=Py->linha;
				system("cls");
				exibir_texto(*l);
				x++;
				y=0;
				gotoxy(x,y);				
				}else{
				aux=Px->prox;				
				qtd_caracter_proxima_linha=Py->qtd_caracter-y;
				inserir_caracter_linha(Px,&Py,'\n',y);
				Px=Px->prox;
				Px->prox=NULL;
				Py->qtd_caracter=y;
				inserir_linha(l,&Py,y);
				Py=Py->prox;
				Px=Py->linha;
				inserir_caracter_linha(aux,&Py,'\0',qtd_caracter_proxima_linha);			
				Px=Py->linha;
				Px->ant=NULL;
				system("cls");
				exibir_texto(*l);				
				x++;
				y=0;
				gotoxy(x,y);
				}
			}
		}else{
			if( c == -32){
			}
			else {
				//cima
				if (c == 72){
					if(x>0){
						if(y<=Py->ant->qtd_caracter){
							x--;
							gotoxy(x,y);
							Py=Py->ant;
							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);							
						}else{
							y=Py->ant->qtd_caracter;
							x--;
							gotoxy(x,y);
							Py=Py->ant;
							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);							
						}
					}
				}//baixo
				else if( c == 80){
					if(x<(*l)->qtd_de_linhas){
						if(y<=Py->prox->qtd_caracter){
							x++;
							gotoxy(x,y);
							Py=Py->prox;
							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);							
						}else{
							y=Py->prox->qtd_caracter;
							x++;
							gotoxy(x,y);
							Py=Py->prox;
							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);							
						}
					}
					//esquerda
				}else if( c== 75){
					if(y>0){
						if(y==1){
							y--;
							gotoxy(x,y);							
						}else{
							y--;
							gotoxy(x,y);
							Px=Px->ant;							
						}
					}
					//direita
				}else if( c == 77 ){
					if(y<Py->qtd_caracter){

						if(y==0){
							y++;
							gotoxy(x,y);							
						}else{
							y++;
							gotoxy(x,y);
							Px=Px->prox;							
						}
					}//backspace
				}else if(c==8){
					if(y>0){					
						remover(&Px,&Py);
						system("cls");
						exibir_texto(*l);
						y--;
						gotoxy(x,y);					
					}else if(Py->prox==NULL && Px==NULL && Py->ant!=NULL){
						aux1=Py;						
						Py=Py->ant;
						Py->prox=NULL;
						(*l)->qtd_de_linhas--;
						(*l)->ult=Py;
						free(aux1);
						Px=Py->linha;
						
						mover_px(Py->qtd_caracter,&Px);
						aux=Px->prox;
						free(aux);
						Px->prox=NULL;

						system("cls");
						exibir_texto(*l);
						y=Py->qtd_caracter;
						x--;
						gotoxy(x,y);
						
					}else if(Py->prox!=NULL && Py->qtd_caracter==0){
						if(x==0){//inicio
							aux1=Py;
							Py=Py->prox;
							free(aux1);							
							(*l)->qtd_de_linhas--;
							(*l)->prim=Py;
							Py->ant=NULL;
							Px=Py->linha;

						system("cls");
						x=0;
						y=0;
						exibir_texto(*l);	
						gotoxy(x,y);

						//meio
						}else{
							aux1=Py;
							aux2=Py->prox;
							Py=Py->ant;
							aux2->ant=aux1->ant;
							aux1->ant->prox=aux2;
							free(aux1);
							(*l)->qtd_de_linhas--;
							Px=Py->linha;
						system("cls");
						x--;
						y=0;
						exibir_texto(*l);	
						gotoxy(x,y);
						}
					}else if (x!=0){
						aux1=Py;
						aux=Px;
						Py=Py->ant;
						Px=Py->linha;
						(*l)->qtd_de_linhas--;
						Py->prox=NULL;
						y=Py->qtd_caracter;
						Py->qtd_caracter=Py->qtd_caracter+aux1->qtd_caracter;
						free(aux1);						
						mover_px(y,&Px);						
						Px->prox=aux;
							system("cls");
							exibir_texto(*l);	
							x--;							
							gotoxy(x,y);						
					}
				}else if(isalnum(c) || c == ' '){

					inserir_caracter_linha(Px,&Py,c,y);
					printf("%c",c);
					y++;
					if(Px==NULL)
						Px=Py->linha;						
					else{
						if(y==1 && Px->ant!=NULL)
							Px=Px->ant;						
						else
							Px=Px->prox;
					}
					system("cls");
					exibir_texto(*l);
					gotoxy(x,y);
				}
			}
		}

	} while (1);

}



int remover(Tcaracter ** Px, Tlinha **Py){
	Tcaracter * aux1, *aux2; 
	Tlinha *aux3=NULL;

	if((*Py)->linha==NULL)//lista vazia
		return 0;
	else if((*Px)->ant==NULL){//inicio
		if((*Px)->prox==NULL){//lista com um unico nó
			aux1=(*Py)->linha;
			(*Py)->linha=NULL;
			(*Py)->qtd_caracter=0;
			(*Px)=NULL;//PX
			free(aux1);			
			return 1;
		}else{//lista com mais de um nó
			aux1=(*Py)->linha;
			(*Py)->linha=aux1->prox;
			(*Py)->linha->ant=NULL;
			(*Py)->qtd_caracter--;
			(*Px)=(*Px)->prox;//PX
			free(aux1);			
			return 1;
		}	
	}else if((*Px)->prox==NULL){//fim
		aux1=(*Px);
		aux2=aux1->ant;
		(*Px)=(*Px)->ant;//PX
		free(aux1);
		(*Py)->qtd_caracter--;
		aux2->prox=NULL;
		return 1;
	}else{//meio
		aux1=(*Px)->ant;
		aux2=(*Px);
		aux1->prox=aux2->prox;
		aux2->prox->ant=aux1;
		(*Px)=(*Px)->ant;//PX
		(*Py)->qtd_caracter--;
		free(aux2);
		return 1;
	}
}

void mover_px(int qtd, Tcaracter ** Px){

	int i=1;

	if(qtd==0)
		return;
	else{
		while (i<qtd){
			(*Px)=(*Px)->prox;
			i++;
		}
	}
}


void salvar(Ldescritor *l, FILE * arq, char nome_arquivo[]){
	Tlinha * Py; // aponta para a primeira linha
	Tcaracter * Px;
	int status=1; //status do arquivo
	int i=0,j=0;
	char c=35;
	fseek (arq, 0, 0); //coloca o arquivo na posicao inicial

	remove(nome_arquivo);
	arq=fopen(nome_arquivo,"w+b");

	for ( Py = l->prim; i <=(l->qtd_de_linhas);i++){
		//printf("I: [%i]",i);
		//Px = primeiro Nó caracter 
		for (Px = Py->linha; j <(Py->qtd_caracter); j++){
			fputc(Px->letra, arq);		
			Px=Px->prox; //proximo caracter
		}
		j=0;//iniciar o for
		if(Px!=NULL){
			fprintf(arq, "\r\n");

		}Py= Py->prox;//aponta para proxima linha
	}
	fclose(arq);//fecha o arquivo

	system("cls");
	gotoxy(9,25);
	printf("Salvando...");
	for ( i = 15; i < 47; i++){
		for (j = 0; j < 100000000; j++){	

	}
		gotoxy(10,i);
		printf("%c",c);
	}
	

	gotoxy(12,18);
	printf("Arquivo Salvo Com Sucesso");

}
//montar as listas
//Ldescritor==NULL
//OK
void carregar(Ldescritor **l, FILE * arq){

	Tlinha * Py; // aponta para Nó linha	
	Tcaracter * Px;
	int y=-1;
	char ch;

	fseek (arq, 0, 0); // inicio do arquivo
	Py=(*l)->prim;
	Px=Py->linha;
	while(1){
		ch=fgetc(arq);
		if( feof(arq) ) {
			break ;
		}
		if(ch=='\n'){
		}else if(ch=='\r'){
			inserir_caracter_linha(Px,&Py,'\n',y);			
			inserir_linha(l,&Py,y); //insere a proxima linha
			Py=Py->prox;
			Px=Py->linha;
		}else {
			inserir_caracter_linha(Px,&Py,ch,y);
			if(Px==NULL)
				Px=Py->linha;
			else
				Px=Px->prox;			
		}
	}//fim do while
}
// passa a linha
//OK
void exibir(Tcaracter *l){
	Tcaracter * aux=l;

	if(aux==NULL)
		return;//vazio
	else{
		while (aux!=NULL){
			printf("%c", aux->letra);
			aux=aux->prox;
		}
	}
}

void exibir_texto(Ldescritor *l){
	Tcaracter * aux;
	Tlinha *aux2;
	int i=0;
	if(l==NULL)	
		return;	
	aux2=l->prim;
	do{
		aux=aux2->linha;
		exibir(aux);
		aux2=aux2->prox;
		i++;
	} while (i<=l->qtd_de_linhas);
}

