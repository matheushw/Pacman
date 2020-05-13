#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXN 4
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

int GenerateRandoms(int upper) { 
    int num = (rand() % upper) + 1; 
    return num;
}

int maior(int a, int b) {
    return (a < b)? b : a;
}

int max_vizinho(int **mat, int i, int j){
	int max = -1;
	max = (i<(MAXN-1))? maior(mat[i+1][j],max):max; //Baixo
	max = (i>0)? maior(mat[i-1][j],max):max; //Cima
	max = (j<(MAXN-1))? maior(mat[i][j+1],max):max; //Direita
	max = (j>0)? maior(mat[i][j-1],max):max; //Esquerda

	return max;
}

int contar_maior(int **mat, int maximo, int i, int j){
	int c = 0;
	if(i<(MAXN-1)){
		mat[i+1][j]==maximo? c++ : c; //Baixo
	}
	if(i>0){
		mat[i-1][j]==maximo? c++ : c; //Cima
	}
	if(j<(MAXN-1)){
		mat[i][j+1]==maximo? c++ : c; //Direita
	}
	if(j>0){
		mat[i][j-1]==maximo? c++ : c; //Esquerda
	}

	return c;
}

ELEMENT *criar_elemento(int x, int y, int camada){
	ELEMENT *aux = (ELEMENT*)malloc(sizeof(ELEMENT));
	aux->x = x;
	aux->y = y;
	aux->camada = camada;
	aux->prox = NULL;

	return aux;
} 

QUEUE *criar_queue(){
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

void print_matriz(int **mat, int x, int y){
	for (int i=0;i<MAXN;i++){
		printf("|");
		for (int j=0;j<MAXN;j++){
			if(mat[i][j] == -2) {
				printf("pm|");
			}
			else if (mat[i][j]>=0 && mat[i][j]<=9){
				printf("0%d|", mat[i][j]);
			} else if(x == j && y == i){
				printf("gh|");
			} else {
				printf("%d|", mat[i][j]);
			}
		}
		printf("\n");
	}
}

int CheckMatrix(int **mat){
	for (int i=0;i<MAXN;i++){
		for (int j=0;j<MAXN;j++){
			if (mat[i][j]!=-1) return 1;
		}
	}
	return 0;
}

void visitar(int *i, int *j, int**mat, int maior, int random){
	int cont = 0;
	if(*i>0 && mat[(*i)-1][*j] == maior) { //Cima
		cont++;
		if(cont == random) {
			mat[(*i)-1][*j] = -1;
			// printf("Cima. i:%d j: %d\n", (*i)-1, *j);
			*i = (*i) - 1;
			// print_matriz(mat);
			// printf("----------------\n");
			return;
		}
	}
	if(*j<(MAXN-1) && mat[*i][(*j)+1] == maior) { //Direita
		cont++;
		if(cont == random) {
			mat[*i][(*j)+1] = -1;
			// printf("Direita. i:%d j: %d\n", *i, (*j)+1);
			*j = (*j) + 1;
			// print_matriz(mat);
			// printf("----------------\n");
			return;
		}
	} 
	if(*i<(MAXN-1) && mat[(*i)+1][*j] == maior) {//Baixo
		cont++;
		if(cont == random) {
			mat[(*i)+1][*j] = -1;
			// printf("Baixo. i:%d j: %d\n", (*i)+1, *j);
			*i = (*i) + 1;
			// print_matriz(mat);
			// printf("----------------\n");
			return;
		}
	}
	if(*j>0 && mat[*i][(*j)-1] == maior) { //Esquerda
		cont++;
		if(cont == random) {
			mat[*i][(*j)-1] = -1;
			// printf("Esquerda. i:%d j: %d\n", *i, (*j)-1);
			*j = (*j) - 1;
			// print_matriz(mat);
			// printf("----------------\n");
			return;
		}
	} 

	return;
}

void percorrer(int *x, int *y, int**mat){

	int max_viz = max_vizinho(mat, *y, *x);
	int qnt_viz = contar_maior(mat, max_viz, *y, *x);
	int random = GenerateRandoms(qnt_viz);
	// printf("max_viz: %d || qnt_viz: %d || random: %d\n", max_viz, qnt_viz, random);
	visitar(y, x, mat, max_viz, random);
	// printf("y_fantasta(i): %d || x_fantasma(j): %d\n", *y, *x);

	return;
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

	srand(time(0));

	int **mat = alocar_matriz();
	int *x_fantasma = malloc(sizeof(int)); 
	int *y_fantasma = malloc(sizeof(int));

	int *x_pacman = malloc(sizeof(int)); 
	int *y_pacman = malloc(sizeof(int));
	*x_pacman = 0;
	*y_pacman = 0;

	scanf("%d %d", x_fantasma, y_fantasma);

	QUEUE *q = criar_queue();
	mat[*y_fantasma][*x_fantasma] = 0;
	insert(q, *x_fantasma, *y_fantasma, 0);
	wavefront(mat, *x_fantasma, *y_fantasma, q);
	mat[*y_fantasma][*x_fantasma] = -1;

	while(*x_fantasma != *x_pacman || *y_fantasma != *y_pacman){
		int aux = mat[*y_pacman][*x_pacman];
		mat[*y_pacman][*x_pacman] = -2;
		print_matriz(mat, *x_fantasma, *y_fantasma);
		for (int i=0;i<MAXN;i++){
			printf("---");
		}
		printf("-\n");
		mat[*y_pacman][*x_pacman] = aux;

		percorrer(x_fantasma, y_fantasma, mat);
		if(!CheckMatrix(mat)) {
			mat[*y_fantasma][*x_fantasma] = 0;
			insert(q, *x_fantasma, *y_fantasma, 0);
			wavefront(mat, *x_fantasma, *y_fantasma, q);
			mat[*y_fantasma][*x_fantasma] = -1;
		}
		char comando;
		scanf("\n%c", &comando);

		if(comando == 's' && (*y_pacman)<(MAXN-1)){ //Baixo
			(*y_pacman) = (*y_pacman) + 1;
		} else if(comando == 'w' && (*y_pacman)>0) { //Cima
			(*y_pacman) = (*y_pacman) - 1;
		} else if(comando == 'd' && (*x_pacman)<(MAXN-1)) { //Direita
			(*x_pacman) = (*x_pacman) + 1;
		} else if(comando == 'a' && (*x_pacman)>0) { //Esquerda
			(*x_pacman) = (*x_pacman) - 1;
		}
		
	}

	print_matriz(mat, *x_fantasma, *y_fantasma);
	printf("Perdeu!\n");

	return 0;
}
