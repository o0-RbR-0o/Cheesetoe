#ifndef __CONTROLS_H__
#define __CONTROLS_H__
#include "../include/game_elements.h"

typedef struct{
	Player *assigned_player;
	int number;
	int rumbletime;
	int left,
		right,
		up,
		down,
		a,
		b,
		x,
		y,
		start,
		stickx,
		sticky;
}Controller;
	
void initController(Controller *controller, int controllernumber);
void checkrumble(Controller *controller);
void controlMouse(Mouse *mouse,Controller *controller);
void controlCursor(Grid *gamegrid, Mouse *mouse, Cursor *cursor, Controller *controller);
void controlInputs(Grid *gamegrid, Mouse *mouse, Cursor *cursor, Controller *controller);
void cacheinputs(Controller *controller);
#endif