#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 401
typedef struct produto {
	char codigo [12];
	char descricao [100];
	float preco;
	int estoque;
	int status;
} TProduto;

typedef struct noTabela {
	char codigo [12];
	int posicao;
	struct noTabela * prox;
} TNo;

FILE * abrirArquivo (char nome[]);
void fecharArquivo (FILE * arq);
void cadastrar (FILE * arq, TNo * tabelaHashing []);
void remover (FILE * arq, TNo * tabelaHashing []);
void consultar (FILE * arq, TNo * tabelaHashing []);
void alterar (FILE * arq, TNo * tabelaHashing []);
void criarIndice (FILE * arq, TNo * tabelaHashing []);
void destruirIndice(TNo * tabelaHashing []);
int buscarTabelaHash (TNo * tabelaHashing , char codigo []);
void inserirTabelaHash (TNo ** tabelaHashing , char codigo [], int pos);
void removerTabelaHash (TNo ** tabelaHashing , char codigo []);
int hashing (char codigo []);
void criar_no(TNo ** novo, char cod[], int pos );
void listar(TNo * tabelaHashing []);

int main ( ) {
	char op, nomeArq[] = "produtos.dat";
	FILE * cadastro;
	TNo * tabelaHashing [N]={{NULL}}; 
	cadastro = abrirArquivo (nomeArq);
	if (cadastro == NULL)
		printf ("Erro na abertura do arquivo. Programa encerrado \n");
	else {
		criarIndice (cadastro,tabelaHashing);
		do {
			printf ("1 - Cadastrar produto \n2 - Consultar produto \n3 - Alterar dados do produto \n4 - Remover produto \n5 - Encerrar programa\n");
			printf ("Informe a opcao: ");
			op = getchar (); fflush (stdin);
			switch(op) {
			case '1': cadastrar (cadastro, tabelaHashing);
				break;
			case '2': consultar (cadastro, tabelaHashing);
				break;
			case '3': alterar (cadastro, tabelaHashing);
				break;
			case '4': remover (cadastro, tabelaHashing);
				break;
			case '5': fecharArquivo (cadastro);
				destruirIndice(tabelaHashing);
				break;
			default: printf ("Opcao invalida \n");
			}
		} while (op != '5');
	}
	return 0;
}

void criar_no(TNo ** novo, char cod[], int pos ){
	*novo=(TNo*)malloc(sizeof(TNo));
	strcpy((*novo)->codigo,cod);
	(*novo)->posicao=pos;
	(*novo)->prox=NULL;
}

FILE * abrirArquivo (char nome[]) {
	FILE * aux;
	aux = fopen (nome, "r+b");
	if (aux == NULL)
		aux = fopen (nome, "w+b");
	return aux;
}

void fecharArquivo (FILE * arq) {
	int status;
	status = fclose (arq);
	if (status != 0)
		printf ("Erro ao tentar fechar o arquivo \n");
}

void criarIndice (FILE * arq, TNo * tabelaHashing []) {
	/* preencher a tabela de hashing com as chaves dos registros que est�o armazenados no arquivo
	1 - Ler o arquivo registro a registro at� o final
	2 - Para cada registro lido, aplicar a fun��o de hashing na chave (ou seja, no codigo)
	3 - O resultado da fun��o indica a posi��o na tabela onde a chave ser� inserida
	4 - Criar n�, preencher com a chave e a posi��o dela no arquivo e inserir na tabela, na lista encadeada correspondente, de forma que a lista permane�a ordenada
	*/
	TProduto p;

	int status, cont=0, pos;

	while (1) {
		status = fread (&p, sizeof (TProduto), 1,arq);
		if (status == 0) {
			if (feof(arq) != 0){				
				break;
			}else
				printf("Erro de leitura\n");
			break;
		}
		else {
			pos=hashing(p.codigo);
			inserirTabelaHash(&tabelaHashing[pos],p.codigo,cont);
			cont++;
		}
	}
}

int buscarTabelaHash (TNo * tabelaHashing , char codigo []) {
	/* procurar na tabela de hashing o codigo desejado e retornar a posi��o do produto no arquivo
	1 - aplicar a fun��o de hashing na chave (ou seja, no codigo)
	2 - procurar a chave na lista indicada pelo resultado da fun��o (usar busca sequencial melhorada)
	3 - caso encontre, retornar a posi��o da chave no arquivo
	4 - caso n�o encontre, retornar -1 */
	
	TNo * aux1=tabelaHashing;

	tabelaHashing[hashing(codigo)];

	if(tabelaHashing == NULL)
		return -1;
	else if(strcmp(tabelaHashing->codigo,codigo)==0)
		return tabelaHashing->posicao;	
	else if(strcmp(tabelaHashing->codigo,codigo)>0)
		return -1;
	else{
		aux1=aux1->prox;
		while (aux1 != NULL){

			
			if(strcmp(aux1->codigo,codigo)==0){
				return aux1->posicao;
			}else if(strcmp(aux1->codigo,codigo)>0)
				return -1;
			else{
				aux1=aux1->prox;
			}


		}

		return -1;


	}



}

void inserirTabelaHash (TNo ** tabelaHashing, char codigo [], int pos) {
	/* Inserir na tabela hashing, na lista encadeada indicada pela fun��o de hashing, uma chave e sua posi��o no arquivo.
	1 - Aplicar a fun��o de hashing � chave (ou seja, ao c�digo)
	2 - O resultado da fun��o indica a posi��o na tabela onde a chave ser� inserida
	3 - Criar n�, preencher com a chave e a posi��o dela no arquivo e inserir na tabela, na lista encadeada correspondente, de forma que a lista permane�a ordenada
	*/

	TNo * aux1=*tabelaHashing,*novo,* aux2;

	if(*tabelaHashing == NULL){
		criar_no(&novo,codigo,pos);
		*tabelaHashing=novo;
		return; 
	}else if(strcmp(codigo,aux1->codigo)<0){
		criar_no(&novo, codigo,pos);
		*tabelaHashing=novo;
		novo->prox=aux1;	
		return;
	}else{
		aux1=*tabelaHashing;
		aux2=(*tabelaHashing)->prox;
		while (aux2!=NULL){

			 if(strcmp(aux2->codigo,codigo)>0){
				criar_no(&novo, codigo,pos);
				aux1->prox=novo;
				novo->prox=aux2;
				return;			
			}else{
				aux1=aux2;
				aux2=aux2->prox;
			}
		} 
		criar_no(&novo, codigo,pos);
		aux1->prox=novo;
	}
}

void removerTabelaHash (TNo ** tabelaHashing , char codigo []) {
	/* Remover da tabela hashing, da lista encadeada indicada pela fun��o de hashing, o n� que cont�m uma determinada chave.
	1 - Aplicar a fun��o de hashing � chave (ou seja, ao c�digo)
	2 - O resultado da fun��o indica a posi��o na tabela onde a chave ser� removida
	3 - Procurar o n� que cont�m a chave na lista indicada pela fun��o de hashing utilizando busca sequencial melhorada e remov�-lo
	*/
	TNo * aux1, * aux2, *aux;
	int i=0, h;

	if(strcmp((*tabelaHashing)->codigo,codigo)==0){//remocao no inicio
		aux1=*tabelaHashing;
		(*tabelaHashing)=(*tabelaHashing)->prox;
		free(aux1);
		return;	
	}else{
		aux1=*tabelaHashing;
		aux2=(*tabelaHashing)->prox;
		do{

			if(strcmp(aux2->codigo,codigo)==0){
				aux1->prox=aux2->prox;
				free(aux2);
				aux=(*tabelaHashing);	

				return;			
			}else{
				aux1=aux2;
				aux2=aux2->prox;
			}


		}while (aux2->prox!=NULL);
	}

}

int hashing (char codigo []) {
	/* A fun��o �hashing� recebe com par�metro a chave (ou seja, o codigo) e retorna o valor calculado segundo o m�todo da divis�o inteira 
	para chaves alfanum�ricas (visto em sala).
	*/
	int i, soma = 0, tam = strlen(codigo);
	for (i = 0; i < tam; i++)
		soma = soma + codigo[i];
	return (soma % N);
}

void cadastrar (FILE * arq, TNo * tabelaHashing []) {
	/* Cadastrar um novo produto. O c�digo � a chave prim�ria do arquivo, portanto n�o pode existir mais de um produto com um mesm c�digo.
	Inserir o registro do produto no arquivo e efetuar a inser��o da entrada correspondente na tabela de hashing.
	1 - Solicitar o c�digo do produto   
	2 - Fazer uma busca na tabela de hashing para verificar se o c�digo j� � cadastrado. Utilizar a fun��o buscarTabelaHash
	3 - Caso o c�digo conste na tabela de hashing, informar que se trata de produto j� cadastrado.
	4 - Caso o c�digo n�o conste na tabela de hashing, proceder a inser��o do produto no arquivo
	4.1 - Solicitar as demais informa��es do produto
	4.2 - Gravar o registro no final do arquivo e inserir a entrada correspondente na tabela de hashing utilizando o procedimento inserirTabelaHash   
	*/
	char cod[12];
	int pos,status,cont=0,h;
	TProduto p;

	printf("Digite o codigo: ");
	gets(cod);fflush(stdin);
	pos=hashing(cod);

	if(buscarTabelaHash(tabelaHashing[pos],cod) == -1){
		strcpy(p.codigo,cod);
		printf("Digite o descricao: ");
		fgets(p.descricao,100,stdin);fflush(stdin);
		printf("Digite o preco: ");
		scanf("%f", &p.preco);fflush(stdin);
		printf("Digite o estoque: ");
		scanf("%i", &p.estoque);fflush(stdin);
		p.status=1;
	
		fseek(arq, 0, SEEK_END);		
		cont=ftell(arq)/sizeof(TProduto);


		fseek (arq, 0, 2);

		status = fwrite (&p, sizeof (TProduto), 1, arq);
		if (status != 1)
			printf ("Erro de gravacao \n");
		else{
			printf ("Contato cadastrado com sucesso \n");
			
			inserirTabelaHash(&tabelaHashing[pos],p.codigo,cont);
		}


	}else{
		printf("Codigo ja Cadastrado\n");
	}

	fclose(arq);
	arq=fopen("produtos.dat","r+b");


}

void consultar (FILE * arq, TNo * tabelaHashing []) {
	/* Exibir os dados de um dado produto.
	1 - Solicitar o c�digo do produto
	2 - Fazer uma busca na tabela de hashing. Utilizar a fun��o buscarTabelaHash.
	3 - Caso n�o encontre o c�digo, informar que o produto n�o est� cadastrado.
	4 - Caso encontre, fazer a leitura no arquivo na posi��o indicada pela tabela de hashing
	5 - Exibir os dados lidos
	*/


	char cod[12];
	int pos=0,status=0,h;

	TProduto p;

	printf("Digite o codigo: ");
	gets(cod);fflush(stdin);
	h=hashing(cod);
	pos=buscarTabelaHash(tabelaHashing[h],cod);
	
	if(pos != -1){

		fseek(arq, pos * sizeof (TProduto), 0);
		status = fread (&p,sizeof (TProduto), 1, arq);

		if (status != 1)
			printf ("Erro de leitura \n");
		else {
			if(p.status==1){
				//printf("Codigo: %s\n", p.codigo);	
				printf("\n\nDescricao: %s",p.descricao);		
				printf("Preco: %.3f\n",p.preco);		
				printf("Estoque: %i\n\n",p.estoque);
			}else{
				printf("Produto Nao Cadastrada\n");
			}
		}	
	}else{
		printf("Produto Nao Cadastrada\n");
	}
}

void alterar (FILE * arq, TNo * tabelaHashing []) {
	/* Alterar o pre�o ou a quantidade em estoque de um dado produto.
	1 - Solicitar o c�digo do produto
	2 - Fazer uma busca na tabela de hashing. Utilizar a fun��o buscarTabelaHash.
	3 - Caso n�o encontre o c�digo, informar que o produto n�o est� cadastrado.
	4 - Caso encontre, fazer a leitura no arquivo na posi��o indicada pela tabela de hashing
	5 - Alterar os dados
	6 - Gravar os dados alterados
	*/
	TProduto p;
	char cod[12];
	int pos=0,status=0,h;

	printf("Digite o codigo: ");
	gets(cod);fflush(stdin);
	h=hashing(cod);
	pos=buscarTabelaHash(tabelaHashing[h],cod);

	if(pos!=-1){

		fseek(arq, pos * sizeof (TProduto), 0);
		status = fread (&p,sizeof (TProduto), 1, arq);
		if (status != 1)
			printf ("Erro de leitura \n");
		else {
			printf("Digite o novo preco: ");
			scanf("%f", &p.preco);fflush(stdin);
			printf("Digite o novo estoque: ");
			scanf("%i", &p.estoque);fflush(stdin);

			fseek(arq, -sizeof(TProduto), 1);
			status = fwrite (&p,sizeof (TProduto), 1, arq);
			if (status != 1)
				printf ("Erro de gravacao \n");
			else
				printf ("Contato alterado com sucesso \n");
		}

	}else{
		printf("Produto Nao Cadastrada\n");
	}



}

void remover (FILE * arq, TNo * tabelaHashing []) {
	/* Remover um dado produto.
	1 - Solicitar o c�digo do produto
	2 - Fazer uma busca na tabela de hashing. Utilizar a fun��o buscarTabelaHash.
	3 - Caso n�o encontre o c�digo, informar que o produto n�o est� cadastrado.
	4 - Caso encontre, fazer a leitura no arquivo na posi��o indicada pela tabela de hashing
	5 - Alterar o status do produto de 1 (ativo) para 0 (removido)
	6 - Gravar o registro alterado
	*/

	TProduto p;
	char cod[12];
	int pos=0,status=0,h;

	printf("Digite o codigo: ");
	gets(cod);fflush(stdin);
	h=hashing(cod);
	pos=buscarTabelaHash(tabelaHashing[h],cod);

	if(pos!=-1){

		fseek(arq, pos * sizeof (TProduto), 0);
		status = fread (&p,sizeof (TProduto), 1, arq);
		if (status != 1)
			printf ("Erro de leitura \n");
		else {
			p.status = 0;
			fseek(arq, -sizeof(TProduto), 1);
			status = fwrite (&p,sizeof (TProduto), 1, arq);
			if (status != 1)
				printf ("Erro de gravacao \n");
			else{
				removerTabelaHash(&tabelaHashing[h],p.codigo);
				printf ("Produto removido com sucesso \n");
				
	}
		}

	}else{
		printf("Produto Nao Cadastrada\n");
	}

}

void destruirIndice(TNo * tabelaHashing []) {
	/* Desalocar todos os n�s das listas encadeadas que comp�em a tabela de hashing. */

	int i=0;
	TNo *aux;

	while(i<N){

		if(tabelaHashing[i] != NULL){			
			while (tabelaHashing[i]!=NULL){
				aux=tabelaHashing[i];
				tabelaHashing[i]=aux->prox;			
				free(aux);
			}					
		}else{
			i++;
		}
	}



}




