

typedef struct t_paddle{
int x;
int y;
int w;
int h;
int vel;
int sprites[3];
int hit;
int timeout;
//int powerups[6];
paddle_movement move;
int confusion;
int long_pad;
int short_pad;
} Paddle;

const Paddle PADDLE_P1_DEFAULT = {30,JO_TV_HEIGHT/2-31,8,62,2,0,0,0,PADDLE_MOVE_NONE,0,0,0};
const Paddle PADDLE_P2_DEFAULT = {JO_TV_WIDTH - 38,JO_TV_HEIGHT/2-31,8,62,2,0,0,0,PADDLE_MOVE_NONE,0,0,0};

void move_paddle(Paddle* player){
    int move = (int)player->vel;// TODO revise this formula //*(delta_time*0.1)
    //invert
    if (player->confusion >0 ){
        if (player->move==PADDLE_MOVE_UP)
            player->move=PADDLE_MOVE_DOWN;
        else if(player->move==PADDLE_MOVE_DOWN){
            player->move=PADDLE_MOVE_UP;
        }
    }
    //Move
    if (player->move==PADDLE_MOVE_UP ){
			player->y -= move;
        if(player->y <= 0)
            player->y = 0;
    }
	if (player->move==PADDLE_MOVE_DOWN ){
		player->y += move;
        if (player->y+player->h >= JO_TV_HEIGHT)
            player->y = JO_TV_HEIGHT-player->h;
    }
}

void paddle_draw(Paddle* player){
    int sp = 0;
    if(player->long_pad >0){
        sp= 1;
    }
    if(player->short_pad >0){
        sp=2;
    }
    if (player-> confusion >= 1){
        //TODO improve this so it doesn't look as jank
        if (player-> confusion %8 == 0){
            jo_set_gouraud_shading_colors(JO_COLOR_Black,JO_COLOR_Black,JO_COLOR_MediumWhite,JO_COLOR_MediumWhite);
        }
        else{
            jo_set_gouraud_shading_colors(JO_COLOR_MediumWhite,JO_COLOR_MediumWhite,JO_COLOR_Black,JO_COLOR_Black);
        }
        jo_sprite_enable_gouraud_shading();
        player-> confusion += 1;
    }
    jo_sprite_draw3D2(player->sprites[sp], player->x, player->y, 500);
    jo_sprite_disable_gouraud_shading();
}

void paddle_powerup(int type,Paddle* paddle){
    // Let's first check if the powerup has been applied already
	if (type == CONFUSION){
        paddle -> confusion = 1;
    }
    // FIXME WONKY as FUCK, collisions don't line up with sprite
    if (type == LONG_PAD){
		paddle->long_pad = 1;
		paddle->h = 124; 
		paddle->vel = 1;
	}
	if (type == SHORT_PAD){
		paddle->short_pad = 1;
		paddle->h = 32;
		paddle->vel = 5;
	}

}
void paddle_powerdown(int type, Paddle* paddle){
	if (type == CONFUSION){
        paddle -> confusion = 0;
    }
    if (type == LONG_PAD){
		paddle->h = 62;
		paddle->vel = 3;
		paddle->long_pad = 0;
	}
	if (type == SHORT_PAD){
		paddle->short_pad = 0;
		paddle->h = 62;
		paddle->vel = 3;
	}
}