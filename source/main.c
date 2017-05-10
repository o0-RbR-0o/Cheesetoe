#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <ogc/tpl.h>


#include "../include/oggplayer.h"
#include "../include/controls.h"
#include "../include/game_logic.h"
#include "../include/game_elements.h"
#include "../include/gfx.h"
#include "../include/sound.h"
#include "../include/interface.h"
#include "../include/maingame.h"


#include "demotrack_ogg.h"

Init_Struct menu();
void highscore(Game_Results gres);


int main( int argc, char **argv ){
	gfx_init();
	PAD_Init();
	ASND_Init();
	
	load_game_gfx();
	
	PlayOgg(demotrack_ogg, demotrack_ogg_size, 0, OGG_INFINITE_TIME);
	SetVolumeOgg(0);
	
	show_logo();
	
	Grid gamegrid;
	while(1){
		Init_Struct initvals=menu();
		SetVolumeOgg(80);
		init_grid(&gamegrid,20,12,initvals.extras_on_grid[0],initvals.extras_on_grid[1],initvals.extras_on_grid[2],GetTimeOgg());
		Game_Results gres=game_main(&gamegrid, initvals);
		SetVolumeOgg(0);
		highscore(gres);
	}
	
	return 0;
}


Init_Struct menu(){
	
	int menuposx=0;
	int controllernum[4];
	int i=0;
	int fader=100;
	char extraspp[3]={0,0,0};
	char extrasog[3]={0,0,0};
	for(i=0;i<4;i++)
		controllernum[i]=0;
	do{
		if(fader>0)
			fader--;
		PAD_ScanPads();		
		if(PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT){
			menuposx=(menuposx+1)%10;
			SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)menu1_raw, menu1_raw_size, 255, 255, NULL);
		}
		if(PAD_ButtonsDown(0) & PAD_BUTTON_LEFT){
			menuposx=(menuposx+9)%10;
			SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)menu1_raw, menu1_raw_size, 255, 255, NULL);
		}
			
		if(PAD_ButtonsDown(0) & PAD_BUTTON_UP){
			SND_SetVoice(3, VOICE_STEREO_16BIT, 44100, 0, (void*)woosh_raw, woosh_raw_size, 255, 255, NULL);
			if(menuposx<2)
				controllernum[menuposx]=(controllernum[menuposx]+1)%5;
			else if(menuposx<4)
				controllernum[menuposx]=(controllernum[menuposx]+1)%6;
			else if(menuposx<7){
				extraspp[menuposx-4]++;
			}
			else{
				extrasog[menuposx-7]=(extrasog[menuposx-7]+1)%51;
			}
		}
		if(PAD_ButtonsDown(0) & PAD_BUTTON_DOWN){
			SND_SetVoice(3, VOICE_STEREO_16BIT, 44100, 0, (void*)woosh_raw, woosh_raw_size, 255, 255, NULL);
			if(menuposx<2)
				controllernum[menuposx]=(controllernum[menuposx]+4)%5;
			else if(menuposx<4)
				controllernum[menuposx]=(controllernum[menuposx]+5)%6;
			else if(menuposx<7){
				extraspp[menuposx-4]--;
			}
			else{
				extrasog[menuposx-7]=(extrasog[menuposx-7]+50)%51;
			}
		}
			
			
		guMtxIdentity(GXmodelView2D);
		GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
		draw_background(&title_screen2);
		drawSpriteTex(0,0,640,64,&text_players);
		
		drawSpriteTex(0,64+128,640,64,&text_startwithpowerupspp);
		drawSpriteTex(0,256,640,64,&menu_extrasconfig);
		drawnumber(extraspp[0],32,256+25);
		drawnumber(extraspp[1]+1,32+214,256+25);
		drawnumber(extraspp[2],32+(214*2),256+25);
		
		drawSpriteTex(0,256+64,640,64,&text_startwithpowerupsog);
		drawSpriteTex(0,256+128,640,64,&menu_extrasconfig);
		drawnumber(extrasog[0],32,256+25+128);
		drawnumber(extrasog[1],32+214,256+25+128);
		drawnumber(extrasog[2],32+(214*2),256+25+128);
		
		for(i=0;i<4;i++){
			if(controllernum[i]<4){
				drawSpriteTex(16+((128+32)*i),64,128,128,&icon_controller);
				drawSpriteTex(16+((128+32)*i),64,128,128,&bignum[controllernum[i]]);
			
			}
			else if(controllernum[i]==4){
				drawSpriteTex(16+((128+32)*i),64,128,128,&icon_bot);
			}
			else if(controllernum[i]==5){
					drawSpriteTex(16+((128+32)*i),64,128,128,&icon_x);
			}
		}
		if(menuposx<4)
			drawSpriteTex(16+((128+32)*menuposx),64,128,128,&menucursor);
		else if(menuposx<7)
			drawSpriteTex(((214)*(menuposx-4)),256,214,64,&menucursor2);
		else
			drawSpriteTex(((214)*(menuposx-7)),256+128,214,64,&menucursor2);
			
		fading_quad( 0, 0, 640, 480, fader);
		
		GX_DrawDone();
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		if(first_frame) {
			VIDEO_SetBlack(FALSE);
			first_frame = 0;
		}
		VIDEO_Flush();
		VIDEO_WaitVSync();
		fb ^= 1;		// flip framebuffer

	}while(!(PAD_ButtonsDown(0) & PAD_BUTTON_A));
	
	SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)pause_raw, pause_raw_size, 255, 255, NULL);
	
	
	for(fader=0;fader<100;fader++){
		guMtxIdentity(GXmodelView2D);
		GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
		draw_background(&title_screen2);
		drawSpriteTex(0,0,640,64,&text_players);
		
		drawSpriteTex(0,64+128,640,64,&text_startwithpowerupspp);
		drawSpriteTex(0,256,640,64,&menu_extrasconfig);
		drawnumber(extraspp[0],32,256+25);
		drawnumber(extraspp[1]+1,32+214,256+25);
		drawnumber(extraspp[2],32+(214*2),256+25);
		
		drawSpriteTex(0,256+64,640,64,&text_startwithpowerupsog);
		drawSpriteTex(0,256+128,640,64,&menu_extrasconfig);
		drawnumber(extrasog[0],32,256+25+128);
		drawnumber(extrasog[1],32+214,256+25+128);
		drawnumber(extrasog[2],32+(214*2),256+25+128);
		
		for(i=0;i<4;i++){
			if(controllernum[i]<4){
				drawSpriteTex(16+((128+32)*i),64,128,128,&icon_controller);
				drawSpriteTex(16+((128+32)*i),64,128,128,&bignum[controllernum[i]]);
			
			}
			else if(controllernum[i]==4){
				drawSpriteTex(16+((128+32)*i),64,128,128,&icon_bot);
			}
			else if(controllernum[i]==5){
					drawSpriteTex(16+((128+32)*i),64,128,128,&icon_x);
			}
		}
		if(menuposx<4)
			drawSpriteTex(16+((128+32)*menuposx),64,128,128,&menucursor);
		else if(menuposx<7)
			drawSpriteTex(((214)*(menuposx-4)),256,214,64,&menucursor2);
		else
			drawSpriteTex(((214)*(menuposx-7)),256+128,214,64,&menucursor2);
			
		fading_quad( 0, 0, 640, 480, fader);
		
		GX_DrawDone();
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		if(first_frame) {
			VIDEO_SetBlack(FALSE);
			first_frame = 0;
		}
		VIDEO_Flush();
		VIDEO_WaitVSync();
		fb ^= 1;		// flip framebuffer
		
	
	}
	
	
	
	Init_Struct initvals;
	for(i=0;i<4;i++)
		initvals.playercontrollers[i]=controllernum[i];	
	for(i=0;i<3;i++){
		initvals.extras_per_player[i]=extraspp[i];
		initvals.extras_on_grid[i]=extrasog[i];
	}
		
	return initvals;

}

void highscore(Game_Results gres){
	int i=0;
	char done=0;
	double score[4];
	int lastscore=0;
	int offsety=100;
	unsigned int mainscore[4];
	for(i=0;i<4;i++){
		mainscore[i]=gres.score[i];
		if((mainscore[i]-(gres.bombs[i]*(gres.bombstrength[i]+1)))+gres.shifts[i]>0)
			gres.score[i]=(mainscore[i]-(gres.bombs[i]*(gres.bombstrength[i]+1)))+gres.shifts[i];
		else
			gres.score[i]=0;
	}
	
	i=0;
	while(!done){
		guMtxIdentity(GXmodelView2D);
		GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
		draw_background(&title_screen2);
		
		int j=0;
		for(j=0;j<4;j++){
			if(gres.players_valid[j]){
				drawnumber((int)mainscore[j],8+0,(64*j)+offsety+(32*j));
				drawnumber((int)gres.bombs[j],8+128,(64*j)+offsety+(32*j));
				drawnumber((int)gres.bombstrength[j]+1,8+256,(64*j)+offsety+(32*j));
				drawnumber((int)gres.shifts[j],8+384,(64*j)+offsety+(32*j));
				drawnumber((int)score[j],8+512,(64*j)+offsety+(32*j));
				
				drawSpriteTex(8+128-16,(64*j)+offsety+(32*j),16,16,&hs_minus);
				drawSpriteTex(8+256-16,(64*j)+offsety+(32*j),16,16,&hs_multi);
				drawSpriteTex(8+384-16,(64*j)+offsety+(32*j),16,16,&hs_plus);
				drawSpriteTex(8+512-16,(64*j)+offsety+(32*j),16,16,&hs_is);
				
				drawSpriteTex(8+0,(64*j)+(32*j)+offsety-32,32,32,&icon_score);
				drawSpriteTex(8+128,(64*j)+(32*j)+offsety-32,32,32,&icon_bomb);
				drawSpriteTex(8+256,(64*j)+(32*j)+offsety-32,32,32,&icon_bombstrength);
				drawSpriteTex(8+384,(64*j)+(32*j)+offsety-32,32,32,&icon_shift);
				
				
				drawSpriteTex(10+256,(64*j)+(32*j)+(offsety-64)+16,128,16,&hs_p[j]);
			}
		}
		
		
		
		
		if((int)score[i]>lastscore){
			SND_SetVoice(1+(int)score%4, VOICE_STEREO_16BIT, 44100, 0, (void*)boxed_raw, boxed_raw_size, 170, 170, NULL);
		}
		lastscore=(int)score[i];
		if(score[i]>gres.score[i]){
			i++;
			lastscore=0;
		while((i<=3)&&gres.players_valid[i]==0)
			i++;
		}
		else
			score[i]+=0.5;
		if(i>3)
			done=1;
		GX_DrawDone();
		GX_CopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
		if(first_frame) {
			VIDEO_SetBlack(FALSE);
			first_frame = 0;
		}
		VIDEO_Flush();
		VIDEO_WaitVSync();
		fb ^= 1;		// flip framebuffer
	}
	
	do{
		PAD_ScanPads();
	}
	while(!(PAD_ButtonsDown(0) & PAD_BUTTON_A));
	do{
		PAD_ScanPads();
	}
	while((PAD_ButtonsDown(0) & PAD_BUTTON_A));		
	SND_SetVoice(2, VOICE_STEREO_16BIT, 44100, 0, (void*)pause_raw, pause_raw_size, 255, 255, NULL);
	
	

	
	
	
	
	
	
}



