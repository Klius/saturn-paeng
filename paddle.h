

typedef struct t_paddle{
int x;
int y;
int w;
int h;
int vel;
int sprite;
int hit;
int timeout;
//int powerups[6];
paddle_movement move;
} Paddle;

const Paddle PADDLE_P1_DEFAULT = {30,JO_TV_HEIGHT/2-31,8,62,2,0,0,0,PADDLE_MOVE_NONE};
const Paddle PADDLE_P2_DEFAULT = {JO_TV_WIDTH - 38,JO_TV_HEIGHT/2-31,8,62,2,0,0,0,PADDLE_MOVE_NONE};

void move_paddle(Paddle* player){
    int move = (int)player->vel;// TODO revise this formula //*(delta_time*0.1)
    if (player->move==PADDLE_MOVE_UP && player->y-move > 0)
			player->y -= move;
	if (player->move==PADDLE_MOVE_DOWN && player->y+player->h+move < JO_TV_HEIGHT)
			player->y += move;
}

void paddle_draw(Paddle* player){
    jo_sprite_draw3D2(player->sprite, player->x, player->y, 500);
}

/*void apply_powerup(int power_type){
    if (power_type == BIG_BALL)
        powerups[BIG_BALL] = 5;
}*/
