#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <ogc/tpl.h>

#include "../include/maingame.h"
#include "../include/oggplayer.h"
#include "../include/controls.h"
#include "../include/game_logic.h"

#include "../include/gfx.h"
#include "../include/sound.h"
#include "../include/interface.h"

void show_logo(){
double logoshow=0;
	SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)introsound_raw, introsound_raw_size, 170, 170, NULL);
	for(logoshow=0;logoshow<100;logoshow+=0.5){
		GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,0);
		GX_InvVtxCache();
		GX_InvalidateTexAll();
		GX_ClearVtxDesc();
		GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
		GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		guMtxIdentity(GXmodelView2D);
		guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, 0.0F);
		GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
		double logoshow2=logoshow;
		if(logoshow==13.5)
			SND_SetVoice(1, VOICE_STEREO_16BIT, 44100, 0, (void*)boxed_raw, boxed_raw_size, 170, 170, NULL);
			
		drawSpriteTex((640/2)-((logoshow2/100)*(640)),(480/2)-((logoshow2/100)*(480)),(logoshow2/100)*640*2,(logoshow2/100)*480*2,&squarewave);
		fading_quad((logoshow/30)*640,0, 640, 480, 100);
		drawSpriteTex((640/2)-((logoshow2/100)*(640)),(480/2)-((logoshow2/100)*(480)),(logoshow2/100)*640*2,(logoshow2/100)*480*2,&title_screen1);
		
		fading_quad( 0, 0, 640, 480, logoshow);

		GX_DrawDone();
		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
		GX_SetAlphaUpdate(GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		if(first_frame) {
			VIDEO_SetBlack(FALSE);
			first_frame = 0;
		}
		VIDEO_Flush();
		VIDEO_WaitVSync();
		fb ^= 1;
	}	
}

Game_Results game_main(Grid *gamegrid, Init_Struct init_vals){
	int i=0;
	int fadeval=100;
	
	Gameglobals game_globals=setup_gameglobals();
	
	Player players[4];
	players[0]=init_player(1);
	players[1]=init_player(2);
	players[2]=init_player(3);
	players[3]=init_player(4);
	game_globals.currentplayer=&players[0];
	
	Controller controllers[5];
	for(i=0;i<5;i++)
		initController(&controllers[i],i);
		
	Cursor cur1=init_cursor(&players[0],&gamegrid->tiles[0]);

	for(i=0;i<4;i++){
		if(init_vals.playercontrollers[i]<5){
			players[i].controllernumber=init_vals.playercontrollers[i];
			register_player(&game_globals, &players[i],0);
		}
		
	}
	for(i=0;i<4;i++){
		players[i].bombs=init_vals.extras_per_player[0];
		players[i].bombstrength=init_vals.extras_per_player[1];
		players[i].tileshifts=init_vals.extras_per_player[2];
	}	
	
	Mouse mouse;
	initMouse(&mouse);
	

	
	while(1) {
		if(fadeval>0)
			fadeval--;

		/*		Logik	*/
		
		/*Wenn Player Controllernummer 4 hat, ist die AI dran*/
		if(game_globals.currentplayer->controllernumber==4 && !ai_movelineactive)
			ai_main(gamegrid, game_globals.currentplayer);
		
		/*Animationen prüfen. Wenn keine Animation: Inputs pollen. Rumble für alle Controller überprüfen. Aktionen prüfen. Spielfeld prüfen.*/	
		int animation_in_progress=checkanimations();
		if(!animation_in_progress){
			controlInputs(gamegrid,&mouse,&cur1,&controllers[game_globals.currentplayer->controllernumber]);
			for(i=0;i<4;i++)
				checkrumble(&controllers[i]);
			
			checkactions(gamegrid, &cur1, &game_globals);
			checkowned(&game_globals, gamegrid);
			
		}
		
		/*Naechsten Spieler rumbeln, wenn er dran ist.*/
		if(cycleplayers(&game_globals,0)){
			controllers[game_globals.currentplayer->controllernumber].rumbletime+=10;
		}
		
		/*		Logikende	*/
		

		/*		Zeichnen									*/
		guMtxIdentity(GXmodelView2D);
		GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
		
		draw_background(&backgroundelement);
		draw_fog();
		drawgrid(gamegrid);
		animate();
		drawcursor(&cur1);
		drawMouse(&mouse);
		for(i=0;i<4;i++)
			if(game_globals.players[i]!=0)
				drawplayerstats(game_globals.players[i],game_globals.currentplayer->number);		
		fading_quad( 0, 0, 640, 480, fadeval);
		
		GX_DrawDone();
		/*		Zeichnen Ende										*/
		
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);
		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		if(first_frame) {
			VIDEO_SetBlack(FALSE);
			first_frame = 0;
		}
		VIDEO_Flush();
		VIDEO_WaitVSync();
		fb ^= 1;		/* flip framebuffer */
		
		if(game_globals.game_ended){
			
			/*Spielfeld-Fadeout*/
			int fadeout=0;
			for(fadeout=0;fadeout<100;fadeout++){
				guMtxIdentity(GXmodelView2D);
				GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
				draw_background(&backgroundelement);
				draw_fog();
				drawgrid(gamegrid);
				animate();
				drawcursor(&cur1);
				drawMouse(&mouse);
				for(i=0;i<4;i++)
					if(game_globals.players[i]!=0)
						drawplayerstats(game_globals.players[i],game_globals.currentplayer->number);
				fading_quad( 0, 0, 640, 480, fadeout);
				SetVolumeOgg(81-(fadeout-20));
				
				GX_DrawDone();
				GX_CopyDisp(frameBuffer[fb],GX_TRUE);

				VIDEO_SetNextFramebuffer(frameBuffer[fb]);
				if(first_frame) {
					VIDEO_SetBlack(FALSE);
					first_frame = 0;
				}
				VIDEO_Flush();
				VIDEO_WaitVSync();
				fb ^= 1;			
			}
			Game_Results gres;
			for(i=0;i<4;i++){
				if(game_globals.players[i]){
						gres.score[i]=game_globals.players[i]->score;
						gres.bombs[i]=game_globals.players[i]->bombs;
						gres.shifts[i]=game_globals.players[i]->tileshifts;
						gres.bombstrength[i]=game_globals.players[i]->bombstrength;
						gres.players_valid[i]=1;
				}
				else{
					gres.players_valid[i]=0;
					gres.score[i]=0;
				}
			}
			/*	statics resetten	*/
			cycleplayers(&game_globals,1);
			register_player(&game_globals,(Player*)0,1);
			/*	end statics resetten	*/
			return gres;
		}		
	}
}