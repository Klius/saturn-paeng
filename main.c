/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include "enums.h"
#include "ball.h"
#include "paddle.h"
#include "powerup.h"


static jo_font      *my_font;
static jo_sound     chime;
static int INPUT_TIMEOUT = 10;
Paddle p1 = PADDLE_P1_DEFAULT;
Paddle p2 = PADDLE_P2_DEFAULT;
Ball ball = T_BALL_DEFAULT;
static int sprite_ball;
int score[] = {0,0};
int scoreLimit = 10;
short winner = 0;
//Audio
bool is_cd_playing = 0;
//main menu
typedef enum {MULTIPLAYER,MEN_OPTIONS} MAIN_OP;
MAIN_OP currentMenuOption = MULTIPLAYER;

#define MAX_POWERUPS 3
Powerup powerup_pool[MAX_POWERUPS];

int cursorPos[2][4] = {
	{98,110,208,120},
	{120,142,188,150}
};
int selSprite=0;
//STATES
typedef enum {MAIN, GAME, OPTIONS} GAME_STATE;
GAME_STATE currentState = MAIN;
bool DEBUG = TRUE; 

int MOD_ANIMATION;
/******************************************************
***
***		CODE
***
*******************************************************/
void change_background(char* background){
	//Background
    jo_img      bg;
    bg.data = NULL;
    jo_tga_loader(&bg, "BG", background, JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);	
}
void print_debug(void){
	//jo_clear_screen(); FIXME breaks all lines
	jo_printf(0, 3, "Sprite memory usage: %d%%", jo_sprite_usage_percent()); 
	jo_printf(0, 4, "Dynamic memory usage: %d%%", jo_memory_usage_percent());
	if (currentState == GAME){
		jo_printf(0,0,"bx: %d by:%d",ball.x,ball.y);
		jo_printf(0,1,"p1hit: %d",p1.hit);
		jo_printf(0,2, "p2hit:%d",p2.hit);
		jo_printf(0,5, "p1y:%d  TVHEIGHT:%d",p1.y+p1.h+p1.vel,JO_TV_HEIGHT);
		for (int i =0;i<MAX_POWERUPS;i++){
			jo_printf(0,6+i, "POW %d X:%d ttl:%d State:%d",i,powerup_pool[i].x,powerup_pool[i].ttl,powerup_pool[i].state);
		}
		jo_printf(0,MAX_POWERUPS+1, "delta_time: %d, time_in_seconds:%d",delta_time,time_in_seconds);
		
	}
	else if (currentState == MAIN){
		jo_printf(0,7, "currentMenuOption:%d",currentMenuOption);
		jo_printf(0,9, "cursor1 X:%d Y:%d",cursorPos[currentMenuOption][0],cursorPos[currentMenuOption][1]);
		jo_printf(0,10, "cursor2 X:%d Y:%d",cursorPos[currentMenuOption][2],cursorPos[currentMenuOption][3]);
	}
}
void check_score(void){
	for(int i=0;i<2;i++){
	if (score[i] == scoreLimit){
			winner = i+1;
		}
	}
}
void game_draw(void){
	for (int i=0;i<3;i++){
		if ((powerup_pool[i].ttl <= 0) || (powerup_pool[i].state != STANDBY))
				continue;
		powerup_draw(&powerup_pool[i]);
	}
	jo_font_printf(my_font, JO_TV_WIDTH/2-80, 20, 4.0f, "%d",score[0]);
	jo_font_printf(my_font, JO_TV_WIDTH/2+60, 20, 4.0f, "%d",score[1]);
	if (winner > 0){
		jo_font_printf(my_font, 30, JO_TV_HEIGHT/2-20, 2.0f, "PLAYER %d WINS",winner);
	}
	paddle_draw(&p1);
	paddle_draw(&p2);
	ball_draw(&ball);
	
}
void main_menu_draw(void){
	jo_font_printf(my_font, JO_TV_WIDTH/4, 40, 4.0f, "PAENG");
	jo_font_printf(my_font,JO_TV_WIDTH/2 -JO_TV_WIDTH/6,120,1.0f,"MULTIPLAYER");
	jo_font_printf(my_font,JO_TV_WIDTH/2 -JO_TV_WIDTH/10,150,1.0f,"OPTIONS");
	jo_sprite_draw3D2(selSprite,cursorPos[currentMenuOption][0],cursorPos[currentMenuOption][1],500);
	jo_sprite_enable_horizontal_flip ();
	jo_sprite_enable_vertical_flip();
	jo_sprite_draw3D2(selSprite,cursorPos[currentMenuOption][2],cursorPos[currentMenuOption][3],500);
	jo_sprite_disable_horizontal_flip ();
	jo_sprite_disable_vertical_flip();
}
void my_draw(void)
{
	if (currentState == GAME){
		game_draw();
	}else if (currentState == MAIN){
		main_menu_draw();
	}
	if (DEBUG)
		print_debug();

}
void reset(void){
	ball.speed = 3;
	ball.velx = 3;
	ball.vely = 0;
	score[0] = 0,score[1]=0;
	winner = 0;
	ball.x = JO_TV_WIDTH/2 -8;
	ball.y = JO_TV_HEIGHT/2 -8;
	p1.y = JO_TV_HEIGHT/2-31;
	p2.y = JO_TV_HEIGHT/2-31;
}
void game_input(void){

		if (jo_is_pad1_available()){
			if (jo_is_pad1_key_pressed(JO_KEY_UP))
				p1.move = PADDLE_MOVE_UP;
			else if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
				p1.move = PADDLE_MOVE_DOWN;
			else
				p1.move = PADDLE_MOVE_NONE;
			if (jo_is_pad1_key_pressed(JO_KEY_START) && (winner > 0 ) )
				reset();

			if (jo_is_pad1_key_pressed(JO_KEY_A))
				powerup_pool[0] = powerup_spawn();
			if (jo_is_pad1_key_pressed(JO_KEY_B))
				DEBUG = !DEBUG;
		}
		if (jo_is_pad2_available()){
			if (jo_is_pad2_key_pressed(JO_KEY_UP))
				p2.move = PADDLE_MOVE_UP;
			else if (jo_is_pad2_key_pressed(JO_KEY_DOWN))
				p2.move = PADDLE_MOVE_DOWN;
			else
				p2.move = PADDLE_MOVE_NONE;
			if (jo_is_pad2_key_pressed(JO_KEY_START) && (winner > 0 ) )
				reset();
			else if (jo_is_pad2_key_pressed(JO_KEY_START) )
				jo_core_suspend();
			if (jo_is_pad2_key_pressed(JO_KEY_A))
				//spawn_modifier();
			if (jo_is_pad2_key_pressed(JO_KEY_B))
				DEBUG = !DEBUG;
		}
}
void read_input(void)
{
	if(currentState == GAME){
		game_input();
	}
	if(currentState == MAIN){
		if (!jo_is_pad1_available())
			return ;
		if(p1.timeout < 0){
			if (jo_is_pad1_key_pressed(JO_KEY_A)){
				if(currentMenuOption == MULTIPLAYER){
					currentState = GAME;
				}
			}
			if (jo_is_pad1_key_pressed(JO_KEY_C)){
				if (jo_is_pad1_key_pressed(JO_KEY_UP))
					cursorPos[currentMenuOption][1]--;

				if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
					cursorPos[currentMenuOption][1]++;

				if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
					cursorPos[currentMenuOption][0]--;

				if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
					cursorPos[currentMenuOption][0]++;

				return;
			}
			if (jo_is_pad1_key_pressed(JO_KEY_B)){
				if (jo_is_pad1_key_pressed(JO_KEY_UP))
					cursorPos[currentMenuOption][3]--;
				if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
					cursorPos[currentMenuOption][3]++;
				if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
					cursorPos[currentMenuOption][2]--;
				if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
					cursorPos[currentMenuOption][2]++;

				return;
				
			}
			if (jo_is_pad1_key_pressed(JO_KEY_UP)){
				currentMenuOption--;
				p1.timeout = INPUT_TIMEOUT;
				if (currentMenuOption == -1)
					currentMenuOption = 1;
			}
			if (jo_is_pad1_key_pressed(JO_KEY_DOWN)){
				currentMenuOption++;
				p1.timeout = INPUT_TIMEOUT;
				if (currentMenuOption > 1)
					currentMenuOption = 0;
			}
			
		}else{
			p1.timeout--;
		}
	}
}
int apply_powerup(int type){
	if (type == BIG_BALL || type == TINY_BALL)
		ball_powerup(type,&ball);
	if (type == CONFUSION){
		if (ball.velx >= 0){
			paddle_powerup(type,&p1);
			return P1;
		}
		else{
			paddle_powerup(type,&p2);
			return P2;
		}
	}
	return BALL;
}
void apply_powerdown(int type,int who){
	if (who == BALL)
		ball_powerdown(type,&ball);
	if (who == P1)
		paddle_powerdown(type,&p1);
	if (who == P2)
		paddle_powerdown(type,&p2);	
}
void update_game(void){
	if (winner == 0){
		move_paddle(&p1);
		move_paddle(&p2);
		if(p1.hit == 0){
			p1.hit = ball_collision(p1.x,p1.y,p1.w,p1.h,&ball);
			if (p1.hit==10)
				jo_audio_play_sound(&chime);
		}
		else{
			--p1.hit;
		}if(p2.hit == 0 ){
			p2.hit = ball_collision(p2.x,p2.y,p2.w,p2.h,&ball);
				if (p2.hit==10)
					jo_audio_play_sound(&chime);
		}else{
			--p2.hit;
		}
		for(int i = 0; i<MAX_POWERUPS;i++){
			if ((powerup_pool[i].state == DEAD)&&(powerup_pool[i].ttl < 0) && (rand()%10>8) )
			{
				powerup_pool[i] = powerup_spawn();
			}
			powerup_update(&powerup_pool[i]);
			powerup_collision(&powerup_pool[i],&ball);
			if (powerup_pool[i].state == ACTIVE){
				int who = apply_powerup(powerup_pool[i].type);
				powerup_pool[i].state = APPLIED;
				powerup_pool[i].who = who;
			}
			else if (powerup_pool[i].state == APPLIED && powerup_pool[i].duration<0){
				apply_powerdown(powerup_pool[i].type,powerup_pool[i].who);
				powerup_pool[i].state = DEAD;
			}
			else if(powerup_pool[i].state == STANDBY && powerup_pool[i].ttl <0){
				powerup_pool[i].state = DEAD;
			}
		}
		ball_move(&ball, score);
		check_score();
	}

}
void update(void){
	jo_fixed_point_time();
	read_input();
	if (currentState == GAME){
		if (!is_cd_playing)
    	{
        	/* the first track is reserved for the game binary so the first track is 2 */
        	jo_audio_play_cd_track(2, 2, 1);
        	is_cd_playing = 1;
    	}
		update_game();
	}
}
void load_audio(void){
	jo_audio_load_pcm("CHIME.PCM",JoSoundMono16Bit, &chime);
}

void jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	p1.sprite = jo_sprite_add_tga("TEX", "P1.TGA", JO_COLOR_Transparent);
	p2.sprite = jo_sprite_add_tga("TEX", "P2.TGA", JO_COLOR_Transparent);
	selSprite = jo_sprite_add_tga("TEX","SEL.TGA",JO_COLOR_Green);
	sprite_ball = jo_sprite_add_tga("TEX","BALL.TGA",JO_COLOR_Black);
	ball.sprite = sprite_ball;
	powerup_init();
	for (int i = 0; i< MAX_POWERUPS;i++){
		powerup_pool[i] = powerup_spawn();
	}
	
	
	change_background("BAK.TGA");
	load_audio();
	
	my_font = jo_font_load(JO_ROOT_DIR, "FONT.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
	jo_core_add_callback(my_draw);
	jo_core_add_callback(update);
	jo_core_run();
}

/*
** END OF FILE
*/
