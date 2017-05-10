#include <gccore.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/gfx.h"




Data_SetAnimation setAnimationData;
Data_ShiftAnimation shiftAnimationData;
Data_BombAnimation bombAnimationData;

//Texturiertes Quad an Position x,y mit Dimensionen w,h zeichnen
inline void drawSpriteTex( int x, int y, int width, int height, GXTexObj *texObj  ) {
	GX_LoadTexObj(texObj, GX_TEXMAP0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);			// Draw A Quad
		GX_Position2f32(x, y);					// Top Left
		GX_TexCoord2f32(0,0);
		GX_Position2f32(x+width, y);			// Top Right
		GX_TexCoord2f32(1.0,0);
		GX_Position2f32(x+width,y+height);	// Bottom Right
		GX_TexCoord2f32(1.0,1.0);
		GX_Position2f32(x,y+height);			// Bottom Left
		GX_TexCoord2f32(0.0,1.0);
	GX_End();									// Done Drawing The Quad 
}

inline void drawAnimFrame( int x, int y, int width, int height, GXTexObj *texObj,int length, int number) {
	GX_LoadTexObj(texObj, GX_TEXMAP0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);			// Draw A Quad
		GX_Position2f32(x, y);					// Top Left
		GX_TexCoord2f32((float)number/(float)length,0);
		GX_Position2f32(x+width, y);			// Top Right
		GX_TexCoord2f32((float)(number+1)/(float)length,0);
		GX_Position2f32(x+width,y+height);	// Bottom Right
		GX_TexCoord2f32((float)(number+1)/(float)length,1.0);
		GX_Position2f32(x,y+height);			// Bottom Left
		GX_TexCoord2f32((float)number/(float)length,1.0);
	GX_End();									// Done Drawing The Quad 
}


inline void fading_quad( int x, int y, int width, int height, char percentage) {
	if(percentage>=100)
		percentage=99;
	if(percentage<=0)
		percentage=1;
	double position=percentage/(double)100;
	GX_LoadTexObj(&fadetoblack, GX_TEXMAP0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);			// Draw A Quad
		GX_Position2f32(x, y);					// Top Left
		GX_TexCoord2f32(0.0+position,0);
		GX_Position2f32(x+width, y);			// Top Right
		GX_TexCoord2f32(0.0+position,0);
		GX_Position2f32(x+width,y+height);	// Bottom Right
		GX_TexCoord2f32(0.0+position,0.0);
		GX_Position2f32(x,y+height);			// Bottom Left
		GX_TexCoord2f32(0.0+position,0.0);
	GX_End();									// Done Drawing The Quad 
}



inline void draw_fog(void){
	static double sinvalue=0.0;
	static int cycles=0.0;
	drawSpriteTex(-64+sin(sinvalue)*64,0-cycles%480,640+128,960,&clouds);
	drawSpriteTex(-32+cos(sinvalue/4)*32,0-cycles/2,640+128,960,&clouds);
	cycles=(cycles+1)%960;
	sinvalue+=0.01;
}

inline void load_game_gfx(void){
	

	TPLFile spriteTPL;	
	TPL_OpenTPLFromMemory(&spriteTPL, (void *)textures_tpl,textures_tpl_size);
	
	TPL_GetTexture(&spriteTPL,game_backgroundgfx,&backgroundelement);
	TPL_GetTexture(&spriteTPL,gridtilegfx,&gridtile);
	TPL_GetTexture(&spriteTPL,cursor,&curgfx);
	TPL_GetTexture(&spriteTPL,overlaygfx,&overlay);
	TPL_GetTexture(&spriteTPL,overlaygfx_t,&overlay_sides[0]);
	TPL_GetTexture(&spriteTPL,overlaygfx_r,&overlay_sides[1]);
	TPL_GetTexture(&spriteTPL,overlaygfx_b,&overlay_sides[2]);
	TPL_GetTexture(&spriteTPL,overlaygfx_l,&overlay_sides[3]);
	TPL_GetTexture(&spriteTPL,cloudsgfx,&clouds);
	TPL_GetTexture(&spriteTPL,sidetile_t,&side_tiles[0]);
	TPL_GetTexture(&spriteTPL,sidetile_r,&side_tiles[1]);
	TPL_GetTexture(&spriteTPL,sidetile_b,&side_tiles[2]);
	TPL_GetTexture(&spriteTPL,sidetile_l,&side_tiles[3]);
	TPL_GetTexture(&spriteTPL,owned_gfx_p1,&owned_sign[1]);
	TPL_GetTexture(&spriteTPL,owned_gfx_p2,&owned_sign[2]);
	
	
	TPL_GetTexture(&spriteTPL,paused_overlay_gfx,&paused_overlay);
	TPL_GetTexture(&spriteTPL,p1_boxed_gfx,&p_boxed[1]);
	TPL_GetTexture(&spriteTPL,p2_boxed_gfx,&p_boxed[2]);
	TPL_GetTexture(&spriteTPL,p3_boxed_gfx,&p_boxed[3]);
	TPL_GetTexture(&spriteTPL,p4_boxed_gfx,&p_boxed[4]);
	TPL_GetTexture(&spriteTPL,fadetoblack_gfx,&fadetoblack);
	TPL_GetTexture(&spriteTPL,bombmiddle_gfx,&bomb_middle);
	
	
	
	
	
	
	TPL_GetTexture(&spriteTPL,bomb_top_m_gfx,&bomb_top_m);
	TPL_GetTexture(&spriteTPL,bomb_right_m_gfx,&bomb_right_m);
	TPL_GetTexture(&spriteTPL,bomb_bottom_m_gfx,&bomb_bottom_m);
	TPL_GetTexture(&spriteTPL,bomb_left_m_gfx,&bomb_left_m);
	TPL_GetTexture(&spriteTPL,bomb_top_e_gfx,&bomb_top_e);
	TPL_GetTexture(&spriteTPL,bomb_right_e_gfx,&bomb_right_e);
	TPL_GetTexture(&spriteTPL,bomb_bottom_e_gfx,&bomb_bottom_e);
	TPL_GetTexture(&spriteTPL,bomb_left_e_gfx,&bomb_left_e);
	TPL_GetTexture(&spriteTPL,title_gfx,&title_screen1);
	
	TPL_GetTexture(&spriteTPL,hud_gfx,&hud);
	TPL_GetTexture(&spriteTPL,squarewave_gfx,&squarewave);
	
	TPL_GetTexture(&spriteTPL,numbers_0_gfx,&numbers[0]);
	TPL_GetTexture(&spriteTPL,numbers_1_gfx,&numbers[1]);
	TPL_GetTexture(&spriteTPL,numbers_2_gfx,&numbers[2]);
	TPL_GetTexture(&spriteTPL,numbers_3_gfx,&numbers[3]);
	TPL_GetTexture(&spriteTPL,numbers_4_gfx,&numbers[4]);
	TPL_GetTexture(&spriteTPL,numbers_5_gfx,&numbers[5]);
	TPL_GetTexture(&spriteTPL,numbers_6_gfx,&numbers[6]);
	TPL_GetTexture(&spriteTPL,numbers_7_gfx,&numbers[7]);
	TPL_GetTexture(&spriteTPL,numbers_8_gfx,&numbers[8]);
	TPL_GetTexture(&spriteTPL,numbers_9_gfx,&numbers[9]);
	
	TPL_GetTexture(&spriteTPL,icon_bomb_gfx,&icon_bomb);
	TPL_GetTexture(&spriteTPL,icon_bombstrength_gfx,&icon_bombstrength);
	TPL_GetTexture(&spriteTPL,icon_shift_gfx,&icon_shift);
	TPL_GetTexture(&spriteTPL,icon_score_gfx,&icon_score);
	TPL_GetTexture(&spriteTPL,icon_controller_gfx,&icon_controller);
	TPL_GetTexture(&spriteTPL,icon_x_gfx,&icon_x);
	TPL_GetTexture(&spriteTPL,icon_bot_gfx,&icon_bot);
	TPL_GetTexture(&spriteTPL,title_gfx2,&title_screen2);
	TPL_GetTexture(&spriteTPL,menucursor_gfx,&menucursor);
	TPL_GetTexture(&spriteTPL,menucursor2_gfx,&menucursor2);
	TPL_GetTexture(&spriteTPL,dummygfx_gfx,&dummygfx);
	
	TPL_GetTexture(&spriteTPL,bignum_1_gfx,&bignum[0]);
	TPL_GetTexture(&spriteTPL,bignum_2_gfx,&bignum[1]);
	TPL_GetTexture(&spriteTPL,bignum_3_gfx,&bignum[2]);
	TPL_GetTexture(&spriteTPL,bignum_4_gfx,&bignum[3]);
	TPL_GetTexture(&spriteTPL,text_players_gfx,&text_players);
	
	TPL_GetTexture(&spriteTPL,menu_extrasconfig_gfx,&menu_extrasconfig);
	
	TPL_GetTexture(&spriteTPL,text_startwithpowerupspp_gfx,&text_startwithpowerupspp);
	TPL_GetTexture(&spriteTPL,text_startwithpowerupsog_gfx,&text_startwithpowerupsog);
	
	TPL_GetTexture(&spriteTPL, hs_plus_gfx,&hs_plus);
	TPL_GetTexture(&spriteTPL, hs_minus_gfx,&hs_minus);
	TPL_GetTexture(&spriteTPL, hs_is_gfx,&hs_is);
	TPL_GetTexture(&spriteTPL, hs_multi_gfx,&hs_multi);
	
	TPL_GetTexture(&spriteTPL, hs_p1_gfx,&hs_p[0]);
	TPL_GetTexture(&spriteTPL, hs_p2_gfx,&hs_p[1]);
	TPL_GetTexture(&spriteTPL, hs_p3_gfx,&hs_p[2]);
	TPL_GetTexture(&spriteTPL, hs_p4_gfx,&hs_p[3]);

}

inline void drawMouse(Mouse *mouse){
	drawSpriteTex(mouse->posx,mouse->posy,16,16,&curgfx);
}

inline void drawhud(){
	drawSpriteTex(0,480-96,640,96,&hud);
}
inline void drawplayerstats(Player *player, int currentplayer){
	static double fadecount=0;
	
	drawSpriteTex(16+(160*(player->number-1)),400,16,16,&icon_score);
	drawSpriteTex(16+(160*(player->number-1)),416,16,16,&icon_bomb);
	drawSpriteTex(16+(160*(player->number-1)),432,16,16,&icon_bombstrength);
	drawSpriteTex(16+(160*(player->number-1)),448,16,16,&icon_shift);
		
	drawnumber(player->score,32+(160*(player->number-1)),400);
	drawnumber(player->bombs,32+(160*(player->number-1)),416);
	drawnumber(player->bombstrength+1,32+(160*(player->number-1)),432);
	drawnumber(player->tileshifts,32+(160*(player->number-1)),448);
	
	fading_quad(0+(160*(currentplayer-1)), 384, 160, 96,sin(fadecount)*5+5);
	fadecount+=0.02;
}


inline int digit_to_int(char d){
 static char str[2]={0,'\0'};
 str[0] = d;
 
 return (int) strtol(str, NULL, 10);
}

inline void drawnumber(int number, int x, int y){
	char buffer[10];
	int chars=0, i=0;
	chars=sprintf(buffer,"%7d",number);
	for(i=0;i<chars;i++){
		drawSpriteTex(x+(i*16),y,16,16,&numbers[digit_to_int(buffer[i])]);
	}
	
}

inline void drawcursor(Cursor *cur){
	if(cur->selected_side<4)
		drawSpriteTex((cur->position->position_x*32)-1,(cur->position->position_y*32)-1,32+1,32+1,&overlay_sides[cur->selected_side]);
	else
		drawSpriteTex((cur->position->position_x*32)-1,(cur->position->position_y*32)-1,32+1,32+1,&overlay);
}


inline void draw_background(GXTexObj *backgroundelement){
	drawSpriteTex(0,0,640,480,backgroundelement);
}


inline void gfx_init(void){
	void *gp_fifo = NULL;
	GXColor background = {0, 0, 0, 0xff};
	f32 yscale;
	u32 xfbHeight;
	Mtx44 perspective;

	VIDEO_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);
	rmode=&TVPal264IntAa;
	  
	//rmode=&TVPal528Int;
	fb = 0;
	first_frame = 1;
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	fb ^= 1;

	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
 
	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);
 
	GX_SetCopyClear(background, 0x00ffffff);
 
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	if (rmode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);


	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(frameBuffer[fb],GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	
	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	GX_InvalidateTexAll();
	guOrtho(perspective,0,479,0,639,0.0,400.0);
	GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);
	
	

	setAnimationData.x=-1;
	setAnimationData.y=-1;
	setAnimationData.x_n=-1;
	setAnimationData.y_n=-1;
	
	setAnimationData.animationinprogress=0;
	setAnimationData.side=4;
	setAnimationData.side_n=4;
	setAnimationData.tick=0;
	
	
	shiftAnimationData.row=-1;
	shiftAnimationData.column=-1;
	shiftAnimationData.tick=0;
	shiftAnimationData.animationinprogress=0;
	shiftAnimationData.direction=0;
	
	bombAnimationData.x=-1;
	bombAnimationData.y=-1;
	bombAnimationData.tick=0;
	bombAnimationData.animationinprogress=0;
	bombAnimationData.length=0;
	
}


inline void drawgrid(Grid *gamegrid){
	static double fadeval=0;
	int i=0;
		
	for(i=0;i<gamegrid->tilecount;i++)
		drawSpriteTex(gamegrid->tiles[i].position_x*32,gamegrid->tiles[i].position_y*32,TILE_SIZE,TILE_SIZE,&gridtile);
		
	
	for(i=0;i<gamegrid->tilecount;i++){	
		int shiftoffsetx=0;
		int shiftoffsety=0;
		int direction=1;
		if(shiftAnimationData.direction)
			direction=-1;
		if(shiftAnimationData.row>=0 && shiftAnimationData.row==gamegrid->tiles[i].position_y && shiftAnimationData.animationinprogress){
			shiftoffsetx=(shiftAnimationData.tick*direction);
		}
		else if(shiftAnimationData.column>=0 && shiftAnimationData.column==gamegrid->tiles[i].position_x && shiftAnimationData.animationinprogress){
			shiftoffsety=(shiftAnimationData.tick*direction);
		}
		
		
		if(gamegrid->tiles[i].owned_by_Player!=0){					
				drawSpriteTex((shiftoffsetx+gamegrid->tiles[i].position_x*32),(shiftoffsety+gamegrid->tiles[i].position_y*32),TILE_SIZE,TILE_SIZE,&p_boxed[gamegrid->tiles[i].owned_by_Player->number]);
				fading_quad((shiftoffsetx+2+gamegrid->tiles[i].position_x*32),(shiftoffsety+2+gamegrid->tiles[i].position_y*32),TILE_SIZE-2,TILE_SIZE-2,sin(fadeval+(i*0.71))*20+20);
		}
		else{
			if(gamegrid->tiles[i].item==1)
				drawSpriteTex((shiftoffsetx+gamegrid->tiles[i].position_x*32),(shiftoffsety+gamegrid->tiles[i].position_y*32),TILE_SIZE,TILE_SIZE,&icon_bomb);
			if(gamegrid->tiles[i].item==2)
				drawSpriteTex((shiftoffsetx+gamegrid->tiles[i].position_x*32),(shiftoffsety+gamegrid->tiles[i].position_y*32),TILE_SIZE,TILE_SIZE,&icon_bombstrength);
			if(gamegrid->tiles[i].item==3)
				drawSpriteTex((shiftoffsetx+gamegrid->tiles[i].position_x*32),(shiftoffsety+gamegrid->tiles[i].position_y*32),TILE_SIZE,TILE_SIZE,&icon_shift);
			int j=0;
			for(j=0;j<4;j++){
				if(shiftAnimationData.row>=0 && (shiftAnimationData.row==gamegrid->tiles[i].position_y-1 || shiftAnimationData.row==gamegrid->tiles[i].position_y+1) && shiftAnimationData.animationinprogress){
					if((j==2 && shiftAnimationData.row==gamegrid->tiles[i].position_y+1) || (j==0 && shiftAnimationData.row==gamegrid->tiles[i].position_y-1))
						shiftoffsetx=shiftAnimationData.tick*direction;
					else
						shiftoffsetx=0;
				}
				else if(shiftAnimationData.column>=0 && (shiftAnimationData.column==gamegrid->tiles[i].position_x-1 || shiftAnimationData.column==gamegrid->tiles[i].position_x+1) && shiftAnimationData.animationinprogress){
				
					if((j==3 && shiftAnimationData.column==gamegrid->tiles[i].position_x-1) || (j==1 && shiftAnimationData.column==gamegrid->tiles[i].position_x+1))
							shiftoffsety=shiftAnimationData.tick*direction;
						else
							shiftoffsety=0;
				}
				if(gamegrid->tiles[i].sides_set[j]==1){
					if(setAnimationData.x!=gamegrid->tiles[i].position_x || setAnimationData.y!=gamegrid->tiles[i].position_y || setAnimationData.side!=j)
						if(setAnimationData.x_n!=gamegrid->tiles[i].position_x || setAnimationData.y_n!=gamegrid->tiles[i].position_y || setAnimationData.side_n!=j)
							drawSpriteTex((shiftoffsetx+gamegrid->tiles[i].position_x*32),(shiftoffsety+gamegrid->tiles[i].position_y*32),TILE_SIZE,TILE_SIZE,&side_tiles[j]);
				}	
			}			
		}			
	}
	drawSpriteTex(0,480-96,640,96,&hud);
	fadeval+=0.02;
}

inline void animation_setside(int side,int x,int y){
	setAnimationData.x=x;
	setAnimationData.y=y;
	setAnimationData.side=side;
	setAnimationData.animationinprogress=1;
	setAnimationData.tick=16;
	if(side==1){
		setAnimationData.side_n=3;
		setAnimationData.x_n=setAnimationData.x+1;
		setAnimationData.y_n=setAnimationData.y;
	}
	if(side==3){
		setAnimationData.side_n=1;
		setAnimationData.x_n=setAnimationData.x-1;
		setAnimationData.y_n=setAnimationData.y;
	}
	if(side==0){
		setAnimationData.side_n=2;
		setAnimationData.x_n=setAnimationData.x;
		setAnimationData.y_n=setAnimationData.y-1;
	}
	if(side==2){
		setAnimationData.side_n=0;
		setAnimationData.x_n=setAnimationData.x;
		setAnimationData.y_n=setAnimationData.y+1;
	}
}




inline void animation_shiftrow(int row,int direction){
	shiftAnimationData.row=row;
	shiftAnimationData.direction=direction;
	shiftAnimationData.column=-1;
	shiftAnimationData.tick=32;
	shiftAnimationData.animationinprogress=1;
}
inline void animation_shiftcolumn(int column,int direction){
	shiftAnimationData.column=column;
	shiftAnimationData.direction=direction;
	shiftAnimationData.row=-1;
	shiftAnimationData.tick=32;
	shiftAnimationData.animationinprogress=1;
}
inline void animation_bomb(int x,int y, int length){
	bombAnimationData.x=x;
	bombAnimationData.y=y;
	bombAnimationData.length=length;
	bombAnimationData.tick=5*2;
	bombAnimationData.animationinprogress=1;
}


inline int checkanimations(){
	if(setAnimationData.animationinprogress || shiftAnimationData.animationinprogress || bombAnimationData.animationinprogress){
		return 1;
	}
	else
		return 0;
}

inline void animate_settile(){
	if(setAnimationData.tick >0){
		int x=setAnimationData.x*TILE_SIZE;
		int y=setAnimationData.y*TILE_SIZE;
		int side=setAnimationData.side;
		int xn=setAnimationData.x_n*TILE_SIZE;
		int yn=setAnimationData.y_n*TILE_SIZE;
		int siden=setAnimationData.side_n;
		if(side%2!=0){
			drawSpriteTex(x,y+(setAnimationData.tick),TILE_SIZE,TILE_SIZE-(setAnimationData.tick)*2,&side_tiles[side]);
			drawSpriteTex(xn,yn+(setAnimationData.tick),TILE_SIZE,TILE_SIZE-(setAnimationData.tick)*2,&side_tiles[siden]);
				
		}
		else{
			drawSpriteTex(x+(setAnimationData.tick),y,TILE_SIZE-(setAnimationData.tick)*2,TILE_SIZE,&side_tiles[side]);
			drawSpriteTex(xn+(setAnimationData.tick),yn,TILE_SIZE-(setAnimationData.tick)*2,TILE_SIZE,&side_tiles[siden]);
		}
		setAnimationData.tick-=2;
	}
	else{
		setAnimationData.animationinprogress=0;
		setAnimationData.x=-1;
		setAnimationData.y=-1;
		setAnimationData.x_n=-1;
		setAnimationData.y_n=-1;
		setAnimationData.side=4;
		setAnimationData.side_n=4;
	}
		
}




inline void animate_shift(){
	if(shiftAnimationData.tick >0){
		shiftAnimationData.tick-=4;
	}
	else{
		shiftAnimationData.row=-1;
		shiftAnimationData.column=-1;
		shiftAnimationData.tick=0;
		shiftAnimationData.animationinprogress=0;
		shiftAnimationData.direction=0;
	}
}

inline void animate_bomb(){
	if(bombAnimationData.tick >0){
		int frame=5-(bombAnimationData.tick/2);
		drawAnimFrame(bombAnimationData.x*TILE_SIZE, bombAnimationData.y*TILE_SIZE, 32, 32, &bomb_middle,5,frame-1);
		int i;
		for(i=1;i<=bombAnimationData.length;i++){
			drawAnimFrame(bombAnimationData.x*TILE_SIZE, (bombAnimationData.y+i)*TILE_SIZE, 32, 32, &bomb_bottom_m,5,frame-1);
			drawAnimFrame(bombAnimationData.x*TILE_SIZE, (bombAnimationData.y-i)*TILE_SIZE, 32, 32, &bomb_top_m,5,frame-1);
			drawAnimFrame((bombAnimationData.x+i)*TILE_SIZE, (bombAnimationData.y  )*TILE_SIZE, 32, 32, &bomb_right_m,5,frame-1);
			drawAnimFrame((bombAnimationData.x-i)*TILE_SIZE, (bombAnimationData.y  )*TILE_SIZE, 32, 32, &bomb_left_m,5,frame-1);
		}
		drawAnimFrame(bombAnimationData.x*TILE_SIZE, (bombAnimationData.y+i)*TILE_SIZE, 32, 32, &bomb_bottom_e,5,frame-1);
		drawAnimFrame(bombAnimationData.x*TILE_SIZE, (bombAnimationData.y-i)*TILE_SIZE, 32, 32, &bomb_top_e,5,frame-1);
		drawAnimFrame((bombAnimationData.x+i)*TILE_SIZE, (bombAnimationData.y  )*TILE_SIZE, 32, 32, &bomb_right_e,5,frame-1);
		drawAnimFrame((bombAnimationData.x-i)*TILE_SIZE, (bombAnimationData.y  )*TILE_SIZE, 32, 32, &bomb_left_e,5,frame-1);		
		bombAnimationData.tick--;
	}
	else{
		bombAnimationData.x=-1;
		bombAnimationData.y=-1;
		bombAnimationData.tick=0;
		bombAnimationData.animationinprogress=0;
		bombAnimationData.length=0;
	}
}



inline void animate(){
	animate_settile();
	animate_shift();
	animate_bomb();
}
