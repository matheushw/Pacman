/*
*   Modelagem Computacional em Grafos
*
*   Aluno: Matheus Barcellos de Castro Cunha
*   NUSP: 11208238
*
*	Aluna: Maria Eduarda Kawakami Moreira
*	NUSP: 11218751
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXN 4

typedef struct element{ //Estrutura criada para armazenar elementos da matriz na fila.
	int x;
	int y;
	int camada;
	struct element *prox;
} ELEMENT;

typedef struct { //Estrutura criada para representar a fila.
	int size;
	ELEMENT *inicial;
	ELEMENT *final;
} QUEUE;

int GenerateRandoms(int upper) { //Função que gera um número aleatorio entre 0 e "upper".
    int num = (rand() % upper) + 1; 
    return num;
}

int menor(int a, int b) { //Retorna o menor valor entre "a" e "b".
	return (a > b)? b : a;
}

int min_vizinho(int **mat, int i, int j){ //Retorna o valor do vizinho de menor valor.
	int min = 99999999;
	min = (i<(MAXN-1))? menor(mat[i+1][j],min):min; //Baixo
	min = (i>0)? menor(mat[i-1][j],min):min; //Cima
	min = (j<(MAXN-1))? menor(mat[i][j+1],min):min; //Direita
	min = (j>0)? menor(mat[i][j-1],min):min; //Esquerda

	return min;
}

int contar_maior(int **mat, int minimo, int i, int j){ //Conta quantos vizinhos de valor "minimo" existem.
	int c = 0;
	if(i<(MAXN-1)){
		mat[i+1][j]==minimo? c++ : c; //Baixo
	}
	if(i>0){
		mat[i-1][j]==minimo? c++ : c; //Cima
	}
	if(j<(MAXN-1)){
		mat[i][j+1]==minimo? c++ : c; //Direita
	}
	if(j>0){
		mat[i][j-1]==minimo? c++ : c; //Esquerda
	}

	return c;
}

ELEMENT *criar_elemento(int x, int y, int camada){ //Aloca memória para um "ELEMENTO".
	ELEMENT *aux = (ELEMENT*)malloc(sizeof(ELEMENT));
	aux->x = x;
	aux->y = y;
	aux->camada = camada;
	aux->prox = NULL;

	return aux;
} 

QUEUE *criar_queue(){ //Aloca memória para uma "QUEUE"(Fila).
	QUEUE *aux = (QUEUE*)malloc(sizeof(QUEUE));
	aux->size = 0;
	aux->inicial = NULL;
	aux->final = NULL;

	return aux;
}

void limpar_fila(QUEUE *q){ //Desalocar memória.
	ELEMENT *aux = q->inicial;
	while(aux){
		ELEMENT *aux2 = aux->prox;
		free(aux);
		aux = aux2;
	}
	free(q);
}

void print_queue(QUEUE *q){ //Função criada para debugar.
	ELEMENT *aux = q->inicial;
	while(aux){
		printf("linha: %d||coluna: %d||camada: %d\n",aux->y,aux->x,aux->camada);
		aux = aux->prox;
	}

	return;
}

void insert(QUEUE *q, int x, int y, int camada){ //Inserir um elemento na fila.
	ELEMENT *aux = criar_elemento(x,y,camada);

	if(q->size == 0){
		q->inicial = aux;
		q->final = aux;
	} else {
		q->final->prox = aux;
		q->final = aux;
	}

	q->size = q->size + 1;

	return;
}

void pop_first(QUEUE *q){ //Excluir o primeiro elemento da fila.

	if(q->size >= 1){
		if(q->size == 1){
			free(q->inicial);
			q->inicial = NULL;
			q->final = NULL;
		} else {
			ELEMENT *aux = q->inicial->prox;
			free(q->inicial);
			q->inicial = aux;
		}

		q->size = q->size - 1;	
	}

	return;
}

void print_matriz(int **mat, int x_f, int y_f, int x_p, int y_p){ //Printa o campo (matriz).
	for (int i=0;i<MAXN;i++){
		printf("|");
		for (int j=0;j<MAXN;j++){
			if(x_p == j && y_p == i) {
				printf("pm|");
			} else if(x_f == j && y_f == i){
				printf("gh|");
			} else if (mat[i][j]>=1 && mat[i][j]<=9){
				printf("0%d|", mat[i][j]);
			} else {
				printf("%d|", mat[i][j]);
			}
		}
		printf("\n");
	}
}

void visitar(int *i, int *j, int**mat, int maior, int random){ //Visita um dos vizinhos da posição dada.
	int cont = 0;
	if(*i>0 && mat[(*i)-1][*j] == maior) { //Cima
		cont++;
		if(cont == random) {
			*i = (*i) - 1;
			return;
		}
	}
	if(*j<(MAXN-1) && mat[*i][(*j)+1] == maior) { //Direita
		cont++;
		if(cont == random) {
			*j = (*j) + 1;
			return;
		}
	} 
	if(*i<(MAXN-1) && mat[(*i)+1][*j] == maior) { //Baixo
		cont++;
		if(cont == random) {
			*i = (*i) + 1;
			return;
		}
	}
	if(*j>0 && mat[*i][(*j)-1] == maior) { //Esquerda
		cont++;
		if(cont == random) {
			*j = (*j) - 1;
			return;
		}
	} 

	return;
}

void percorrer(int *x, int *y, int**mat){ //Mudar a posição do pacman com base nos seus vizinhos.

	int max_viz = min_vizinho(mat, *y, *x);
	int qnt_viz = contar_maior(mat, max_viz, *y, *x);
	int random = GenerateRandoms(qnt_viz);
	visitar(y, x, mat, max_viz, random);

	return;
}

void wavefront(int **mat, int x, int y, QUEUE *q){ //Função para realizar o wavefront.

	if(y<(MAXN-1) && mat[y+1][x]==-1){ //Baixo
		insert(q, x, y+1, q->inicial->camada+1);
		mat[y+1][x] = q->inicial->camada+1;
	} if(y>0 && mat[y-1][x]==-1){ //Cima
		insert(q, x, y-1, q->inicial->camada+1);
		mat[y-1][x] = q->inicial->camada+1;
	} if(x<(MAXN-1) && mat[y][x+1]==-1){ //Direita
		insert(q, x+1, y, q->inicial->camada+1);
		mat[y][x+1] = q->inicial->camada+1;
	} if(x>0 && mat[y][x-1]==-1){ //Esquerda
		insert(q, x-1, y, q->inicial->camada+1);
		mat[y][x-1] = q->inicial->camada+1;
	} 

	pop_first(q);

	if(q->size == 0) return;

	wavefront(mat, q->inicial->x, q->inicial->y, q);

	return;
}

int **alocar_matriz(){ //Alocando memória para a matriz que servirá como campo.
	int **aux = (int**)malloc(sizeof(int*) * MAXN);
	for (int i=0;i<MAXN;i++){
		aux[i] = (int*)malloc(sizeof(int) * MAXN);
	}

	for (int i=0;i<MAXN;i++){
		for (int j=0;j<MAXN;j++){
			aux[i][j]=-1;
		}
	}
	return aux;
}

int main () {

	srand(time(0));	

	int **mat = alocar_matriz();
	int *x_fantasma = malloc(sizeof(int)); 
	int *y_fantasma = malloc(sizeof(int));

	int *x_pacman = malloc(sizeof(int)); 
	int *y_pacman = malloc(sizeof(int));
	//Pacman sempre começa no canto superior esquerdo da matriz.
	*x_pacman = 0;
	*y_pacman = 0;

	//A posição do fantasma, que será controlado pelo usuário, será também escolhida pelo usuário.
	printf("Determine a linha e a coluna do fantasma no campo:\n");
	scanf("%d %d", x_fantasma, y_fantasma);

	//TO DO cunha acho q aqui n precisa ter comentario, vc pode colocar na função do wavefront
	//cm funciona e cm usa queue
	QUEUE *q = criar_queue();
	mat[*y_fantasma][*x_fantasma] = 0;
	insert(q, *x_fantasma, *y_fantasma, 0); //Inserindo o fantasma na fila.
	wavefront(mat, *x_fantasma, *y_fantasma, q); //Fazer o wavefront a partir do fantasma e usando a fila "q".

	// Loop até que o Pacman chegue no fantasma.
	while(*x_fantasma != *x_pacman || *y_fantasma != *y_pacman){

		print_matriz(mat, *x_fantasma, *y_fantasma, *x_pacman, *y_pacman);
		for (int i=0;i<MAXN;i++){
			printf("---");
		}
		printf("-\n");

		percorrer(x_pacman, y_pacman, mat); 

		char comando;
		scanf("\n%c", &comando);


		//Movimentando o pacman pela matriz recebendo o comando do usuário.
		if(comando == 's' && (*y_fantasma)<(MAXN-1)){ //Baixo
			(*y_fantasma) = (*y_fantasma) + 1;
		} else if(comando == 'w' && (*y_pacman)>0) { //Cima
			(*y_fantasma) = (*y_fantasma) - 1;
		} else if(comando == 'd' && (*x_pacman)<(MAXN-1)) { //Direita
			(*x_fantasma) = (*x_fantasma) + 1;
		} else if(comando == 'a' && (*x_pacman)>0) { //Esquerda
			(*x_fantasma) = (*x_fantasma) - 1;
		}

		insert(q, *x_fantasma, *y_fantasma, 0);
		//TO DO pq tem esse for for aqui? 
		for (int i=0;i<MAXN;i++){
			for (int j=0;j<MAXN;j++){
				mat[i][j] = -1;
			}
		}
		mat[*y_fantasma][*x_fantasma] = 0;
		//Refaz a matriz do wavefront a cada vez que o fantasma se move.
		wavefront(mat, *x_fantasma, *y_fantasma, q);
		
	}

	print_matriz(mat, *x_fantasma, *y_fantasma, *x_pacman, *y_pacman);
	
	//Frees
	free(x_fantasma);
	free(y_fantasma);
	free(x_pacman);
	free(y_pacman);
	limpar_fila(q);
	for (int i=0;i<MAXN;i++){
		free(mat[i]);
	}
	free(mat);
	printf("Perdeu!\n");

	return 0;
}
