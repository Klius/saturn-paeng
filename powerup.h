/*
*
* POWERUPS to make
* confusion(inverted controls)
* Max/min Ball
* Max/min player
* speed player
* wall
*/
#include <stdlib.h>
typedef struct t_powerup {
	int id;
	int type;
	int ttl;
	int x;
	int y;
	int w;
    int h;
    int state;
    int duration;
    int who;
} Powerup;

int powerup_sprites[6];
int powerup_animation[6];
int MAX_TTL = 5000;//in_seconds

void powerup_init(void){
    //Load sprites
    powerup_sprites[BIG_BALL] = jo_sprite_add_image_pack("PMAXBALL", "PMB.TEX", JO_COLOR_Black);
    powerup_sprites[TINY_BALL] = jo_sprite_add_image_pack("PMINBALL", "PMIN.TEX", JO_COLOR_Black);
    powerup_sprites[CONFUSION] = jo_sprite_add_image_pack("CONF", "PCONF.TEX", JO_COLOR_Black);
    powerup_sprites[LONG_PAD] = jo_sprite_add_image_pack("PLAC", "PLACE.TEX", JO_COLOR_Black);
    powerup_sprites[SHORT_PAD] = jo_sprite_add_image_pack("PLAC", "PLACE.TEX", JO_COLOR_Black);
    //Load animations
    powerup_animation[BIG_BALL] = jo_create_sprite_anim(powerup_sprites[BIG_BALL], 5, 5);
    powerup_animation[TINY_BALL] = jo_create_sprite_anim(powerup_sprites[TINY_BALL], 5, 5);
    powerup_animation[CONFUSION] = jo_create_sprite_anim(powerup_sprites[CONFUSION], 5, 5);
    powerup_animation[LONG_PAD] = jo_create_sprite_anim(powerup_sprites[LONG_PAD], 5, 5);
    powerup_animation[SHORT_PAD] = jo_create_sprite_anim(powerup_sprites[SHORT_PAD], 5, 5);
}

int powerup_selector(void){
    int r = rand()%100;
    int pow_select = BIG_BALL;
    if ( r < 25){
        pow_select = TINY_BALL;
    }
    else if(r>25 && r<50){
        pow_select = CONFUSION;
    }
    else if(r>50 && r<60){
        pow_select = LONG_PAD;
    }
    else if(r>60 && r<80){
        pow_select = SHORT_PAD;
    }
    else if(r>80){
        pow_select = LONG_PAD;
    }
    //pow_select = SHORT_PAD;
    return pow_select;
}

Powerup powerup_spawn(void){
    int type = powerup_selector();
    Powerup pow = {0,type,MAX_TTL,rand()%JO_TV_WIDTH,rand()%JO_TV_HEIGHT,16,16,STANDBY,5000,1};
    jo_start_sprite_anim_loop(powerup_animation[type]);
    return pow;
}

void powerup_update(Powerup* pow){
    int delta = delta_time/60;
    if (pow->state != ACTIVE)
        pow->ttl -= delta;
    if (pow->state == APPLIED)
        pow->duration -= delta;
}



void powerup_draw(Powerup* pow){
    jo_sprite_draw3D2(jo_get_anim_sprite(powerup_animation[pow->type]), pow->x, pow->y,600);
}
void powerup_collision(Powerup* pow, Ball* ball){
    if((pow->state == STANDBY) && (ball->x < pow->x+pow->w) && 
       (pow->x < ball->x+ball->w) && (ball->y < pow->y+pow->h) && (pow->y < ball->y+ball->w)){
	    pow->state = ACTIVE;
    }
}
