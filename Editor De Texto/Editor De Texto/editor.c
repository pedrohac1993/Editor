#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>
#define tam 256
#ifndef ESTRUTURAS
#define ESTRUTURAS
typedef struct letras {
	char caracter ;
	int status;
} Tletras;
#endif

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
void inserir_linha(Ldescritor **l);
void criar_no_linha(Tlinha ** novo);
void inserir_caracter_linha(FILE * arq, Tlinha **c, char letra);
void criar_no_caracter(Tcaracter **novo, char letra );
void inserir(Ldescritor **l, FILE * arq, char nome_arquivo[]);
//void exibir(Tcaracter *l);
void exibir_texto2(Ldescritor *l);
void exibir(Tcaracter *l);
void salvar(Ldescritor *l, FILE * arq);
void carregar(Ldescritor **l, FILE * arq);
//curso para alterar o texto
void gotoxy(int x, int y);
void mover_px(int qtd, Tcaracter ** Px);
void txt(char nome_arquivo[tam]);


void inserir_caracter_linha_versao_foda (Tcaracter *px,Tlinha **py, char c, int y);

int main(){

	char nome_arquivo[tam];
	FILE * arq=NULL;
	Ldescritor *l=NULL;
	setlocale(LC_ALL, "Portuguese");

	printf("\t\tBem vindo\n\t   Editor de Texto 1.0\n\nDigite O Nome Do Texto: ");
	gets(nome_arquivo) ;fflush(stdin);
	txt(nome_arquivo);

	system("cls");
	//inicializa o descritor
	inserir_linha(&l);
	arq = fopen (nome_arquivo, "r+b");
	if (arq == NULL) {
		
		arq = fopen (nome_arquivo, "w+b");
		
	}else{
			
		carregar(&l,arq);
		exibir_texto2(l);

	}

	//exibir(l->prim->linha);
	
	
	//arq=criarArquivo(nome_arquivo,&l);

	inserir(&l,arq,nome_arquivo);

	return 0;
}

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
void inserir_linha(Ldescritor **l){

	Tlinha *novo=NULL;
	if((*l)==NULL){
		//cria a primeira linha
		criar_no_linha(&novo);
		novo->ant=novo;
		novo->prox=novo;
		//cria o descridor das linhas
		(*l)=(Ldescritor*)malloc(sizeof(Ldescritor));
		(*l)->prim=novo;
		(*l)->ult=novo;
		(*l)->qtd_de_linhas=0;


	}else{
		//cria a proxima linha
		criar_no_linha(&novo);

		(*l)->ult->prox=novo;
		novo->ant=(*l)->ult;
		(*l)->ult=novo;
		novo->prox=(*l)->prim;
		(*l)->prim->ant=novo;

		((*l)->qtd_de_linhas)++;

	}
}


// passa o nó que sera criado a linha de caracter
void inserir_caracter_linha(FILE * arq, Tlinha **c, char letra){


	Tcaracter * novo, *aux;


	if((*c)->linha==NULL){
		criar_no_caracter(&novo,letra);
		novo->ant=novo;
		novo->prox=novo;
		(*c)->linha=novo;
		(*c)->qtd_caracter=1;

	}else{
		aux=(*c)->linha;
		do{
			if(aux->prox==(*c)->linha){
				criar_no_caracter(&novo,letra);

				aux->prox=novo;
				novo->ant=aux;
				novo->prox=(*c)->linha;
				(*c)->linha->ant=novo;
				(*c)->qtd_caracter++;
				return;

			}else{
				aux=aux->prox;
			}
		}while(1);

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


void inserir_caracter_linha_versao_foda (Tcaracter *px,Tlinha **py, char c, int y){

	Tcaracter * novo, *aux;

	//insere no inicio
	if((*py)->linha==NULL){
		
		criar_no_caracter(&novo,c);
		novo->ant=novo;
		novo->prox=novo;
		(*py)->linha=novo;
		(*py)->qtd_caracter=1;
			//printf("AQUI1");
	}
	else {//insere no fim
		if (px->prox == (*py)->linha){

			criar_no_caracter(&novo,c);

			px->prox=novo;
			novo->ant=px;
			novo->prox=(*py)->linha;
			(*py)->linha->ant=novo;
			(*py)->qtd_caracter++;
			
				//printf("AQUI2");
		}//insere no inicio		
		else if(px==(*py)->linha && y==0){
			criar_no_caracter(&novo,c);
			(*py)->linha=novo;
			novo->prox=px;
			novo->ant=px->ant;
			px->ant->prox=novo;
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
			//printf("AaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaQUI2");
			
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
	int x=(*l)->qtd_de_linhas,y=(*l)->ult->qtd_caracter ;
	Tlinha * Py=(*l)->ult;
	Tcaracter * Px=Py->linha;

	if(Py->prox != Py ){
		
		
		x = (*l)->qtd_de_linhas;
		y=Py->qtd_caracter;
		Px = Py->linha->ant;
		gotoxy(x,y);
	
	}

	if(Px==NULL){
		Px=Py->linha;
	
	}else{
		Px=Py->linha->ant;
	}

	do{
		c=_getch();fflush(stdin);
		//ctrl+s
		if(c==19){
			
			salvar(*l,arq);
			arq=fopen(nome_arquivo,"r+b");
			if (arq==NULL)
				printf("Erro De Abertura do Arquivo");
			//printf("Salvo");
			//ctrl+e
		}else if(c==5){
			return;
			//enter
		}else if(c==13){
			
			inserir_caracter_linha_versao_foda(Px,&Py,'\n',y);
			inserir_linha(l);
			x=(*l)->qtd_de_linhas;
			y=0;
			gotoxy(x,y);
			Py=(*l)->ult;
		}else{



			if( c == -32){

			}
			else {
				//cima
				if (c == 72){
					//printf("X1: %i Y1: %i",x,y);
					if(x>0){
						//printf("prox: %i ant: %i",Py->qtd_caracter,Py->ant->qtd_caracter);

						if(y<=Py->ant->qtd_caracter){
							//printf("X1: %i Y1: %i",x,y);
							x--;
							gotoxy(x,y);
							Py=Py->ant;
							//
							Px=Py->linha;
							mover_px(y,&Px);
							
							//printf("X1: %i Y1: %i",x,y);
						}else {
							y=Py->ant->qtd_caracter - 1;
							//printf("X2: %i Y2: %i",x,y);
							x--;
							gotoxy(x,y);
							Py=Py->ant;
							

							Px=Py->linha;
							mover_px(y,&Px);
							if(Px->letra == '\n'){
								y--;
								mover_px(y,&Px);
							}
						}
					}

				}//baixo
				else if( c == 80){
					//printf("X2: %i Y2: %i",x,(*l)->qtd_de_linhas);
					if(x<(*l)->qtd_de_linhas){
						//printf("prox: %i ant: %i",Py->qtd_caracter,Py->ant->qtd_caracter);

						if(y<=Py->prox->qtd_caracter){
							//printf("X1: %i Y1: %i",x,y);
							x++;
							//y=Py->qtd_caracter;
							gotoxy(x,y);
							Py=Py->prox;

							Px=Py->linha;
							mover_px(y,&Px);

						}else{
							y=Py->prox->qtd_caracter;
							//printf("X2: %i Y2: %i",x,y);
							x++;
							gotoxy(x,y);
							Py=Py->prox;

							Px=Py->linha;
							mover_px(y,&Px);
							
						}

					}


				}else if( c== 75){
					//esquerda
					//printf("Aqui");
					if(y>1){
						y--;
						gotoxy(x,y);
						Px=Px->ant;
						//printf("X1: %i Y1: %i PX: %c",x,y,Px->letra);
					}else if(y==1){
						y--;
						gotoxy(x,y);
						//printf("X1: %i Y1: %i PX: %c",x,y,Px->letra);
					}
				}else if( c == 77 ){
					//direita
					//printf("Py: %i Y: %i",Py->qtd_caracter,y);
					if(Px->prox->letra != '\n' && y < Py->qtd_caracter ){
						y++;
						gotoxy(x,y);
						Px=Px->prox;
						//printf("X2: %i Y2: %i PX: %c",x,y,Px->letra);
					}



				}else if(isalnum(c) || c == ' '){


					inserir_caracter_linha_versao_foda(Px,&Py,c,y);
					printf("%c",c);


					y++;

					if(Py->linha->ant == Py->linha->prox){
						Px=Py->linha->ant;
						
					}else{
						if(y==1)
							Px=Px->ant;
						else
						Px=Px->prox;
					
					}
					
						system("cls");
					exibir_texto2(*l);
					
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

	

	//printf("PX: %c",(*Px)->letra);

}


void salvar(Ldescritor *l, FILE * arq){
	Tlinha * Py; // aponta para a primeira linha
	Tcaracter * Px;
	Tletras c;
	int status=1; //status do arquivo
	int i=0,j=0;

	fseek (arq, 0, 0); //coloca o arquivo na posicao inicial
	//Py = primeira Nó linha

	for ( Py = l->prim; i <=(l->qtd_de_linhas);i++){
		//printf("I: [%i]",i);
		//Px = primeiro Nó caracter 
		for (Px = Py->linha; j <(Py->qtd_caracter); j++){
			//printf("\nJ: [%i]",j);
			c.caracter=Px->letra; // recebe o caracter do texto
			c.status = 1;
			//status = fwrite (&c, sizeof (Tletras), 1, arq);

			if(Px->letra=='\n'){
				fprintf(arq, "\r\n");			
			}else
				fputc(Px->letra, arq);

			if (status != 1){
				printf ("Erro de gravacao \n");
			}else{
				//printf ("Letra gravada com sucesso \n");
				Px=Px->prox; //proximo caracter
			}
		}
		j=0;//iniciar o for
		Py= Py->prox;//aponta para proxima linha
	}


	fclose(arq);//fecha o arquivo
}

//montar as listas
//Ldescritor==NULL
void carregar(Ldescritor **l, FILE * arq){
	Tlinha * Py; // aponta para Nó linha
	//Tletras c; //estrutura para manipulação do arquivo
	int status=1; //status do arquivo
	char ch;
	fseek (arq, 0, 0); // inicio do arquivo
	Py=(*l)->prim;
	while(1){

		ch=fgetc(arq);

		if( feof(arq) ) {
			break ;
		}
		//  printf("%c", ch);

		if(ch=='\n'){


		}else if(ch=='\r'){
			inserir_caracter_linha(arq,&Py,'\n');
			inserir_linha(l); //insere a proxima linha
			Py=(*l)->ult;
		}else {
			inserir_caracter_linha(arq,&Py,ch);
		}






	}//fim do while
}











// passa a linha
void exibir(Tcaracter *l){
	Tcaracter * aux=l;
	if(aux==NULL)
		return;//vazio
	else{
		do{
			printf("%c", aux->letra);
			aux=aux->prox;

		} while (aux!=l);
	}
}



void exibir_texto (Ldescritor *l) {
	Tlinha * Py; // aponta para a primeira linha
	Tcaracter * Px;
	int i=1,j=1;

	for ( Py = l->prim; i <=(l->qtd_de_linhas);i++){
		//Px = primeiro Nó caracter 
		for (Px = Py->linha; j <= (Py->qtd_caracter); j++){
			printf("%c",Px->letra);
			Px=Px->prox; //proximo caracter
		}
		Py= Py->prox;//aponta para proxima linha
	}
}




void exibir_texto2(Ldescritor *l){
	Tcaracter * aux;
	Tlinha *aux2;
	int i=0;
	if(l==NULL){
		printf("vazio\n");
		return;
	}
	aux2=l->prim;
	do{
		aux=aux2->linha;
		exibir(aux);
		aux2=aux2->prox;
		i++;
	} while (i<=l->qtd_de_linhas);
}

