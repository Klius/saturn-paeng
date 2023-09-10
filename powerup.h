/*
*
* POWERUPS to make
* confusion(inverted controls)
* Max/min Ball
* Max/min player
* speed player
* wall
*/

typedef struct t_powerup {
	int id;
	int type;
	int ttl;
	int x;
	int y;
	int w;
    int h;
    int state;
} Powerup;

int powerup_sprites[2];
int powerup_animation[2];
int MAX_TTL = 50000;//in_seconds

void powerup_init(void){
    //Load sprites
    powerup_sprites[BIG_BALL] = jo_sprite_add_image_pack("PMAXBALL", "PMB.TEX", JO_COLOR_Black);
    powerup_sprites[TINY_BALL] = jo_sprite_add_image_pack("PMINBALL", "PMIN.TEX", JO_COLOR_Black);
    //Load animations
    powerup_animation[BIG_BALL] = jo_create_sprite_anim(powerup_sprites[BIG_BALL], 5, 5);
    powerup_animation[TINY_BALL] = jo_create_sprite_anim(powerup_sprites[TINY_BALL], 5, 5);
}

Powerup powerup_spawn(void){
    Powerup pow = {0,BIG_BALL,MAX_TTL,rand()%JO_TV_WIDTH,rand()%JO_TV_HEIGHT,16,16,STANDBY};
    jo_start_sprite_anim_loop(powerup_animation[BIG_BALL]);
    return pow;
}

void powerup_update(Powerup* pow){
    //float delta = 0.1 * delta_time;
    //TODO fix this crap
    pow->ttl -= 0.1//delta_time;
}



void powerup_draw(Powerup* pow){
    jo_sprite_draw3D2(jo_get_anim_sprite(powerup_animation[pow->type]), pow->x, pow->y,600);
}
void powerup_collision(Powerup* pow, Ball* ball){
    if((pow->state == STANDBY) && (ball->x < pow->x+pow->w) && 
       (pow->x < ball->x+ball->w) && (ball->y < pow->y+pow->h) && (pow->y < ball->y+ball->w)){
	    pow->state = ACTIVE;
        pow->ttl = MAX_TTL; //TODO set right amount per type
    }
}
