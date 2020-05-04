#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 8
#define MAXQ 1000

// mat[i+1][j]; //Baixo
// mat[i-1][j]; //Cima
// mat[i][j+1]; //Direita
// mat[i][j-1]; //Esquerda

typedef struct element{
	int x;
	int y;
	int camada;
	struct element *prox;
} ELEMENT;

typedef struct {
	int size;
	ELEMENT *inicial;
	ELEMENT *final;
} QUEUE;

ELEMENT *criar_elemento(int x, int y, int camada){
	ELEMENT *aux = (ELEMENT*)malloc(sizeof(ELEMENT));
	aux->x = x;
	aux->y = y;
	aux->camada = camada;
	aux->prox = NULL;

	return aux;
} 

QUEUE *cirar_queue(){
	QUEUE *aux = (QUEUE*)malloc(sizeof(QUEUE));
	aux->size = 0;
	aux->inicial = NULL;
	aux->final = NULL;

	return aux;
}

void print_queue(QUEUE *q){
	ELEMENT *aux = q->inicial;
	while(aux){
		printf("linha: %d||coluna: %d||camada: %d\n",aux->y,aux->x,aux->camada);
		aux = aux->prox;
	}

	return;
}

void insert(QUEUE *q, int x, int y, int camada){
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

void pop_first(QUEUE *q){

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

void print_matriz(int **mat){
	for (int i=0;i<MAXN;i++){
		for (int j=0;j<MAXN;j++){
			printf("%d|", mat[i][j]);
		}
		printf("\n");
	}
}

void wavefront(int **mat, int x, int y, QUEUE *q){

	if(y<(MAXN-1) && mat[y+1][x] && mat[y+1][x]==-1){ //Baixo
		insert(q, x, y+1, q->inicial->camada+1);
		mat[y+1][x] = q->inicial->camada+1;
	} if(y>0 && mat[y-1][x] && mat[y-1][x]==-1){ //Cima
		insert(q, x, y-1, q->inicial->camada+1);
		mat[y-1][x] = q->inicial->camada+1;
	} if(x<(MAXN-1) && mat[y][x+1] && mat[y][x+1]==-1){ //Direita
		insert(q, x+1, y, q->inicial->camada+1);
		mat[y][x+1] = q->inicial->camada+1;
	} if(x>0 && mat[y][x-1] && mat[y][x-1]==-1){ //Esquerda
		insert(q, x-1, y, q->inicial->camada+1);
		mat[y][x-1] = q->inicial->camada+1;
	} 

	pop_first(q);

	if(q->size == 0) return;

	wavefront(mat, q->inicial->x, q->inicial->y, q);

	return;
}

int **alocar_matriz(){
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

	int **mat = alocar_matriz();
	int x_fantasma, y_fantasma;

	scanf("%d %d", &x_fantasma, &y_fantasma);

	QUEUE *q = cirar_queue();
	mat[y_fantasma][x_fantasma] = 0;
	insert(q, x_fantasma, y_fantasma, 0);
	wavefront(mat, x_fantasma, y_fantasma, q);

	print_matriz(mat);

	// printf("%d\n", );

	return 0;
}