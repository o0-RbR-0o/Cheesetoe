#include "../include/interface.h"
#include <math.h>
#include <stdlib.h>
#include "../include/ai.h"


/*	Schreibt die Koordinaten einer Linie von fromx,fromy bis tox,toy in 2 Arrays.
	Der KI-Mauszeiger wird später an dieser Linie entlangbewegt						*/
void ai_calc_bresenham_cursor_move(int fromx, int fromy, int tox, int toy){
  int i=0;
  for(i=0;i<801;i++){
	ai_brmovey[i]=-1;
	ai_brmovex[i]=-1;
  }
  int dx =  abs(tox-fromx), sx = fromx<tox ? 1 : -1;
  int dy = -abs(toy-fromy), sy = fromy<toy ? 1 : -1;
  int err = dx+dy, e2;
  i=0;
  for(;;){
	ai_brmovex[i]=fromx;
	ai_brmovey[i]=fromy;
	i++;
    if (fromx==tox && fromy==toy) break;
    e2 = 2*err;
    if (e2 > dy) { err += dy; fromx += sx; }
    if (e2 < dx) { err += dx; fromy += sy; }
  }
}
void init_ai(){
	ai_action=0;
	ai_position=0;
	ai_movelineactive=0;
	ai_start_action=0;
}

void do_ai_actions(Mouse *mouse, Controller *controller){
	controller->a=0;
	controller->b=0;
	controller->x=0;
	controller->y=0;
	controller->up=0;
	controller->down=0;
	controller->left=0;
	controller->right=0;
	if(ai_movelineactive){
		if(ai_calc_line){
			ai_calc_bresenham_cursor_move(mouse->posx, mouse->posy, (ai_coords[0]*32)+16, (ai_coords[1]*32)+16);
			ai_calc_line=0;
		}
		if(ai_brmovex[ai_position]!=-1){
			mouse->posx=ai_brmovex[ai_position];
			mouse->posy=ai_brmovey[ai_position];
			ai_position+=16;
		}
		else{
			ai_movelineactive=0;
			ai_position=0;
			controller->a=0;
			controller->b=0;
			controller->x=0;
			controller->y=0;
			controller->up=0;
			controller->down=0;
			controller->left=0;
			controller->right=0;
			if(ai_action==SETSIDE){
				controller->a=1;
				if		(ai_direction==TOP)
					controller->up=1;
				else if(ai_direction==RIGHT)
					controller->right=1;
				else if(ai_direction==BOTTOM)
					controller->down=1;
				else if(ai_direction==LEFT)
					controller->left=1;
				controller->b=1;
			}
			else if(ai_action==BOMB){
				controller->a=1;
				controller->y=1;
			
			}
			else if(ai_action==SHIFT){
				controller->a=1;
				if		(ai_direction==TOP)
					controller->up=1;
				else if(ai_direction==RIGHT)
					controller->right=1;
				else if(ai_direction==BOTTOM)
					controller->down=1;
				else if(ai_direction==LEFT)
					controller->left=1;
				controller->x=1;
			}
		}
	}
	else{
		init_ai();
		controller->a=0;
		controller->b=0;
		controller->x=0;
		controller->y=0;
		controller->up=0;
		controller->down=0;
		controller->left=0;
		controller->right=0;
		ai_calc_line=1;
	}
}

void start_ai_action(int x, int y, int action, char direction){
	if(!ai_movelineactive){
		ai_calc_line=1;
		ai_coords[0]=x;
		ai_coords[1]=y;
		ai_movelineactive=1;
		ai_action=action;
		ai_direction=direction;
	}
}


int ai_checkowned(Player *player, Grid *grid){
	int i=0;
	for(i=0;i<grid->tilecount;i++){
		if(grid->tiles[i].sides_set[0]==1){
			if(grid->tiles[i].position_y>0){
				set_tile_side(grid, 2,grid->tiles[i].position_x, grid->tiles[i].position_y-1);
			}
		}
		if(grid->tiles[i].sides_set[1]==1){
			if(grid->tiles[i].position_x<grid->dimension_x-1){ //laeuft
				grid->tiles[i+1].sides_set[3]=1;
			}
		}
		if(grid->tiles[i].sides_set[2]==1){
			if(grid->tiles[i].position_y<grid->dimension_y-1){
				set_tile_side(grid, 0,grid->tiles[i].position_x, grid->tiles[i].position_y+1);
			}
		}
		if(grid->tiles[i].sides_set[3]==1){
			if(grid->tiles[i].position_x>0){
				grid->tiles[i-1].sides_set[1]=1;
			}
		}
	}
	int ownedcount=0;
	for(i=0;i<grid->tilecount;i++){
		int sidecount=0;
		int j=0;
		for(j=0;j<4;j++){
			sidecount+=grid->tiles[i].sides_set[j];
		}
		if(sidecount==4){
			if(grid->tiles[i].owned_by_Player==0){
				grid->tiles[i].owned_by_Player=player;
				ownedcount++;
				//neues Kaestchen wurde geowned von Player
				
				if(grid->tiles[i].item==1){
					//Player Bekommt bombs++
					grid->tiles[i].item=0;
				}	
				if(grid->tiles[i].item==2){
					//Player bekommt Bombstrength++
					grid->tiles[i].item=0;
				}	
				if(grid->tiles[i].item==3){
					//Player bekommt Shifts++
					grid->tiles[i].item=0;
				}	
			}
		}
		if(sidecount<4){
			grid->tiles[i].owned_by_Player=0;
		}
	}
	return ownedcount; //Anzahl neu geownter Tiles zurückgeben
}


/*Hier ist quasi Skomps Main-Funktion.*/
void ai_main(Grid *grid, Player *player){
	ai(grid, player);
	/*int i=0;
	int foundaction=0;
	for(i=0;i<(grid->dimension_x*grid->dimension_y);i++){
		int j=0;
		int sidecount=0;
		for(j=0;j<4;j++){
			if(grid->tiles[i].sides_set[j]!=0){
				sidecount++;
			}
		}
		if(sidecount==3 &&foundaction==0){
			for(j=0;j<4;j++){
				if(grid->tiles[i].sides_set[j]==0){
					start_ai_action(grid->tiles[i].position_x, grid->tiles[i].position_y, SETSIDE, j);
					foundaction=1;
				}
			}
		}
	}
	if(foundaction==0){
		int val1=rand()%grid->dimension_x,val2=rand()%grid->dimension_y,val3=rand()%4;
		int cnt=0;
		while(grid->tiles[val2*grid->dimension_x+val1].owned_by_Player){
			val1=rand()%grid->dimension_x,val2=rand()%grid->dimension_y;
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
			foundaction=1;
		
	}*/

}