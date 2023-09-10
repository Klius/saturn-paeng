

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
    if (player->move==PADDLE_MOVE_UP && player->y-player->vel > 0)
			player->y -= player->vel;
	if (player->move==PADDLE_MOVE_DOWN && player->y+player->h+player->vel < JO_TV_HEIGHT)
			player->y += player->vel;
}

void paddle_draw(Paddle* player){
    jo_sprite_draw3D2(player->sprite, player->x, player->y, 500);
}

/*void apply_powerup(int power_type){
    if (power_type == BIG_BALL)
        powerups[BIG_BALL] = 5;
}*/
