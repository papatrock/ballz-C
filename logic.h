
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEY_SEEN 1
#define KEY_RELEASED 2
#define SQUARE_N 49
#define DISPLAY_X 640
#define DISPLAY_Y 920

struct balls {
  float x;
  float y;
  float dx;
  float dy;
};
typedef struct balls balls_t;

struct square {
  float x1;
  float y1;
  float x2;
  float y2;
  bool ball;
  int lives;
};
typedef struct square square_t;

struct matrizSquare {
  square_t *square;
};
typedef struct matrizSquare matrizSquare_t;

struct nodo_f {
  balls_t *ball;
  struct nodo_f *prox; /* ponteiro para o proximo  */
};
typedef struct nodo_f nodo_f_t;

struct fila {
  nodo_f_t *ini; /* ponteiro para o inicio da lista */
  nodo_f_t *fim; /* ponteiro para o fim da lista    */
  int tamanho;   /* numero de elementos na lista    */
};
typedef struct fila fila_t;

void must_init(bool test, const char *description);

void troca(nodo_f_t *a, nodo_f_t *b);

int createBall(balls_t *ball, float x, float y);

int createSquare(square_t *square);

void initMatriz(square_t matriz[10][10]);

void storeSquare(square_t *vetor[], square_t *square, int i);

void colorSelect(int lives, int *r, int *g, int *b);

void drawAllSquare(square_t matriz[10][10], ALLEGRO_FONT *font, int animation);

void drawAim(fila_t *f, ALLEGRO_MOUSE_STATE mousepos, float y, float x, ALLEGRO_FONT *font, ALLEGRO_TRANSFORM transform);

void drawAllBalls(fila_t *f);

void drawCollectedBalls(fila_t *f);

void drawHud(int score, ALLEGRO_FONT *font, int highscore);

int spawnSquare(square_t matriz[10][10], int score);

int ballMove(fila_t *f, fila_t *ballsCollected, fila_t *aux, int *score, square_t matrizSquare[10][10]);

void collectedBallMove(fila_t *f);

int collide(fila_t *fila, fila_t *coletedBallsQueue, square_t matriz[10][10], ALLEGRO_SAMPLE *colideSample,
            ALLEGRO_SAMPLE_INSTANCE *collecteSample, int *score);

int readHightScore();

void writeHightScore(int score);

/*-----------------FILA------------------------*/

/*
 * Cria uma fila vazia e a retorna, se falhar retorna NULL.
 */
fila_t *fila_cria();

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 */
fila_t *fila_destroi(fila_t *f);

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int fila_vazia(fila_t *f);

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int fila_tamanho(fila_t *f);

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int BallQueue(fila_t *f, balls_t *elemento);

/*
 * Remove elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int dequeue(fila_t *f);

nodo_f_t *criaItem();

int moveEasterEgg(float *x, float *y, square_t matriz[10][10]);

void drawEasterEgg(float x, float y, ALLEGRO_BITMAP *dog1, ALLEGRO_BITMAP *dog2, int animation);
