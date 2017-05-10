#include "../include/game_elements.h"
#include <malloc.h>
#include <stdlib.h>

void initMouse(Mouse *mouse){
	mouse->posx=0;
	mouse->posy=0;
}

/*	Initializes a player																*/
Player init_player(unsigned int number){
	Player player;
	player.number=number;
	player.score=0;
	player.bombs=0;
	player.bombstrength=0;
	player.tileshifts=0;
	return player;
}

/*	Initializes a tile																	*/
Tile init_tile(unsigned int position_x, unsigned int position_y){
	Tile tile;
	tile.position_x=position_x;
	tile.position_y=position_y;
	tile.item=0;
	int i=0;
	for(i=0;i<4;i++)
		tile.sides_set[i]=0;
	tile.owned_by_Player=0;
	return tile;
}

/*	Initializes a grid																	*/
void init_grid(Grid *grid, unsigned int dimension_x,unsigned int dimension_y, unsigned int bombs, unsigned int bombstrength,unsigned int shifts, int randseed){
	grid->dimension_x=dimension_x;
	grid->dimension_y=dimension_y;
	grid->tilecount=dimension_x*dimension_y;
	srand(randseed);
	if(grid->tiles!=0){
		free(grid->tiles);
	}
	grid->tiles=(Tile*)malloc(sizeof(Tile)*(dimension_x*dimension_y));
	int i=0;
	for(i=0;i<(grid->tilecount);i++){
		grid->tiles[i]=init_tile(i%dimension_x,i/dimension_x);
		
	}
	for(i=0;i<450;i++){
		int randtile=(rand()%grid->tilecount-1);
		int j=0;
		int count=0;
		for(j=0;j<4;j++){
			if(grid->tiles[randtile].sides_set[j])
				count++;
		}
		if(count<2)
			grid->tiles[randtile].sides_set[i%4]=1;	
		for(j=0;j<grid->tilecount;j++){
		
			if(grid->tiles[j].sides_set[0]==1){
				if(grid->tiles[j].position_y>0){
					set_tile_side(grid, 2,grid->tiles[j].position_x, grid->tiles[j].position_y-1);
				}
			}
			if(grid->tiles[j].sides_set[1]==1){
				if(grid->tiles[j].position_x<grid->dimension_x-1){ //laeuft
					grid->tiles[j+1].sides_set[3]=1;
				}
			}
			if(grid->tiles[j].sides_set[2]==1){
				if(grid->tiles[j].position_y<grid->dimension_y-1){
					set_tile_side(grid, 0,grid->tiles[j].position_x, grid->tiles[j].position_y+1);
				}
			}
			if(grid->tiles[j].sides_set[3]==1){
				if(grid->tiles[j].position_x>0){
					grid->tiles[j-1].sides_set[1]=1;
				}
			}
		}
		for(j=0;j<grid->tilecount;j++){
			int k=0, owncount=0;
			for(k=0;k<4;k++)
				owncount+=grid->tiles[j].sides_set[k];
			if(owncount>2){
				for(k=0;k<4;k++){
					clear_tile_side(grid,k,grid->tiles[j].position_x,grid->tiles[j].position_y);
					clear_tile_side(grid,k,grid->tiles[j].position_x+1,grid->tiles[j].position_y);
					clear_tile_side(grid,k,grid->tiles[j].position_x-1,grid->tiles[j].position_y);
					clear_tile_side(grid,k,grid->tiles[j].position_x,grid->tiles[j].position_y+1);
					clear_tile_side(grid,k,grid->tiles[j].position_x,grid->tiles[j].position_y-1);
				}
			}
		}
	}
	
	while((bombs+bombstrength+shifts)>0){
		int randtile=(rand()%grid->tilecount-1);
		if(grid->tiles[randtile].item<=0){
			if(bombs>0){
				grid->tiles[randtile].item=1;
				bombs--;
			}
			else if(bombstrength>0){
				grid->tiles[randtile].item=2;
				bombstrength--;
			}
			else if(shifts>0){
				grid->tiles[randtile].item=3;
				shifts--;
			}
		}
	}
}

Grid init_gridcopy(Grid *grid){
	Grid gridcopy;
	gridcopy.dimension_x=grid->dimension_x;
	gridcopy.dimension_y=grid->dimension_y;
	gridcopy.tilecount=grid->dimension_x*grid->dimension_y;
	gridcopy.tiles=(Tile*)malloc(sizeof(Tile)*(gridcopy.dimension_x*gridcopy.dimension_y));
	return gridcopy;
}
void free_grid(Grid *grid){
	if(grid->tiles!=0){
		free(grid->tiles);
	}
}


/*	copies grid properties																*/
void copy_grid_properties(Grid *grid, Grid *gridcopy){
	int i=0;
	for(i=0;i<(gridcopy->tilecount);i++){
		gridcopy->tiles[i]=init_tile(i%gridcopy->dimension_x,i/gridcopy->dimension_x);
		
		gridcopy->tiles[i].position_x=grid->tiles[i].position_x;
		gridcopy->tiles[i].position_y=grid->tiles[i].position_y;
		gridcopy->tiles[i].item=grid->tiles[i].item;
		gridcopy->tiles[i].owned_by_Player=grid->tiles[i].owned_by_Player;
		int j=0;
		for(j=0;j<4;j++)
			gridcopy->tiles[i].sides_set[j]=grid->tiles[i].sides_set[j];
	}
}



/*	Initializes the cursor																*/
Cursor init_cursor(Player *player,Tile *tile){
	Cursor cursor;
	cursor.position=tile;
	cursor.is_movable=1;
	cursor.selected_side=4;
	cursor.setsSide=0;
	cursor.shiftrow=0;
	cursor.shiftcolumn=0;
	cursor.bomb=0;
	return cursor;
}


/*	Sets the cursor position to a tile													*/
void set_cursor_position(Cursor *cursor, Tile *tile){
	cursor->position=tile;
}

/*	Gets the tile the cursor is currently set to										*/
Tile *get_cursor_position(Cursor *cursor){
	return cursor->position;
}

/*	Sets the cursor movable status														*/
void set_cursor_movable(Cursor *cursor, unsigned int movable_status){
	cursor->is_movable=movable_status;
}

/*	Sets the cursor movable status														*/
unsigned int get_cursor_movable(Cursor *cursor){
	return cursor->is_movable;
}

/*	Sets the cursor selected side														*/
void set_cursor_side(Cursor *cursor, unsigned int side){
	cursor->selected_side=side;
}

/*	Gets the cursor selected side 														*/
unsigned int get_cursor_side(Cursor *cursor){
	return cursor->selected_side;
}

/*	Sets the owner of a tile by pointing <owned_by_Player> to <player>					*/
void own_tile(Grid *grid, Player *player,unsigned int x, unsigned int y){
	if(x>=0 && x<grid->dimension_x && y>=0 && y<grid->dimension_y)
		grid->tiles[y*grid->dimension_x+x].owned_by_Player=player;

}

/*	Deletes the owner of a tile by pointing <owned_by_Player> to NULL					*/
void deown_tile(Grid *grid, unsigned int x, unsigned int y){
	if(x>=0 && x<grid->dimension_x && y>=0 && y<grid->dimension_y)
		grid->tiles[y*grid->dimension_x+x].owned_by_Player=0;

}

/*	Sets <side> (0-3 -> TRBL *greets*) of a tile at <x> and <y> position to blocked		*/
void set_tile_side(Grid *grid, unsigned int side, unsigned int x, unsigned int y){
	if(x>=0 && x<grid->dimension_x && y>=0 && y<grid->dimension_y)
		grid->tiles[y*grid->dimension_x+x].sides_set[side]=1;

}

/*	Clears a set <side> of a tile (0-3 -> TRBL *greets*) at <x> and <y> position 		*/
void clear_tile_side(Grid *grid, unsigned int side, unsigned int x, unsigned int y){
	if(x>=0 && x<grid->dimension_x && y>=0 && y<grid->dimension_y)
		grid->tiles[y*grid->dimension_x+x].sides_set[side]=0;

}

/*	Clears all sides of a tile at <x> and <y> position									*/
void clear_tile(Grid *grid, unsigned int x, unsigned int y){
	int i=0;
	for(i=0;i<4;i++){
		clear_tile_side(grid, i, x, y );
	}
}

/*	Shifts a row <row_number> one unit in direction <direction> (0 left, 1 right)		*/
/*	Doesn't actually shift the tiles but the attributes, except position of course		*/
void shift_row(Grid *grid, int row_number, int direction){
	
	/*Shift left*/
	if(direction==0){
		Tile temptile[3];
		temptile[0]=grid->tiles[row_number*grid->dimension_x];
		temptile[1]=grid->tiles[(row_number-1)*grid->dimension_x];
		temptile[2]=grid->tiles[(row_number+1)*grid->dimension_x];
		int i=0;
		for(i=0;i<grid->dimension_x-1;i++){
			int j=0;
			for(j=0;j<4;j++){
				grid->tiles[row_number*grid->dimension_x+i].sides_set[j]=grid->tiles[row_number*grid->dimension_x+(i+1)].sides_set[j];
			}
			if(row_number!=0)
				grid->tiles[(row_number-1)*grid->dimension_x+i].sides_set[2]=grid->tiles[(row_number-1)*grid->dimension_x+(i+1)].sides_set[2];
			grid->tiles[(row_number+1)*grid->dimension_x+i].sides_set[0]=grid->tiles[(row_number+1)*grid->dimension_x+(i+1)].sides_set[0];
			
			grid->tiles[row_number*grid->dimension_x+i].item=grid->tiles[row_number*grid->dimension_x+(i+1)].item;
			grid->tiles[row_number*grid->dimension_x+i].owned_by_Player=grid->tiles[row_number*grid->dimension_x+(i+1)].owned_by_Player;
		}
		for(i=0;i<4;i++){
			grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1)].sides_set[i]=temptile[0].sides_set[i];
		}
		if(row_number!=0)
			grid->tiles[(row_number-1)*grid->dimension_x+(grid->dimension_x-1)].sides_set[2]=temptile[1].sides_set[2];
		if(row_number!=grid->dimension_y-1)
			grid->tiles[(row_number+1)*grid->dimension_x+(grid->dimension_x-1)].sides_set[0]=temptile[2].sides_set[0];
		grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1)].item=temptile[0].item;
		grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1)].owned_by_Player=temptile[0].owned_by_Player;
	}
	
	/*Shift right*/
	if(direction==1){
		Tile temptile[3];
		temptile[0]=grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1)];
		temptile[1]=grid->tiles[(row_number-1)*grid->dimension_x+(grid->dimension_x-1)];
		temptile[2]=grid->tiles[(row_number+1)*grid->dimension_x+(grid->dimension_x-1)];
		int i=0;
		for(i=0;i<grid->dimension_x-1;i++){
			int j=0;
			for(j=0;j<4;j++){
				grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)].sides_set[j]=grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)-1].sides_set[j];
			}
			if(row_number!=0)
				grid->tiles[(row_number-1)*grid->dimension_x+(grid->dimension_x-1-i)].sides_set[2]=grid->tiles[(row_number-1)*grid->dimension_x+(grid->dimension_x-1-i)-1].sides_set[2];
			if(row_number!=grid->dimension_y-1)
				grid->tiles[(row_number+1)*grid->dimension_x+(grid->dimension_x-1-i)].sides_set[0]=grid->tiles[(row_number+1)*grid->dimension_x+(grid->dimension_x-1-i)-1].sides_set[0];
			grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)].item=grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)-1].item;
			grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)].owned_by_Player=grid->tiles[row_number*grid->dimension_x+(grid->dimension_x-1-i)-1].owned_by_Player;
		}
		for(i=0;i<4;i++){
			grid->tiles[row_number*grid->dimension_x].sides_set[i]=temptile[0].sides_set[i];
		}
		if(row_number!=0)
			grid->tiles[(row_number-1)*grid->dimension_x].sides_set[2]=temptile[1].sides_set[2];
		if(row_number!=grid->dimension_y-1)
			grid->tiles[(row_number+1)*grid->dimension_x].sides_set[0]=temptile[2].sides_set[0];
		
		grid->tiles[row_number*grid->dimension_x].item=temptile[0].item;
		grid->tiles[row_number*grid->dimension_x].owned_by_Player=temptile[0].owned_by_Player;
	}
}



/*	Shifts a column <column_number> one unit in direction <direction> (0 up, 1 down)		*/
/*	Doesn't actually shift the tiles but the attributes, except position of course			*/
void shift_column(Grid *grid, int column_number, int direction){
	
	/*Shift up*/
	if(direction==0){
		Tile temptile[3];
		temptile[0]=grid->tiles[grid->dimension_x-1*grid->dimension_x+column_number];
		temptile[1]=grid->tiles[column_number-1];
		temptile[2]=grid->tiles[column_number+1];
		int i=0;
		for(i=0;i<grid->dimension_y-1;i++){
			int j=0;
			for(j=0;j<4;j++){
				grid->tiles[i*grid->dimension_x+column_number].sides_set[j]=grid->tiles[(i+1)*grid->dimension_x+column_number].sides_set[j];
			}
			if(column_number!=0)
				grid->tiles[i*grid->dimension_x+(column_number-1)].sides_set[1]=grid->tiles[(i+1)*grid->dimension_x+(column_number-1)].sides_set[1];
			if(column_number!=grid->dimension_x-1)
				grid->tiles[i*grid->dimension_x+(column_number+1)].sides_set[3]=grid->tiles[(i+1)*grid->dimension_x+(column_number+1)].sides_set[3];
			grid->tiles[i*grid->dimension_x+column_number].item=grid->tiles[(i+1)*grid->dimension_x+column_number].item;
			grid->tiles[i*grid->dimension_x+column_number].owned_by_Player=grid->tiles[(i+1)*grid->dimension_x+column_number].owned_by_Player;
		}
		for(i=0;i<4;i++){
			grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].sides_set[i]=temptile[0].sides_set[i];
		}
		if(column_number!=0)
			grid->tiles[(grid->dimension_y-1)*grid->dimension_x+(column_number-1)].sides_set[1]=temptile[1].sides_set[1];
		if(column_number!=grid->dimension_x-1)
			grid->tiles[(grid->dimension_y-1)*grid->dimension_x+(column_number+1)].sides_set[3]=temptile[2].sides_set[3];
		grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].item=temptile[0].item;
		grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].owned_by_Player=temptile[0].owned_by_Player;
	}
	
	/*Shift down*/
	if(direction==1){
		int c=0;
		for(c=0;c<grid->dimension_y-1;c++){
			Tile temptile[3];
			temptile[0]=grid->tiles[grid->dimension_x-1*grid->dimension_x+column_number];
			temptile[1]=grid->tiles[column_number-1];
			temptile[2]=grid->tiles[column_number+1];
			int i=0;
			for(i=0;i<grid->dimension_y-1;i++){
				int j=0;
				for(j=0;j<4;j++){
					grid->tiles[i*grid->dimension_x+column_number].sides_set[j]=grid->tiles[(i+1)*grid->dimension_x+column_number].sides_set[j];
				}
				if(column_number!=0)
					grid->tiles[i*grid->dimension_x+(column_number-1)].sides_set[1]=grid->tiles[(i+1)*grid->dimension_x+(column_number-1)].sides_set[1];
				if(column_number!=grid->dimension_x-1)
					grid->tiles[i*grid->dimension_x+(column_number+1)].sides_set[3]=grid->tiles[(i+1)*grid->dimension_x+(column_number+1)].sides_set[3];
				grid->tiles[i*grid->dimension_x+column_number].item=grid->tiles[(i+1)*grid->dimension_x+column_number].item;
				grid->tiles[i*grid->dimension_x+column_number].owned_by_Player=grid->tiles[(i+1)*grid->dimension_x+column_number].owned_by_Player;
			}
			for(i=0;i<4;i++){
				grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].sides_set[i]=temptile[0].sides_set[i];
			}
			if(column_number!=0)
				grid->tiles[(grid->dimension_y-1)*grid->dimension_x+(column_number-1)].sides_set[1]=temptile[1].sides_set[1];
			if(column_number!=grid->dimension_x-1)
				grid->tiles[(grid->dimension_y-1)*grid->dimension_x+(column_number+1)].sides_set[3]=temptile[2].sides_set[3];
			grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].item=temptile[0].item;
			grid->tiles[(grid->dimension_y-1)*grid->dimension_x+column_number].owned_by_Player=temptile[0].owned_by_Player;
		}
	}
}

/*	"Bombs" the given tile at <pos_x>, <pos_y>, Bomberman style, kinda (+Bomb strength x,y)	*/
void bomb_position(Grid *grid, Player *player, unsigned int pos_x, unsigned int pos_y){
	int i=0;
	clear_tile(grid,pos_x,pos_y);
	for(i=0;i<player->bombstrength+1;i++){
		
			clear_tile(grid,pos_x,pos_y-i);			
			clear_tile(grid,pos_x,pos_y+i);		
			clear_tile(grid,pos_x-i,pos_y);		
			clear_tile(grid,pos_x+i,pos_y);
			
			clear_tile_side(grid,0,pos_x,pos_y-i+1);			
			clear_tile_side(grid,0,pos_x,pos_y+i+1);		
			clear_tile_side(grid,0,pos_x-i,pos_y+1);		
			clear_tile_side(grid,0,pos_x+i,pos_y+1);
			
			clear_tile_side(grid,1,pos_x-1,pos_y-i);			
			clear_tile_side(grid,1,pos_x-1,pos_y+i);		
			clear_tile_side(grid,1,pos_x-1-i,pos_y);		
			clear_tile_side(grid,1,pos_x-1+i,pos_y);
			
			clear_tile_side(grid,2,pos_x,pos_y-i-1);			
			clear_tile_side(grid,2,pos_x,pos_y+i-1);		
			clear_tile_side(grid,2,pos_x-i,pos_y-1);		
			clear_tile_side(grid,2,pos_x+i,pos_y-1);
			
			clear_tile_side(grid,3,pos_x+1,pos_y-i);			
			clear_tile_side(grid,3,pos_x+1,pos_y+i);		
			clear_tile_side(grid,3,pos_x-i+1,pos_y);		
			clear_tile_side(grid,3,pos_x+i+1,pos_y);		
	}
}