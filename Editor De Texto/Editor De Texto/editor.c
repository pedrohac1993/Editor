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
void salvar(Ldescritor *l, FILE * arq);
void carregar(Ldescritor **l, FILE * arq);
void txt(char nome_arquivo[tam]);
//curso para alterar o texto
void gotoxy(int x, int y);
void mover_px(int qtd, Tcaracter ** Px);

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
void inserir_linha(Ldescritor **l, Tlinha ** Py, int y){

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
		//insere no fim
		if ((*Py)->prox == NULL){

			criar_no_linha(&novo);
			(*Py)->prox=novo;
			novo->ant=(*Py);
			novo->prox=NULL;
			(*l)->qtd_de_linhas++;
			//insere no inicio com nos já inseridos
		}else if((*Py)->ant==NULL && y==0){
			criar_no_linha(&novo);
			(*l)->prim=novo;
			novo->prox=(*Py);
			(*Py)->ant=novo;
			(*l)->qtd_de_linhas++;
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
		criar_no_caracter(&novo,c);		
		(*py)->linha=novo;
		(*py)->qtd_caracter=1;			
	}
	else {
		if(px->letra=='\n'){
			px=px->ant;				
		}
		//insere no fim
		if (px->prox == NULL){		
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
			novo->ant=px->ant;
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
	Tlinha * Py=(*l)->prim;
	Tcaracter * Px=Py->linha;

	gotoxy(0,0);//inicio do texto

	do{
		c=_getch();fflush(stdin);

		//ctrl+s
		if(c==19){
			salvar(*l,arq);
			arq=fopen(nome_arquivo,"r+b");
			if (arq==NULL)
				printf("Erro De Abertura do Arquivo");
			//ctrl+e
		}else if(c==5){
			return;
			//enter
		}else if(c==13){
			inserir_caracter_linha(Px,&Py,'\n',y);
			inserir_linha(l,&Py,y);
			x++;//ando com curso
			y=0;
			gotoxy(x,y);
			Py=Py->prox;
			Px=Py->linha;
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
							//printf("X1: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}else{
							y=Py->ant->qtd_caracter;
							x--;
							gotoxy(x,y);
							Py=Py->ant;

							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);
							//printf("X2: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
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
							//printf("X1: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}else{
							y=Py->prox->qtd_caracter;
							x++;
							gotoxy(x,y);
							Py=Py->prox;

							Px=Py->linha;
							if(y==0 || y==1){
							}else
								mover_px(y,&Px);
							//printf("X2: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}
					}
					//esquerda
				}else if( c== 75){

					if(y>0){

						if(y==1){
							y--;
							gotoxy(x,y);
							//printf("X1: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}else{
							y--;
							gotoxy(x,y);
							Px=Px->ant;
							//printf("X1: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}
					}
					//direita
				}else if( c == 77 ){

					if(y<Py->qtd_caracter){

						if(y==0){
							y++;
							gotoxy(x,y);
							//printf("X3: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}else{
							y++;
							gotoxy(x,y);
							Px=Px->prox;
							//printf("X3: %i Y1: %i PX: %c Py: %i",x,y,Px->letra,Py->qtd_caracter);
						}
					}
				}else if(isalnum(c) || c == ' '){

					inserir_caracter_linha(Px,&Py,c,y);
					printf("%c",c);

					y++;//ando com o curso

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


void salvar(Ldescritor *l, FILE * arq){
	Tlinha * Py; // aponta para a primeira linha
	Tcaracter * Px;
	int status=1; //status do arquivo
	int i=0,j=0;

	fseek (arq, 0, 0); //coloca o arquivo na posicao inicial
	

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
			
			//printf("Letra: %c",Px->letra);
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

