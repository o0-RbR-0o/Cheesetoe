#ifndef __GFX_H__
#define __GFX_H__
#include "../include/game_elements.h"
#include "textures_tpl.h"
#include "textures.h"


#define TILE_SIZE 32
#define NUM_TILES (640/TILE_SIZE)*(480/TILE_SIZE)

#define DEFAULT_FIFO_SIZE	(256*1024)

typedef struct{
	int x,
		y,
		x_n,
		y_n,
		animationinprogress,
		side,
		side_n,
		tick;

}Data_SetAnimation;

typedef struct{
 int 	row,
		column,
		tick,
		direction,
		animationinprogress;

}Data_ShiftAnimation;

typedef struct{
	int	x,
		y,
		tick,
		animationinprogress,
		length;
}Data_BombAnimation;


void *frameBuffer[2];
GXRModeObj *rmode;
u32	fb;
u32 first_frame;

GXTexObj 	backgroundelement,
			gridtile,
			curgfx,
			overlay,
			overlay_sides[4],
			clouds,
			side_tiles[4],
			owned_sign[3],
			paused_overlay,
			p_boxed[5],
			fadetoblack,
			bomb_middle,
			bomb_top_m,
			bomb_right_m,
			bomb_bottom_m,
			bomb_left_m,
			bomb_top_e,
			bomb_right_e,
			bomb_bottom_e,
			bomb_left_e,
			title_screen1,
			hud,
			squarewave,
			bignum[4],
			menu_extrasconfig,
			icon_score,
			icon_bomb,
			icon_bombstrength,
			icon_shift,
			hs_plus,
			hs_minus,
			hs_multi,
			hs_is,
			icon_controller,
			icon_x,
			icon_bot,
			hs_p[4],
			text_players,
			dummygfx,
			text_startwithpowerupspp,
			text_startwithpowerupsog,
			menucursor,
			menucursor2,
			numbers[10],
			title_screen2;
	

inline void drawSpriteTex( int x, int y, int width, int height, GXTexObj *texObj );
inline void drawAnimFrame( int x, int y, int width, int height, GXTexObj *texObj,int length, int number);
inline void gfx_init(void);
inline void draw_fog(void);
inline void load_game_gfx(void);
inline void fading_quad( int x, int y, int width, int height, char percentage);
inline void drawgrid(Grid *gamegrid);
inline void drawMouse(Mouse *mouse);
inline void drawcursor(Cursor *cur);
inline void draw_background(GXTexObj *backgroundelement);
inline int checkanimations(void);
inline void animation_setside(int side,int x,int y);
inline void animate_settile();
inline void animate_shift();
inline void animate();
inline void animation_shiftrow(int row,int direction);
inline void animation_shiftcolumn(int column,int direction);
inline void animation_bomb(int x,int y, int length);
inline int digit_to_int(char d);
inline void drawnumber(int number, int x, int y);
inline void drawhud();
inline void drawplayerstats(Player *player, int currentplayer);

#endif