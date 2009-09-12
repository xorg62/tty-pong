#include "ttypong.h"

void
init_curses(void)
{
     int bg;

     initscr();
     noecho();
     nodelay(stdscr, TRUE);
     keypad(stdscr, TRUE);
     curs_set(FALSE);

     /* On check la taille du terminal */
     if(COLS < FW || LINES < FH)
     {
          endwin();
          fprintf(stderr, "Terminal too small (request %dx%d).\n", FH, FW);
          free(pong);
          exit(EXIT_FAILURE);
     }

     start_color();
     bg = (use_default_colors() == OK) ? -1 : COLOR_BLACK;
     init_pair(0, bg, bg);
     init_pair(1, COLOR_WHITE, COLOR_WHITE);
     init_pair(2, COLOR_GREEN, bg);

     return;
}

/* Initialisation de la frame */
void
init_frame(void)
{
     int i;

     COL(1);

     for(i = GX; i < FW + GX; ++i)
     {
          mvaddch(0, i, '-');
          mvaddch(FH, i, '-');
     }

     UNCOL(1);

     COL(2);

     for(i = 1; i < FH; ++i)
          mvaddch(i, (FW / 2) + GX, '|');

     UNCOL(2);

     racket_move((FH / 2) - RACKETL / 2 - 1);

     refresh();

     return;
}

void
key_event(void)
{
     int c;

     switch((c = getch()))
     {
          case KEY_UP:
               racket_move(pong->racket.y - 1);
               break;

          case KEY_DOWN:
               racket_move(pong->racket.y + 1);
               break;

               /* Touche q ou Q pour quitter */
          case 'q':
          case 'Q':
               pong->running = 0;
               break;

          /* Pause avec p ou P .. */
          case 'p':
          case 'P':
               /* On attends tant que getch() soit pas egale à p ou P */
               while(getch() != c);
               break;
     }

     return;
}

/* C'est ici que l'on fait rebondir la balle */
void
manage_ball(void)
{
     if(pong->ball.x < 1)
          pong->ball.a = 1;
     if(pong->ball.x > FW - 1)
          pong->ball.a = -1;
     if(pong->ball.y < 2)
          pong->ball.b = 1;
     if(pong->ball.y > FH - 2)
          pong->ball.b = -1;

     /* On efface la bale à son ancienne position */
     mvaddch(pong->ball.y, pong->ball.x + GX, ' ');

     pong->ball.x += pong->ball.a;
     pong->ball.y += pong->ball.b;

     mvaddch(pong->ball.y, pong->ball.x + GX, BALL);

     refresh();

     return;
}

/* Movement de la raquette */
void
racket_move(int y)
{
     int i;

     if(y <= 0 || y > FH - RACKETL)
          return;

     pong->racket.y = y;

     for(i = 1; i < FH; ++i)
          mvaddch(i, GX, ' ');

     COL(1);

     for(i = y; i < y + RACKETL; ++i)
          mvaddch(i, GX, ' ');
     UNCOL(1);

     return;

}

/* Raquette de l'IA (enfin IA...) */
void
ia_racket(void)
{
     int i;

     pong->racket.iay = pong->ball.y - (RACKETL / 2);

     if(pong->ball.y < 4)
          ++pong->racket.iay;
     if(pong->ball.y > FH - 3)
          --pong->racket.iay;

     for(i = 1; i < FH; ++i)
          mvaddch(i, GX + FW, ' ');

     COL(1);

     for(i = 0; i < RACKETL; ++i)
          mvaddch(i + pong->racket.iay, GX + FW, ' ');

     UNCOL(1);

     return;
}

int
main(int argc, char **argv)
{
     int time;

     pong = malloc(sizeof(Pong));

     pong->running = 1;
     pong->ball.a = 1;
     pong->ball.b = -1;
     pong->ball.x = 10;
     pong->ball.y = 10;

     init_curses();
     init_frame();

     /* Boucle principale */
     for(time = 0; pong->running; ++time)
     {
          key_event();

          if(time > TIMEDELAY)
          {
               ia_racket();
               manage_ball();
               time = 0;
          }

          usleep(1000);
     }

     endwin();
     free(pong);

     return 1;
}

