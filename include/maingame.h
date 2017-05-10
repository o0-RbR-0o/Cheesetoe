#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "../include/game_elements.h"
Mtx GXmodelView2D;
typedef struct{
	char playercontrollers[4];
	char extras_on_grid[3];
	char extras_per_player[3];
}Init_Struct;

typedef struct{
	unsigned int score[4];
	unsigned int bombs[4];
	unsigned int shifts[4];
	unsigned int bombstrength[4];
	char players_valid[4];
}Game_Results;

void show_logo();
Game_Results game_main(Grid *gamegrid, Init_Struct inital_vals);



#endif