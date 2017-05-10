#include "../include/ai.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _move {
	int x,
		y;
	int action;
	int direction;
	
} move;

void score_with(Grid *grid, move* m) {
	int i = 0;
	for(i=0; i < (grid->dimension_x*grid->dimension_y); i++){
		int j = 0;
		int sidecount = 0;
		for(j=0;j<4;j++){
			if(grid->tiles[i].sides_set[j]!=0) {
				sidecount++;
			}
		}
		if(sidecount == 3)
			for(j=0;j<4;j++){
				if(grid->tiles[i].sides_set[j]==0) {
					m->x = grid->tiles[i].position_x;
					m->y = grid->tiles[i].position_y;
					m->action = SETSIDE;
					m->direction = j;
					return;
				}
			}
	}
}

void perform_action(move *m) {
	start_ai_action(m->x, m->y, m->action, m->direction);
}

int count_sides(Grid *grid, int tile) {
	int sidecount = 0, j;
	for(j = 0; j < 4; j++){
		if(grid->tiles[tile].sides_set[j]) {
			sidecount++;
		}
	}
	return sidecount;
}

void random_move(Grid *grid, move *m) {
	int sidecount = 0, countcount = 0;
	int tile,
		init = rand() % grid->tilecount;
	do {
		tile = (init + countcount) % grid->tilecount;
		sidecount = count_sides(grid, tile);
		if(sidecount < 2) {
			int x = tile % grid->dimension_x,
				y = tile / grid->dimension_x;
			if(grid->tiles[tile].sides_set[LEFT] == 0) {
				if (x > 0 && count_sides(grid, tile - 1) >= 2)
					continue;
				else
					m->direction = LEFT;
			}
			if(m->direction == -1) {
				if(!grid->tiles[tile].sides_set[RIGHT] == 0) {
					if (x < grid->dimension_x && count_sides(grid, tile + 1) >= 2)
						continue;
					else
						m->direction = RIGHT;
					}
			}
			if(m->direction == -1) {
				if(!grid->tiles[tile].sides_set[TOP] == 0) {
					if (y > 0 && count_sides(grid, tile - grid->dimension_x) >= 2)
						continue;
					else
						m->direction = TOP;	
				}
			}
			if(m->direction == -1) {
				if(grid->tiles[tile].sides_set[BOTTOM] == 0) {
					if(y < grid->dimension_y && count_sides(grid, tile + grid->dimension_x) >= 2)
						continue;
					else
						m->direction = BOTTOM;
				}
			}
			if(m->direction != -1) {		
				m->action = SETSIDE;
				m->x = grid->tiles[tile].position_x;
				m->y = grid->tiles[tile].position_y;
				return;
			}
		}
	} while(countcount++ < 10000);
}

void ai(Grid *grid, Player *player) {
	move* m = (move*)malloc(sizeof(m));
	m->action = -1;
	m->direction = -1;
	
	score_with(grid, m);
	
	if(m->direction == -1)
		random_move(grid, m);
	
	
	
	if(m->direction == -1) {

		int val1, 
			val2,
			val3;
		int cnt=0;
		
		
		val1 = rand() % grid->dimension_x; 
		val2 = rand() % grid->dimension_y;
		val3 = rand() % 4;
		cnt=0;
		while(grid->tiles[val2*grid->dimension_x+val1].owned_by_Player){
			val1 = rand() % grid->dimension_x, val2 = rand() % grid->dimension_y;
		}
		while(grid->tiles[val2*grid->dimension_x+val1].sides_set[val3] && cnt<5){
			val3=(val3+1)%4;
			cnt++;
		}
		if(player->bombs && (rand()%20)==1)
			start_ai_action(val1,val2,BOMB, val3);
		else if(player->tileshifts && (rand()%5)==1)
			start_ai_action(val1,val2,SHIFT, val3);
		else
			start_ai_action(val1,val2,SETSIDE, val3);
		
	} else
		perform_action(m);
	free(m);
}
