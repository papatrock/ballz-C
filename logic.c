#include "logic.h"
#define RAIO 7
#define VEL 15

void must_init(bool test, const char *description) {
  if (test) return;

  printf("couldn't initialize %s\n", description);
  exit(1);
}

int createBall(balls_t *ball, float x, float y) {
  ball->x = x;
  ball->y = y;
  ball->dx = 0;
  ball->dy = 0;
  return 1;
}

int createSquare(square_t *square) {
  square->x1 = rand() % 500;
  square->x2 = square->x1 + 320 / 7;  // tamanho do display x / 7
  square->y1 = square->x1;
  square->y2 = square->x2;
  square->lives = rand() % 100;
  if (square) return 1;
  return 0;
}

void initMatriz(square_t matriz[10][10]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 7; j++) {
      matriz[i][j].x1 = j * DISPLAY_X / 7;
      matriz[i][j].x2 = matriz[i][j].x1 + DISPLAY_X / 7 - 5;
      matriz[i][j].y1 = 180 + (i * DISPLAY_X / 7);
      matriz[i][j].y2 = matriz[i][j].y1 + DISPLAY_X / 7 - 5;
      matriz[i][j].lives = 0;
      matriz[i][j].ball = false;
    }
  }
}

void colorSelect(int lives, int *r, int *g, int *b) {
  if (lives < 6) {
    *r = 200;
    *g = 150;
    *b = 50;
  }

  if (lives > 5 && lives < 20) {
    *r = 185;
    *g = 22;
    *b = 20;
  }

  if (lives > 20 && lives < 29) {
    *r = 55;
    *g = 160;
    *b = 60;
  }

  if (lives > 29 && lives < 50) {
    *r = 155;
    *g = 88;
    *b = 203;
  }

  if (lives > 50) {
    *r = 60;
    *g = 60;
    *b = 200;
  }
}

void drawAllSquare(square_t matriz[10][10], ALLEGRO_FONT *font, int animation) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 7; j++) {
      if (matriz[i][j].ball == true) {
        if (animation)
          al_draw_circle(matriz[i][j].x1 + (DISPLAY_X / 7) / 2, matriz[i][j].y1 - 10 + (DISPLAY_Y / 8) / 2, 15, al_map_rgb(255, 255, 255),
                         4);
        else
          al_draw_circle(matriz[i][j].x1 + (DISPLAY_X / 7) / 2, matriz[i][j].y1 - 10 + (DISPLAY_Y / 8) / 2, 19, al_map_rgb(255, 255, 255),
                         4);

        al_draw_filled_circle(matriz[i][j].x1 + (DISPLAY_X / 7) / 2, matriz[i][j].y1 - 10 + (DISPLAY_Y / 8) / 2, 7,
                              al_map_rgb(250, 250, 250));
      }

      if (matriz[i][j].lives == 0) {
        continue;
      }
      int r, g, b;
      colorSelect(matriz[i][j].lives, &r, &g, &b);
      al_draw_filled_rectangle(matriz[i][j].x1, matriz[i][j].y1, matriz[i][j].x2, matriz[i][j].y2, al_map_rgb(r, g, b));
      al_draw_textf(font, al_map_rgb(0, 0, 0), matriz[i][j].x1 + (DISPLAY_X / 7) / 2 - 5, matriz[i][j].y1 + 30, ALLEGRO_ALIGN_CENTRE, "%d",
                    matriz[i][j].lives);
    }
  }
}

void drawAllBalls(fila_t *f) {
  if (fila_vazia(f)) {
    return;
  }

  nodo_f_t *tmp;
  tmp = f->ini;
  while (tmp != NULL) {
    al_draw_filled_circle(tmp->ball->x, tmp->ball->y, RAIO, al_map_rgb(250, 250, 250));
    tmp = tmp->prox;
  }
}

void drawCollectedBalls(fila_t *f) {
  if (fila_vazia(f)) {
    return;
  }

  nodo_f_t *tmp;
  tmp = f->ini;
  while (tmp != NULL) {
    al_draw_filled_circle(tmp->ball->x, tmp->ball->y, 7, al_map_rgb(50, 250, 50));
    tmp = tmp->prox;
  }
}

void drawAim(fila_t *f, ALLEGRO_MOUSE_STATE mousepos, float y, float x, ALLEGRO_FONT *font, ALLEGRO_TRANSFORM transform) {
  al_identity_transform(&transform);
  al_translate_transform(&transform, -f->ini->ball->x, -f->ini->ball->y);
  al_rotate_transform(
      &transform, -sin((f->ini->ball->x - mousepos.x) / sqrt(pow(f->ini->ball->x - mousepos.x, 2) + pow(f->ini->ball->y - mousepos.y, 2))));
  al_translate_transform(&transform, f->ini->ball->x, f->ini->ball->y);
  al_use_transform(&transform);

  al_draw_triangle(f->ini->ball->x - 5, f->ini->ball->y - 35, f->ini->ball->x + 5, f->ini->ball->y - 35, f->ini->ball->x,
                   f->ini->ball->y - 60, al_map_rgb(250, 250, 250), 4);
  al_draw_filled_circle(x, y - 40, 5, al_map_rgb(255, 255, 255));

  al_identity_transform(&transform);
  al_use_transform(&transform);

  al_draw_textf(font, al_map_rgb(250, 250, 250), f->ini->ball->x, f->ini->ball->y - 35, ALLEGRO_ALIGN_CENTER, "%d", f->tamanho);
}

void drawHud(int score, ALLEGRO_FONT *font, int highscore) {
  al_draw_filled_rectangle(0, 0, DISPLAY_X, DISPLAY_Y / 7, al_map_rgb(20, 20, 20));
  al_draw_filled_rectangle(0, DISPLAY_Y - DISPLAY_Y / 20, DISPLAY_X, DISPLAY_Y, al_map_rgb(20, 20, 20));
  al_draw_line(25, 30, 25, 70, al_map_rgb(40, 40, 40), 7);
  al_draw_line(50, 30, 50, 70, al_map_rgb(40, 40, 40), 7);
  al_draw_textf(font, al_map_rgb(250, 250, 250), DISPLAY_X - 90, 20, 0, "Best");
  al_draw_textf(font, al_map_rgb(250, 250, 250), DISPLAY_X - 70, 45, ALLEGRO_ALIGN_CENTER, "%d", highscore);
  al_draw_textf(font, al_map_rgb(250, 250, 250), DISPLAY_X / 2, 20, ALLEGRO_ALIGN_CENTRE, "%d", score);
}

int spawnSquare(square_t matriz[10][10], int score) {
  int bola = rand() % 5 + 1;
  if (score == 0) {
    for (int j = 0; j < 9; j++) {
      if (rand() % 3) matriz[1][j].lives = (rand() % 1) + 1;
    }
    return 1;
  }

  for (int j = 0; j < 8; j++) {
    if (!rand() % 2) matriz[0][j].ball = true;
    if (rand() % 3 && matriz[0][j].ball == false) matriz[0][j].lives = rand() % (score * 2);
  }

  for (int i = 8; i >= 1; i--) {
    for (int j = 0; j < 7; j++) {
      matriz[i][j].lives = matriz[i - 1][j].lives;
      matriz[i][j].ball = matriz[i - 1][j].ball;
      for (int k = 0; k < 7; k++) {
        if (matriz[7][k].lives != 0) {
          return 0;
        }
      }
    }
  }
  matriz[1][bola].ball = true;
  matriz[1][bola].lives = 0;

  for (int j = 0; j < 8; j++) {
    matriz[0][j].lives = 0;
  }

  return 1;
}

int ballMove(fila_t *f, fila_t *ballsCollected, fila_t *aux, int *score, square_t matrizSquare[10][10]) {
  if (fila_vazia(f)) return 0;
  nodo_f_t *tmp;
  tmp = f->ini;

  while (tmp != NULL) {
    tmp->ball->x += tmp->ball->dx;
    tmp->ball->y += tmp->ball->dy;
    if (tmp->ball->x < 0) {
      tmp->ball->x *= -1;
      tmp->ball->dx *= -1;
    }
    // parede direta
    if (tmp->ball->x > DISPLAY_X) {
      tmp->ball->x -= (tmp->ball->x - DISPLAY_X) * 2;
      tmp->ball->dx *= -1;
    }
    // teto
    if (tmp->ball->dy < 0 && tmp->ball->y - fmax(VEL, RAIO) < DISPLAY_Y / 8) {
      tmp->ball->dy *= -1;
    }
    // bola caiu no chão
    if (tmp->ball->y >= DISPLAY_Y - DISPLAY_Y / 20 && (tmp->ball->dx != 0 && tmp->ball->dy != 0)) {
      tmp->ball->dx = tmp->ball->dy = 0;
      tmp->ball->y = DISPLAY_Y - DISPLAY_Y / 20 - RAIO;
      BallQueue(aux, tmp->ball);
    }
    tmp = tmp->prox;
  }
  // todas as bolas cairam
  if (aux->tamanho == f->tamanho) {
    // limpa f
    while (f->ini != NULL) dequeue(f);

    // coloca todas as bolas na fila (em ordem de que caiu)
    for (nodo_f_t *tmp2 = aux->ini; tmp2 != NULL; tmp2 = tmp2->prox) {
      BallQueue(f, tmp2->ball);
    }
    while (aux->ini != NULL) {
      dequeue(aux);
    }
    // coloca novas bolas
    for (nodo_f_t *tmp2 = ballsCollected->ini; tmp2 != NULL; tmp2 = tmp2->prox) {
      BallQueue(f, tmp2->ball);
    }
    while (ballsCollected->ini != NULL) {
      dequeue(ballsCollected);
    }

    // coloca a posição de todas as bolas na primeira que caiu
    for (nodo_f_t *tmp2 = f->ini; tmp2 != NULL; tmp2 = tmp2->prox) {
      tmp2->ball->x = f->ini->ball->x;
    }

    *score = *score + 1;
    if (!spawnSquare(matrizSquare, *score)) {
      return 0;
    }
  }

  return 1;
}

void collectedBallMove(fila_t *f) {
  nodo_f_t *tmp;
  tmp = f->ini;
  while (tmp != NULL) {
    tmp->ball->y += tmp->ball->dy;
    if (tmp->ball->y >= DISPLAY_Y - DISPLAY_Y / 20) {
      tmp->ball->dx = tmp->ball->dy = 0;
      tmp->ball->y = DISPLAY_Y - DISPLAY_Y / 20 - RAIO;
    }
    tmp = tmp->prox;
  }
}

int collide(fila_t *fila, fila_t *collectedBallsQueue, square_t matriz[10][10], ALLEGRO_SAMPLE *colideSample,
            ALLEGRO_SAMPLE_INSTANCE *collecteSample, int *score) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 7; j++) {
      if (matriz[i][j].lives > 0 || matriz[i][j].ball == true) {
        for (nodo_f_t *tmp = fila->ini; tmp != NULL; tmp = tmp->prox) {
          int bateu = 0;

          if ((tmp->ball->dy > 0 && tmp->ball->y + fmax(VEL, RAIO) > matriz[i][j].y1 && tmp->ball->y < matriz[i][j].y1) &&
              (tmp->ball->x + fmax(VEL, RAIO) > matriz[i][j].x1 && tmp->ball->x - fmax(VEL, RAIO) < matriz[i][j].x2)) {
            if (matriz[i][j].ball == true) {
              balls_t *ball = malloc(sizeof(balls_t));
              if (!ball) return 0;
              createBall(ball, (matriz[i][j].x1 + matriz[i][j].x2) / 2, (matriz[i][j].y1 + matriz[i][j].y2) / 2);
              ball->dy = 15;
              BallQueue(collectedBallsQueue, ball);
              matriz[i][j].ball = false;
              al_play_sample_instance(collecteSample);
              continue;
            }
            al_play_sample(colideSample, 0.3, 0, 0.75, ALLEGRO_PLAYMODE_ONCE, NULL);
            tmp->ball->dy *= -1;
            bateu = 1;
          }
          // colisao em baixo
          if ((tmp->ball->dy < 0) && (tmp->ball->y - fmax(VEL, RAIO) < matriz[i][j].y2) && (tmp->ball->y > matriz[i][j].y2) &&
              (tmp->ball->x + fmax(VEL, RAIO) > matriz[i][j].x1) && (tmp->ball->x - fmax(VEL, RAIO) < matriz[i][j].x2)) {
            if (matriz[i][j].ball == true) {
              balls_t *ball = malloc(sizeof(balls_t));
              if (!ball) return 0;
              createBall(ball, (matriz[i][j].x1 + matriz[i][j].x2) / 2, (matriz[i][j].y1 + matriz[i][j].y2) / 2);
              ball->dy = 15;
              BallQueue(collectedBallsQueue, ball);
              matriz[i][j].ball = false;
              al_play_sample_instance(collecteSample);
              continue;
            }
            al_play_sample(colideSample, 0.3, 0, 0.75, ALLEGRO_PLAYMODE_ONCE, NULL);
            tmp->ball->dy *= -1;
            bateu = 1;
          }
          // colisao do lado direito
          if ((tmp->ball->dx < 0 && tmp->ball->x - fmax(VEL, RAIO) < matriz[i][j].x2 && tmp->ball->x > matriz[i][j].x2) &&
              (tmp->ball->y - fmax(VEL, RAIO) < matriz[i][j].y2 && tmp->ball->y + fmax(VEL, RAIO) > matriz[i][j].y1)) {
            if (matriz[i][j].ball == true) {
              balls_t *ball = malloc(sizeof(balls_t));
              if (!ball) return 0;
              createBall(ball, (matriz[i][j].x1 + matriz[i][j].x2) / 2, (matriz[i][j].y1 + matriz[i][j].y2) / 2);
              ball->dy = 15;
              BallQueue(collectedBallsQueue, ball);
              matriz[i][j].ball = false;
              al_play_sample_instance(collecteSample);
              continue;
            }
            al_play_sample(colideSample, 0.3, 0, 0.75, ALLEGRO_PLAYMODE_ONCE, NULL);
            tmp->ball->dx *= -1;
            bateu = 1;
          }
          // colisao do lado esquerdo
          if ((tmp->ball->dx > 0 && tmp->ball->y + fmax(VEL, RAIO) > matriz[i][j].y1 && tmp->ball->y - fmax(VEL, RAIO) < matriz[i][j].y2) &&
              (tmp->ball->x < matriz[i][j].x1 && tmp->ball->x + fmax(VEL, RAIO) > matriz[i][j].x1)) {
            if (matriz[i][j].ball == true) {
              balls_t *ball = malloc(sizeof(balls_t));
              createBall(ball, (matriz[i][j].x1 + matriz[i][j].x2) / 2, (matriz[i][j].y1 + matriz[i][j].y2) / 2);
              ball->dy = 15;
              BallQueue(collectedBallsQueue, ball);
              matriz[i][j].ball = false;
              al_play_sample_instance(collecteSample);
              continue;
            }
            al_play_sample(colideSample, 0.3, 0, 0.75, ALLEGRO_PLAYMODE_ONCE, NULL);
            tmp->ball->dx *= -1;
            bateu = 1;
          }

          if (bateu) matriz[i][j].lives = matriz[i][j].lives - 1;

          if (matriz[i][j].lives < 0) matriz[i][j].lives = 0;
        }
      }
    }
  }
  return 1;
}

int readHightScore() {
  char *token;
  char line[1024];
  FILE *file = fopen("highscore.txt", "r");
  must_init(file, "file");
  fgets(line, 1024, file);

  token = strtok(line, ":");
  while (token) {
    if (strcmp(token, "Best score") == 0) {
      token = strtok(NULL, ":");
      return atoi(token);
    }
    token = strtok(NULL, ":");
  }

  fclose(file);

  return 0;
}

void writeHightScore(int score) {
  FILE *file = fopen("highscore.txt", "w");
  must_init(file, "file");
  fprintf(file, "Best score:%d", score);
  fclose(file);
}
/*------FILA------*/

fila_t *fila_cria() {
  fila_t *fila;
  if (!(fila = malloc(sizeof(fila_t)))) return NULL;

  fila->ini = fila->fim = NULL;
  fila->tamanho = 0;
  return fila;
}

fila_t *fila_destroi(fila_t *f) {
  nodo_f_t *tmp;
  while (f->ini != NULL) {
    tmp = f->ini;
    f->ini = f->ini->prox;
    free(tmp->ball);
    free(tmp);
  }

  free(f);
  return NULL;
}

int fila_vazia(fila_t *f) {
  if (f->ini != NULL) return 0;
  return 1;
}

int fila_tamanho(fila_t *f) { return f->tamanho; }

int BallQueue(fila_t *f, balls_t *elemento) {
  nodo_f_t *novo;
  novo = criaItem();
  if (!novo) return 0;

  novo->ball = elemento;
  novo->prox = NULL;

  if (fila_vazia(f)) {
    f->ini = novo;
    f->fim = novo;
  } else {
    f->fim->prox = novo;
    f->fim = novo;
  }
  f->tamanho++;

  return 1;
}

int dequeue(fila_t *f) {
  nodo_f_t *tmp;
  if (fila_vazia(f)) return 0;

  tmp = f->ini;

  if (f->ini == f->fim) {
    f->ini = NULL;
    f->fim = NULL;
  } else
    f->ini = f->ini->prox;

  f->tamanho--;
  free(tmp);
  return 1;
}

nodo_f_t *criaItem() {
  nodo_f_t *tmp;
  if (!(tmp = malloc(sizeof(nodo_f_t)))) return NULL;

  return tmp;
}

int moveEasterEgg(float *x, float *y, square_t matriz[10][10]) {
  if (*x < -650) return 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 7; j++) {
      if (matriz[i][j].x1 > *x && (matriz[i][j].y1 > *y && matriz[i][j].y2 < *y + 300)) {
        matriz[i][j].lives = 99;
      }
    }
  }

  *x -= 1;
  return 1;
}

void drawEasterEgg(float x, float y, ALLEGRO_BITMAP *dog1, ALLEGRO_BITMAP *dog2, int animation) {
  if (animation)
    al_draw_bitmap(dog1, x, y, 0);
  else
    al_draw_bitmap(dog2, x, y, 0);
}