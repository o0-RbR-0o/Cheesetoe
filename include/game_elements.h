#ifndef __GAME_ELEMENTS_H__
#define __GAME_ELEMENTS_H__


/*	Struct for Player																		*/
typedef struct{
	unsigned int number;
	unsigned int score;
	unsigned int bombs;
	unsigned int tileshifts;
	unsigned int bombstrength;
	int controllernumber;
}Player;

/*	Struct for Tile																			*/
typedef struct{
	unsigned int position_x;
	unsigned int position_y;
	unsigned int item;
	unsigned int sides_set[4]; /*4 Sides: TRBL ;)*/
	Player *owned_by_Player;
}Tile;

/*	Struct for Grid																			*/
typedef struct{
	unsigned int dimension_x;
	unsigned int dimension_y;
	unsigned int tilecount;
	Tile *tiles;
}Grid;

/*	Struct for Cursor																		*/
typedef struct{
	Tile 	*position;
	unsigned int is_movable;
	unsigned int selected_side; /*0-3 TRBL*/
	unsigned int setsSide;
	unsigned int shiftrow;
	unsigned int shiftcolumn;
	unsigned int bomb;
}Cursor;

typedef struct{
	int posx;
	int posy;
}Mouse;


void initMouse(Mouse *mouse);

/*------------------------------------------------------------------------------------------*/
/*	Init functions																			*/
/*------------------------------------------------------------------------------------------*/

/*	Initializes a player																	*/
Player init_player(unsigned int number);

/*	Initializes a tile																		*/
Tile init_tile(unsigned int position_x, unsigned int position_y);

/*	Initializes a grid																		*/
void init_grid(Grid *grid, unsigned int dimension_x,unsigned int dimension_y, unsigned int bombs, unsigned int bombstrength,unsigned int shifts, int randseed);

/*	Initializes a cursor																	*/
Cursor init_cursor(Player *player,Tile *tile);

Grid init_gridcopy(Grid *grid);
void free_grid(Grid *grid);

/*	copies grid properties																*/
void copy_grid_properties(Grid *grid, Grid *gridcopy);

/*------------------------------------------------------------------------------------------*/
/*	Basic cursor functions																	*/
/*------------------------------------------------------------------------------------------*/

/*	Sets the cursor position to a tile														*/
void set_cursor_position(Cursor *cursor, Tile *tile);

/*	Gets the tile the cursor is currently set to											*/
Tile *get_cursor_position(Cursor *cursor);

/*	Sets the player controlling the cursor													*/
void set_cursor_player(Cursor *cursor, Player *player);

/*	Gets the player controlling the cursor													*/
Player *get_cursor_player(Cursor *cursor);

/*	Sets the cursor movable status															*/
void set_cursor_movable(Cursor *cursor, unsigned int movable_status);

/*	Gets the cursor movable status															*/
unsigned int get_cursor_movable(Cursor *cursor);

/*	Sets the cursor selected side															*/
void set_cursor_side(Cursor *cursor, unsigned int side);									

/*	Gets the cursor selected side 															*/
unsigned int get_cursor_side(Cursor *cursor);


/*------------------------------------------------------------------------------------------*/
/*	Elements special functions																*/
/*------------------------------------------------------------------------------------------*/

/*	Sets the owner of a tile by pointing <owned_by_Player> to <player>						*/
void own_tile(Grid *grid, Player *player,unsigned int x, unsigned int y);

/*	Deletes the owner of a tile by pointing <owned_by_Player> to NULL						*/
void deown_tile(Grid *grid, unsigned int x, unsigned int y);

/*	Sets <side> (0-3 -> TRBL *greets*) of a tile at <x> and <y> position to blocked			*/
void set_tile_side(Grid *grid, unsigned int side, unsigned int x, unsigned int y);

/*	Clears a set <side> of a tile (0-3 -> TRBL *greets*) at <x> and <y> position 			*/
void clear_tile_side(Grid *grid, unsigned int side, unsigned int x, unsigned int y);
	
/*	Clears all sides of a tile at <x> and <y> position										*/
void clear_tile(Grid *grid, unsigned int x, unsigned int y);

/*	Shifts a row <row_number> one unit in direction <direction> (0 left, 1 right)			*/
void shift_row(Grid *grid, int row_number, int direction);

/*	Shifts a column <column_number> one unit in direction <direction> (0 up, 1 down)		*/
void shift_column(Grid *grid, int column_number, int direction);

/*	"Bombs" the given tile at <pos_x>, <pos_y>, Bomberman style, kinda (+Bomb strength x,y)	*/
void bomb_position(Grid *grid, Player *player, unsigned int pos_x, unsigned int pos_y);

#endif