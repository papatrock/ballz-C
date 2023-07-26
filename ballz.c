#include "logic.h"
#define VEL 15
#define RAIO 7

int main() {
  int raio = 7, score = 0, animation = 0;
  float x, y, dogX, dogY;
  char dog[10];

  x = DISPLAY_X / 2;
  y = DISPLAY_Y - DISPLAY_Y / 20 - raio;

  float mx, my;
  mx = 0;
  my = 0;

  long double lancamentoTemp = 0, flick = 0;

  bool redraw = true, lancamento = false, game = true, gameplay = false;
  bool endGame = false, menu = true, pause = false, help = false, easterEgg = false;

  square_t matrizSquare[10][10];
  fila_t* ballsQueue = fila_cria();
  fila_t* collectedBallsQueue = fila_cria();
  fila_t* auxQueue = fila_cria();

  nodo_f_t* aux = NULL;

  /*------------------------INICIA COISAS--------------------------------------------*/
  srand(time(NULL));

  must_init(al_init(), "allegro");
  must_init(al_install_keyboard(), "keyboard");
  must_init(al_install_mouse(), "mouse");
  must_init(al_get_mouse_num_buttons(), "mouse button");
  al_init_font_addon();
  al_is_ttf_addon_initialized();
  must_init(al_init_ttf_addon(), "allegro ttf");

  // 60 fps
  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_X, DISPLAY_Y);
  must_init(disp, "display");

  /*--------FONTES------------------*/
  ALLEGRO_FONT* squareFont = al_create_builtin_font();
  must_init(squareFont, "squareFont");
  squareFont = al_load_font("roboto/Roboto-Medium.ttf", 20, 0);

  ALLEGRO_FONT* scoreFont = al_create_builtin_font();
  must_init(scoreFont, "scoreFont");
  scoreFont = al_load_font("roboto/Roboto-Medium.ttf", 25, 0);

  ALLEGRO_FONT* menuFont = al_create_builtin_font();
  must_init(menuFont, "menuFont");
  menuFont = al_load_font("roboto/Roboto-Medium.ttf", 35, 0);

  ALLEGRO_FONT* endScoreFont = al_create_builtin_font();
  must_init(endScoreFont, "endScoreFont");
  endScoreFont = al_load_font("roboto/Roboto-Medium.ttf", 90, 0);
  /*---------------------------------*/

  /*--------SOM------------------*/
  must_init(al_install_audio(), "audio");
  must_init(al_init_acodec_addon(), "audio codecs");
  must_init(al_reserve_samples(128), "reserve samples");

  ALLEGRO_SAMPLE* colideSample = al_load_sample("sounds/hitsound.wav");
  ;
  must_init(colideSample, "colide Sample");

  ALLEGRO_SAMPLE* dogMusic = al_load_sample("sounds/dogSound.wav");
  ALLEGRO_SAMPLE_INSTANCE* dogMusicInstance = al_create_sample_instance(dogMusic);
  al_attach_sample_instance_to_mixer(dogMusicInstance, al_get_default_mixer());
  must_init(dogMusicInstance, "dogMusic");

  ALLEGRO_SAMPLE* collectSample = al_load_sample("sounds/collectSample.wav");
  ALLEGRO_SAMPLE_INSTANCE* collectSampleInstance = al_create_sample_instance(collectSample);
  al_attach_sample_instance_to_mixer(collectSampleInstance, al_get_default_mixer());

  /*---------------------------------*/

  /*--------ROTAÇÃO DE PRIMITIVOS------------------*/
  ALLEGRO_TRANSFORM transform;

  /*--------ARQUIVO------------------*/
  int highscore = readHightScore();

  /*--------IMAGENS------------------*/
  must_init(al_init_image_addon(), "allegro image");
  ALLEGRO_BITMAP* ballzIcon = al_load_bitmap("sprites/ballz-icon.png");
  must_init(ballzIcon, "ballzIcon");
  ALLEGRO_BITMAP* playIcon = al_load_bitmap("sprites/play.png");
  must_init(playIcon, "playIcon");
  ALLEGRO_BITMAP* playCover = al_load_bitmap("sprites/play-cover.png");
  must_init(playCover, "playCover");
  ALLEGRO_BITMAP* exitIcon = al_load_bitmap("sprites/exit.png");
  must_init(exitIcon, "exitIcon");
  ALLEGRO_BITMAP* yellowIcon = al_load_bitmap("sprites/yellow-icon.png");
  must_init(yellowIcon, "yellowIcon");
  ALLEGRO_BITMAP* helpPhoto = al_load_bitmap("sprites/help.png");
  must_init(helpPhoto, "helpPhoto");
  ALLEGRO_BITMAP* setaIcon = al_load_bitmap("sprites/seta.png");
  must_init(setaIcon, "setaIcon");
  ALLEGRO_BITMAP* dog1 = al_load_bitmap("sprites/anoyng-dog-1.png");
  must_init(dog1, "dog1");
  ALLEGRO_BITMAP* dog2 = al_load_bitmap("sprites/anoyng-dog-2.png");
  must_init(dog1, "dog2");

  /*---------------------------------*/

  must_init(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_mouse_event_source());

  ALLEGRO_EVENT event;

  ALLEGRO_MOUSE_STATE mousepos;

  // al_grab_mouse(disp);

  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key));

  /*----------------------------------------------------------------------------*/
  float dir;

  balls_t* ball = malloc(sizeof(balls_t));
  createBall(ball, x, y);
  BallQueue(ballsQueue, ball);

  initMatriz(matrizSquare);

  spawnSquare(matrizSquare, score);

  float aimVet[16];
  for (int i = 0; i < 16; i++) {
    aimVet[i] = DISPLAY_Y - DISPLAY_Y / 20;
  }

  al_start_timer(timer);

  flick = al_get_timer_count(timer);

  while (game) {
    /*------------------------MENU PRINCIPAL--------------------------------------------*/
    while (menu) {
      al_wait_for_event(queue, &event);
      al_get_mouse_state(&mousepos);

      switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          menu = false;
          game = false;
          break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
          // botao inciar
          if ((mousepos.x >= 185 && mousepos.x <= 440) && (mousepos.y >= 438 && mousepos.y <= 530)) {
            gameplay = true;
            menu = false;
          }
          // botao sair
          if ((mx >= 179 && mx <= 445) && (my >= 565 && my <= 665)) {
            menu = false;
            game = false;
          }
          // botao de ajuda
          if ((mx >= 580 && mx <= 630) && (my >= 860 && my <= 900)) {
            help = true;
          }
          break;
        case ALLEGRO_EVENT_MOUSE_AXES:
          mx = event.mouse.x;
          my = event.mouse.y;
          // botao inciar
          if ((mx >= 185 && mx <= 440) && (my >= 438 && my <= 530))
            al_draw_scaled_bitmap(playCover, 0, 0, al_get_bitmap_width(playIcon), al_get_bitmap_height(playIcon), DISPLAY_X / 2 - 150,
                                  DISPLAY_Y / 2 - 50, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
          // botao sair

          break;
      }
      if (redraw && al_is_event_queue_empty(queue)) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(ballzIcon, 0, 0, al_get_bitmap_width(ballzIcon), al_get_bitmap_height(ballzIcon), DISPLAY_X / 2 - 150, 60,
                              300, 150, 0);
        al_draw_scaled_bitmap(playIcon, 0, 0, al_get_bitmap_width(playIcon), al_get_bitmap_height(playIcon), DISPLAY_X / 2 - 150,
                              DISPLAY_Y / 2 - 50, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
        al_draw_scaled_bitmap(exitIcon, 0, 0, al_get_bitmap_width(exitIcon), al_get_bitmap_height(exitIcon), DISPLAY_X / 2 - 150,
                              DISPLAY_Y / 2 + 80, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
        al_draw_filled_rectangle(DISPLAY_X - 60, DISPLAY_Y - 60, DISPLAY_X - 10, DISPLAY_Y - 20, al_map_rgb(70, 70, 250));
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2, ALLEGRO_ALIGN_CENTRE, "PLAY");
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2 + 130, ALLEGRO_ALIGN_CENTRE, "EXIT");
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X - 35, DISPLAY_Y - 60, ALLEGRO_ALIGN_CENTRE, "?");
        al_flip_display();
      }

      /*------------------------MENU DE AJUDA--------------------------------------------*/
      while (help) {
        al_wait_for_event(queue, &event);
        al_get_mouse_state(&mousepos);
        mx = event.mouse.x;
        my = event.mouse.y;

        switch (event.type) {
          case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if ((mx >= 13 && mx <= 131) && (my >= 0 && my <= 56)) help = false;
            break;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
          al_clear_to_color(al_map_rgb(0, 0, 0));
          al_draw_scaled_bitmap(setaIcon, 0, 0, al_get_bitmap_width(setaIcon), al_get_bitmap_height(setaIcon), 0, 0,
                                al_get_bitmap_width(setaIcon) / 2.5, al_get_bitmap_height(setaIcon) / 2.5, 0);
          al_draw_scaled_bitmap(helpPhoto, 0, 0, al_get_bitmap_width(helpPhoto), al_get_bitmap_height(helpPhoto), -5, 180,
                                al_get_bitmap_width(helpPhoto) / 1.1, al_get_bitmap_width(helpPhoto) / 1.1, 0);
          al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, 50, ALLEGRO_ALIGN_CENTRE, "COMO JOGAR");
          al_flip_display();
        }
      }
    }

    /*------------------------JOGO--------------------------------------------*/
    while (gameplay) {
      al_wait_for_event(queue, &event);
      al_get_mouse_state(&mousepos);

      switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
          mx = mousepos.x;
          my = mousepos.y;

          /*------fim do jogo------*/
          if (!ballMove(ballsQueue, collectedBallsQueue, auxQueue, &score, matrizSquare)) {
            if (score > highscore) {
              highscore = score;
              writeHightScore(highscore);
            }
            endGame = true;
            gameplay = false;
          }
          collectedBallMove(collectedBallsQueue);

          for (int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= KEY_SEEN;
          redraw = true;

          /*
            lancamento
          */
          if (lancamento) {
            if (aux == NULL) {
              lancamento = false;
              break;
            }

            if (aux->ball->dx == 0 && aux->ball->dy == 0) {
              aux->ball->dx = cos(dir) * VEL;
              aux->ball->dy = sin(dir) * VEL;
            }
            if (al_get_timer_count(timer) > lancamentoTemp + 10) {
              aux = aux->prox;
              lancamentoTemp = al_get_timer_count(timer);
            }
          }
          break;

        case ALLEGRO_EVENT_KEY_DOWN:
          switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              pause = true;
              break;

            case ALLEGRO_KEY_D:
              dog[0] = 'd';

            case ALLEGRO_KEY_O:
              if (dog[0] == 'd')
                dog[1] = 'o';
              else
                dog[0] = '\0';
              break;

            case ALLEGRO_KEY_G:
              if (dog[0] == 'd' && dog[1] == 'o')
                dog[2] = 'g';
              else {
                dog[0] = '\0';
                dog[1] = '\0';
              }
          }
          break;

        case ALLEGRO_EVENT_MOUSE_AXES:
          mx += mousepos.x * 0.2;
          my += mousepos.y * 0.2;
          break;

        /*
          Faz a bola ser lançada em direção ao mouse
        */
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
          /*botao de pause*/
          if ((mx >= 20 && mx <= 44) && (my >= 28 && my <= 72)) {
            pause = true;
            break;
          }

          if (auxQueue->tamanho != 0 || collectedBallsQueue->tamanho != 0) {
            break;
          }
          dir = atan2((event.mouse.y - ball->y), (event.mouse.x - ball->x));
          lancamento = true;
          lancamentoTemp = al_get_timer_count(timer);
          aux = ballsQueue->ini;

          // al_set_mouse_xy(disp, DISPLAY_X, DISPLAY_Y);
          break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          gameplay = false;
          game = false;
          break;
      }
      /*
          COLISÃO
      */
      if (!collide(ballsQueue, collectedBallsQueue, matrizSquare, colideSample, collectSampleInstance, &score)) {
        printf("erro ao alocar memoria\n");
        gameplay = false;
        game = false;
      }

      if (al_get_timer_count(timer) > flick + 25) {
        if (animation == 0) {
          animation = 1;
        } else {
          animation = 0;
        }

        flick = al_get_timer_count(timer);
      }

      /*easter egg*/
      // MUDAR PARA SE DIGITOU E O ULTIMO EASTER EGG JÁ ACABOU
      if (strcmp(dog, "dog") == 0 && !easterEgg) {
        easterEgg = true;
        /*seta a posição inicial do easter egg*/
        dogX = DISPLAY_X + 100;
        dogY = DISPLAY_Y / 2 - 200;
        /*inicia a musica*/
        al_play_sample_instance(dogMusicInstance);
        /*reseta o vetor dog*/
        dog[0] = '0';
        dog[1] = '0';
        dog[2] = '0';
      }

      if (easterEgg)
        if (!moveEasterEgg(&dogX, &dogY, matrizSquare)) {
          easterEgg = false;
          dogX = DISPLAY_X + 100;
          dogY = DISPLAY_Y / 2 - 200;
        }

      /*
        Desenha as coisas
      */
      if (redraw && al_is_event_queue_empty(queue)) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        drawAllBalls(ballsQueue);
        drawAllSquare(matrizSquare, squareFont, animation);
        drawAllBalls(collectedBallsQueue);
        drawCollectedBalls(collectedBallsQueue);
        drawAllBalls(auxQueue);
        drawHud(score, scoreFont, highscore);
        if (easterEgg) {
          drawEasterEgg(dogX, dogY, dog1, dog2, animation);
        }

        if (auxQueue->tamanho == 0 && collectedBallsQueue->tamanho == 0 && ballsQueue->ini->ball->y >= 470 &&
            ballsQueue->ini->ball->dx == 0 && ballsQueue->ini->ball->dy == 0) {
          aimVet[0] = ballsQueue->ini->ball->y;
          for (int i = 1; i < 16; i++) {
            aimVet[i] = aimVet[i - 1] - 30 + (i / 5);
            if (aimVet[i] > DISPLAY_Y - DISPLAY_Y / 20) aimVet[i] = DISPLAY_Y - DISPLAY_Y / 20;
            if (aimVet[i] < mousepos.y) aimVet[i] = mousepos.y;
            drawAim(ballsQueue, mousepos, aimVet[i], ballsQueue->ini->ball->x, squareFont, transform);
          }
        }
        al_flip_display();

        redraw = false;
      }

      /*------------------------MENU DE PAUSE--------------------------------------------*/
      while (pause) {
        al_wait_for_event(queue, &event);
        al_get_mouse_state(&mousepos);

        switch (event.type) {
          case ALLEGRO_EVENT_TIMER:

            mx = mousepos.x;
            my = mousepos.y;
            redraw = true;
            break;

          case ALLEGRO_EVENT_MOUSE_AXES:
            mx += mousepos.x * 0.2;
            my += mousepos.y * 0.2;
            break;

          case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

            /*continue*/
            if ((mx >= 190 && mx <= 440) && (my >= 318 && my <= 405)) {
              pause = false;
            }

            /*restar*/
            if ((mx >= 190 && mx <= 440) && (my >= 440 && my <= 530)) {
              score = 0;
              while (collectedBallsQueue->ini != NULL) dequeue(collectedBallsQueue);

              while (ballsQueue->ini != NULL) dequeue(ballsQueue);

              while (auxQueue->ini != NULL) dequeue(auxQueue);

              createBall(ball, x, y);
              BallQueue(ballsQueue, ball);
              initMatriz(matrizSquare);
              spawnSquare(matrizSquare, score);
              pause = false;
              gameplay = true;
            }
            /*main menu*/
            if ((mx >= 178 && mx <= 615) && (my >= 565 && my <= 663)) {
              score = 0;
              gameplay = false;
              pause = false;
              menu = true;

              while (collectedBallsQueue->ini != NULL) dequeue(collectedBallsQueue);

              while (ballsQueue->ini != NULL) dequeue(ballsQueue);

              while (auxQueue->ini != NULL) dequeue(auxQueue);

              createBall(ball, x, y);
              BallQueue(ballsQueue, ball);
              initMatriz(matrizSquare);
              spawnSquare(matrizSquare, score);
            }
            break;

          case ALLEGRO_EVENT_DISPLAY_CLOSE:
            pause = false;
            gameplay = false;
            game = false;
            menu = false;
            break;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
          al_clear_to_color(al_map_rgb(0, 0, 0));
          al_draw_scaled_bitmap(playIcon, 0, 0, al_get_bitmap_width(playIcon), al_get_bitmap_height(playIcon), DISPLAY_X / 2 - 150, 287,
                                al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
          al_draw_scaled_bitmap(yellowIcon, 0, 0, al_get_bitmap_width(yellowIcon), al_get_bitmap_height(yellowIcon), DISPLAY_X / 2 - 150,
                                DISPLAY_Y / 2 - 50, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
          al_draw_scaled_bitmap(exitIcon, 0, 0, al_get_bitmap_width(exitIcon), al_get_bitmap_height(exitIcon), DISPLAY_X / 2 - 150,
                                DISPLAY_Y / 2 + 80, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
          al_draw_textf(endScoreFont, al_map_rgb(255, 255, 255), 190, 175, 0, "PAUSE");
          al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, 340, ALLEGRO_ALIGN_CENTRE, "CONTINUE");
          al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2, ALLEGRO_ALIGN_CENTRE, "RESTART");
          al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2 + 130, ALLEGRO_ALIGN_CENTRE, "MAIN MENU");
          al_flip_display();
        }
      }
      /*------------------------FIM DO MENU DE PAUSE--------------------------------------------*/
    }

    /*------------------------FIM DO JOGO--------------------------------------------*/
    while (endGame) {
      al_wait_for_event(queue, &event);
      al_get_mouse_state(&mousepos);
      switch (event.type) {
        case ALLEGRO_EVENT_TIMER:

          mx = mousepos.x;
          my = mousepos.y;
          redraw = true;
          break;

        case ALLEGRO_EVENT_MOUSE_AXES:
          mx += mousepos.x * 0.2;
          my += mousepos.y * 0.2;
          break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
          /*replay*/
          if ((mx >= 190 && mx <= 440) && (my >= 440 && my <= 530)) {
            score = 0;
            while (collectedBallsQueue->ini != NULL) dequeue(collectedBallsQueue);

            while (ballsQueue->ini != NULL) dequeue(ballsQueue);

            while (auxQueue->ini != NULL) dequeue(auxQueue);

            createBall(ball, x, y);
            BallQueue(ballsQueue, ball);
            initMatriz(matrizSquare);
            spawnSquare(matrizSquare, score);
            endGame = false;
            gameplay = true;
          }
          /*main menu*/
          if ((mx >= 178 && mx <= 615) && (my >= 565 && my <= 663)) {
            score = 0;
            while (collectedBallsQueue->ini != NULL) dequeue(collectedBallsQueue);

            while (ballsQueue->ini != NULL) dequeue(ballsQueue);

            while (auxQueue->ini != NULL) dequeue(auxQueue);

            createBall(ball, x, y);
            BallQueue(ballsQueue, ball);
            initMatriz(matrizSquare);
            spawnSquare(matrizSquare, score);

            endGame = false;
            menu = true;
          }
          break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          gameplay = false;
          game = false;
          menu = false;
          break;
      }
      if (redraw && al_is_event_queue_empty(queue)) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(playIcon, 0, 0, al_get_bitmap_width(playIcon), al_get_bitmap_height(playIcon), DISPLAY_X / 2 - 150,
                              DISPLAY_Y / 2 - 50, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
        al_draw_scaled_bitmap(exitIcon, 0, 0, al_get_bitmap_width(exitIcon), al_get_bitmap_height(exitIcon), DISPLAY_X / 2 - 150,
                              DISPLAY_Y / 2 + 80, al_get_bitmap_width(playIcon) + 100, al_get_bitmap_height(playIcon) + 80, 0);
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2, ALLEGRO_ALIGN_CENTRE, "REPLAY");
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), DISPLAY_X / 2 - 10, DISPLAY_Y / 2 + 130, ALLEGRO_ALIGN_CENTRE, "MAIN MENU ");
        al_draw_textf(endScoreFont, al_map_rgb(255, 255, 255), 275, 175, 0, "%d", score);
        al_draw_textf(menuFont, al_map_rgb(255, 255, 255), 245, 330, 0, "BEST %d", highscore);
        al_flip_display();
      }
    }
  }
  fila_destroi(ballsQueue);
  fila_destroi(collectedBallsQueue);
  fila_destroi(auxQueue);
  al_destroy_bitmap(ballzIcon);
  al_destroy_font(squareFont);
  al_destroy_font(scoreFont);
  al_destroy_sample(colideSample);
  al_destroy_sample(dogMusic);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}
