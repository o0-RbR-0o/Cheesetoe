#include <gccore.h>
#include <stdlib.h>
#include "../include/game_logic.h"
#include "../include/sound.h"


Gameglobals setup_gameglobals(void){
	Gameglobals globals;
	globals.switchplayer=0;
	globals.ispaused=0;
	globals.currentplayer=0;
	globals.end_round=0;
	globals.game_ended=0;
	int i=0;
	for(i=0;i<4;i++)
		globals.players[i]=0;
	return globals;
}

void register_player(Gameglobals *globals, Player *player,int reinit){
	static int playercount=0;
	if(!reinit){
		if(playercount<4){
			globals->players[playercount]=player;
			playercount++;
		}
	}
	else{
		playercount=0;
	}
}

unsigned int cycleplayers(Gameglobals *globals, int reinit){
	static int cycler=0;
	int i=0;
	int pcount=0;
	if(!reinit){
		if(globals->end_round){
			for(i=0;i<4;i++)
				if(globals->players[i]!=0)
					pcount++;
			cycler=(cycler+1)%pcount;
			globals->currentplayer=globals->players[cycler];
			globals->end_round=0;
			return 1;
		}
	}
	else{
		cycler=0;
	}
	return 0;
}

Player *getCurrentPlayer(Gameglobals *globals){
	return globals->currentplayer;
}



void checkactions(Grid *grid, Cursor *cur1, Gameglobals *globals){
	if(!cur1->is_movable){
		if(cur1->setsSide){
			if(cur1->selected_side<4){
				if(!cur1->position->sides_set[cur1->selected_side]){
						SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)setside_raw, setside_raw_size, 255, 255, NULL);
						animation_setside(cur1->selected_side, cur1->position->position_x,cur1->position->position_y);
						cur1->position->sides_set[cur1->selected_side]=1;
						cur1->is_movable=1;
						cur1->selected_side=0;
						cur1->setsSide=0;
						globals->end_round=1;
				}
			}
		}
		if(cur1->shiftrow && globals->currentplayer->tileshifts >0){
			SND_SetVoice(3, VOICE_STEREO_16BIT, 44100, 0, (void*)woosh_raw, woosh_raw_size, 255, 255, NULL);
			animation_shiftrow(cur1->position->position_y,cur1->shiftrow-1);
			shift_row(grid, cur1->position->position_y,cur1->shiftrow-1);
			cur1->shiftrow=0;
			globals->end_round=1;
			cur1->is_movable=1;
			cur1->selected_side=0;
			globals->currentplayer->tileshifts--;
		}
		else if(cur1->shiftrow && globals->currentplayer->tileshifts <=0){
			cur1->shiftrow=0;
		}
		else if(cur1->shiftcolumn && globals->currentplayer->tileshifts >0){
			SND_SetVoice(3, VOICE_STEREO_16BIT, 44100, 0, (void*)woosh_raw, woosh_raw_size, 255, 255, NULL);
			animation_shiftcolumn(cur1->position->position_x,cur1->shiftcolumn-1);
			shift_column(grid, cur1->position->position_x,cur1->shiftcolumn-1);
			cur1->shiftcolumn=0;
			globals->end_round=1;
			cur1->is_movable=1;
			cur1->selected_side=0;
			globals->currentplayer->tileshifts--;
		}
		else if(cur1->shiftrow && globals->currentplayer->tileshifts <=0){
			cur1->shiftcolumn=0;
		}
		else if(cur1->bomb && globals->currentplayer->bombs >0){
			SND_SetVoice(4, VOICE_STEREO_16BIT, 44100, 0, (void*)explosion_raw, explosion_raw_size, 255, 255, NULL);
			animation_bomb(cur1->position->position_x,cur1->position->position_y, globals->currentplayer->bombstrength);
			bomb_position(grid, globals->currentplayer, cur1->position->position_x, cur1->position->position_y);
			cur1->bomb=0;
			globals->end_round=1;
			cur1->is_movable=1;
			cur1->selected_side=0;
			globals->currentplayer->bombs--;
		}
		else if(cur1->bomb && globals->currentplayer->bombs <=0){
			cur1->bomb=0;
		}
	}
}

void checkowned(Gameglobals *globals, Grid *grid){
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
				grid->tiles[i].owned_by_Player=globals->currentplayer;
				SND_SetVoice(1, VOICE_STEREO_16BIT, 44100, 0, (void*)boxed_raw, boxed_raw_size, 170, 170, NULL);
				globals->end_round=0;
				globals->currentplayer->score++;
				if(grid->tiles[i].item==1){
					globals->currentplayer->bombs++;
					grid->tiles[i].item=0;
				}	
				if(grid->tiles[i].item==2){
					globals->currentplayer->bombstrength++;
					grid->tiles[i].item=0;
				}	
				if(grid->tiles[i].item==3){
					globals->currentplayer->tileshifts++;
					grid->tiles[i].item=0;
				}	
			}
			ownedcount++;
		}
		if(sidecount<4){
			grid->tiles[i].owned_by_Player=0;
		}
	}
	if(ownedcount>=grid->tilecount){
		//Hier ist das Spiel zuende!!
		globals->game_ended=1;
	}
	
}