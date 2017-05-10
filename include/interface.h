#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "../include/game_elements.h"
#include "../include/controls.h"
#define SETSIDE 1
#define BOMB 2
#define SHIFT 3
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3
short ai_brmovex[801];
short ai_brmovey[801];
char ai_action;
char ai_direction;
char ai_movelineactive;
int ai_position;
char ai_start_action;
char ai_calc_line;
unsigned int ai_coords[2];

void ai_calc_bresenham_cursor_move(int fromx, int fromy, int tox, int toy);

void do_ai_actions(Mouse *mouse, Controller *controller);
void init_ai();
void start_ai_action(int x, int y, int action, char direction);

void ai_main(Grid *grid, Player *player);
#endif