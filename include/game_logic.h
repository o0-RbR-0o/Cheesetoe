#ifndef __GAME_LOGIC_H_
#define __GAME_LOGIC_H_
#include "../include/game_elements.h"
#include "../include/gfx.h"


typedef struct{
	Player *currentplayer;
	int ispaused;
	int switchplayer;
	int end_round;
	Player *players[4];
	int game_ended;
}Gameglobals;

void register_player(Gameglobals *globals, Player *player, int reinit);
Gameglobals setup_gameglobals(void);
unsigned int cycleplayers(Gameglobals *globals, int reinit);
void checkowned(Gameglobals *globals, Grid *grid);
void checkactions(Grid *grid, Cursor *cur1, Gameglobals *globals);
#endif