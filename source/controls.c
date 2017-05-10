#include <gccore.h>
#include "../include/controls.h"

#include "../include/interface.h"

void checkrumble(Controller *controller){
	if(controller->rumbletime){
		PAD_ControlMotor(controller->number, PAD_MOTOR_RUMBLE);
		controller->rumbletime--;
	}
	else{
		PAD_ControlMotor(controller->number, PAD_MOTOR_STOP_HARD);
	}
}



void controlCursor(Grid *gamegrid, Mouse *mouse, Cursor *cursor, Controller *controller){
	int i=0;
	for(i=0;i<gamegrid->tilecount;i++){
		if(	mouse->posx >=(gamegrid->tiles[i].position_x*32) &&
			mouse->posx <(gamegrid->tiles[i].position_x*32)+32 &&
			mouse->posy >=(gamegrid->tiles[i].position_y*32) &&
			mouse->posy <(gamegrid->tiles[i].position_y*32)+32
		){
			
			set_cursor_position(cursor, &gamegrid->tiles[i]);
		}
	}
	if(controller->a){
		if(cursor->is_movable)
			cursor->is_movable=0;
		else{
			cursor->is_movable=1;
			cursor->setsSide=0;
			cursor->shiftrow=0;
			cursor->shiftcolumn=0;
			cursor->bomb=0;
		}
	}
	cursor->setsSide=0;
	cursor->shiftrow=0;
	cursor->shiftcolumn=0;
	cursor->bomb=0;
	if(!cursor->is_movable){
		if(controller->up || controller->sticky>64)
			cursor->selected_side=0;
		else if(controller->right || controller->stickx>64)
			cursor->selected_side=1;
		else if(controller->down || controller->sticky<-64)
			cursor->selected_side=2;
		else if(controller->left || controller->stickx<-64)
			cursor->selected_side=3;
	}
	else{
		cursor->selected_side=4;
	}
	if(controller->b)
		cursor->setsSide=1;
	if(controller->x){
		if(cursor->selected_side==0)
			cursor->shiftcolumn=1;
		if(cursor->selected_side==1)
			cursor->shiftrow=2;
		if(cursor->selected_side==2)
			cursor->shiftcolumn=2;
		if(cursor->selected_side==3)
			cursor->shiftrow=1;
	}
	if(controller->y){
		cursor->bomb=1;
	}
}


void controlMouse(Mouse *mouse,Controller *controller){
	if(controller->right)
		mouse->posx+=4;
	if(controller->left)
		mouse->posx-=4;
	if(controller->down)
		mouse->posy+=4;
	if(controller->up)
		mouse->posy-=4;
	if(controller->stickx>8 || controller->stickx<-8)
		mouse->posx+=controller->stickx/8;
	if(controller->sticky>8 || controller->sticky<-8)
		mouse->posy-=controller->sticky/8;
	
		
	if(mouse->posx>640)
		mouse->posx=0;
	if(mouse->posx<0)
		mouse->posx=639;
	if(mouse->posy>480-96)
		mouse->posy=0;
	if(mouse->posy<0)
		mouse->posy=479-96;

}

void controlInputs(Grid *gamegrid, Mouse *mouse, Cursor *cursor, Controller *controller){
	if(controller->number!=4){
		cacheinputs(controller);	
	}
	else{
		do_ai_actions(mouse, controller);
	}
	
	if(cursor->is_movable){
		controlMouse(mouse,controller);	
	}
	controlCursor(gamegrid,mouse,cursor,controller);
}

void cacheinputs(Controller *controller){
	if(controller->number!=4){
		PAD_ScanPads();
		controller->up=PAD_ButtonsHeld(controller->number) & PAD_BUTTON_UP;
		controller->down=PAD_ButtonsHeld(controller->number) & PAD_BUTTON_DOWN;
		controller->left=PAD_ButtonsHeld(controller->number) & PAD_BUTTON_LEFT;
		controller->right=PAD_ButtonsHeld(controller->number) & PAD_BUTTON_RIGHT;
		controller->a=PAD_ButtonsDown(controller->number) & PAD_BUTTON_A;
		controller->b=PAD_ButtonsDown(controller->number) & PAD_BUTTON_B;
		controller->x=PAD_ButtonsDown(controller->number) & PAD_BUTTON_X;
		controller->y=PAD_ButtonsDown(controller->number) & PAD_BUTTON_Y;
		controller->start=PAD_ButtonsDown(controller->number) & PAD_BUTTON_START;
		controller->stickx=PAD_StickX(controller->number);
		controller->sticky=PAD_StickY(controller->number);
	}
}

void initController(Controller *controller,int number){
	controller->assigned_player=0;
	controller->number=number;
	controller->rumbletime=10;
	controller->left=0;
	controller->right=0;
	controller->up=0;
	controller->down=0;
	controller->a=0;
	controller->b=0;
	controller->x=0;
	controller->y=0;
	controller->start=0;
	controller->stickx=0;
	controller->sticky=0;
}