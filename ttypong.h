#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

/* Options */
#define FH 20
#define FW 60
#define RACKETL 3
#define TIMEDELAY 40
#define BALL '0'

/* Macros */
#define GX ((COLS / 2) - (FW / 2))
#define GY ((LINES / 2) - (FH / 2)) - 1
#define COL(x)    attron(COLOR_PAIR(x))
#define UNCOL(x)  attroff(COLOR_PAIR(x))

/* Struct */
typedef struct
{
     int running;

     struct
     {
          /* Ball position */
          int x, y;

          /* Incrementer (Oo ?) */
          int a, b;
     } ball;

     struct
     {
          int y, iay;
     } racket;

} Pong;

/* Prototypes */
void init_curses(void);
void init_frame(void);
void key_event(void);
void manage_ball(void);
void racket_move(int y);
void ia_racket(void);

Pong *pong;
