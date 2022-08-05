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
#include "ball.h"
#include "paddle.h"
/*
**
** Structures
**
*/
struct mod {
	int id;
	int type;
	int ttl;
	int x;
	int y;
	int w;
	int sprite;
	int animationId;
};
/*
*
* POWERUPS to make
* confusion(inverted controls)
* Max/min Ball
* Max/min player
* speed player
* wall
*/
int ring_anim_id=0;
int powSprites[2];
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
enum MAIN_OP {MULTIPLAYER,MEN_OPTIONS};
enum MAIN_OP currentMenuOption = MULTIPLAYER;


int cursorPos[2][4] = {
	{98,110,208,120},
	{120,142,188,150}
};
int selSprite=0;
//STATES
enum STATE {MAIN, GAME, OPTIONS};
enum STATE currentState = MAIN;
bool DEBUG = false; 
struct mod test,test2;

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
    jo_tga_loader(&bg, "BG", background, JO_COLOR_Black);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);	
}
void print_debug(void){
	jo_printf(0, 3, "Sprite memory usage: %d%% ", jo_sprite_usage_percent()); 
	jo_printf(0, 4, "Dynamic memory usage: %d%%  ", jo_memory_usage_percent());
	if (currentState == GAME){
		jo_printf(0,0,"bx: %d by:%d",ball.x,ball.y);
		jo_printf(0,1,"p1hit: %d",p1.hit);
		jo_printf(0,2, "p2hit:%d",p2.hit);
		jo_printf(0,5, "p1y:%d  TVHEIGHT:%d",p1.y+p1.h+p1.vel,JO_TV_HEIGHT);
	}
	if (currentState == MAIN){
		jo_printf(0,6, "currentMenuOption:%d ",currentMenuOption);
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
	jo_sprite_draw3D2(jo_get_anim_sprite(test.animationId), test.x, test.y,600);
	if (test2.id == 0)
		jo_sprite_draw3D2(jo_get_anim_sprite(test2.animationId), test2.x, test2.y,600);
	jo_font_printf(my_font, JO_TV_WIDTH/2-80, 20, 4.0f, "%d",score[0]);
	jo_font_printf(my_font, JO_TV_WIDTH/2+60, 20, 4.0f, "%d",score[1]);
	if (winner == 0){
		if(p1.hit == 0){
			p1.hit = ball_collision(p1.x,p1.y,p1.w,p1.h,&ball);
			if (p1.hit==10)
				jo_audio_play_sound(&chime);
		}else{
			--p1.hit;
		}if(p2.hit == 0 ){
			p2.hit = ball_collision(p2.x,p2.y,p2.w,p2.h,&ball);
				if (p2.hit==10)
					jo_audio_play_sound(&chime);
		}else{
			--p2.hit;
		}
		ball_move(&ball, score);
		check_score();
	}
	else{
		jo_font_printf(my_font, 30, JO_TV_HEIGHT/2-20, 2.0f, "PLAYER %d WINS",winner);
	}
	jo_sprite_draw3D2(p1.sprite, p1.x, p1.y, 500);
	jo_sprite_draw3D2(p2.sprite, p2.x, p2.y, 500);
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
		 if (!is_cd_playing)
    	{
        	/* the first track is reserved for the game binary so the first track is 2 */
        	jo_audio_play_cd_track(2, 2, 1);
        	is_cd_playing = 1;
    	}
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
//TODO work_it_good
void spawn_modifier(void){
	test2.id=0;
	test2.type=0;
	test2.ttl=10;
	test2.x=rand()%JO_TV_WIDTH;
	test2.y=JO_TV_HEIGHT/2;
	test2.w=0;
	test2.sprite=0;
	test2.animationId=MOD_ANIMATION;
	jo_start_sprite_anim_loop(test2.animationId);
}
void			my_gamepad(void)
{
	if(currentState == GAME){
		if (!jo_is_pad1_available())
			return ;
		if (jo_is_pad1_key_pressed(JO_KEY_UP) && p1.y-p1.vel > 0)
			p1.y -= p1.vel;
		if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && p1.y+p1.h+p1.vel < JO_TV_HEIGHT)
			p1.y += p1.vel;
		if (jo_is_pad1_key_pressed(JO_KEY_START) && (winner > 0 ) )
			reset();
		if (jo_is_pad1_key_pressed(JO_KEY_START) )
			jo_core_suspend();
		if (jo_is_pad1_key_pressed(JO_KEY_A))
			spawn_modifier();
		if (jo_is_pad1_key_pressed(JO_KEY_B))
			DEBUG = !DEBUG;
	/* 
		if (jo_is_pad1_key_pressed(JO_KEY_A))
			jo_audio_play_cd_track(2, 2, 1);
		if (jo_is_pad1_key_down(JO_KEY_B))
			draw_circle_at_cursor_pos(40);
		
		*/
		//Player2
		if(!jo_is_pad2_available())
			return;
		if (jo_is_pad2_key_pressed(JO_KEY_UP) && p2.y-p2.vel > 0)
			p2.y -= p2.vel;
		if (jo_is_pad2_key_pressed(JO_KEY_DOWN) && p2.y+p2.h+p2.vel < JO_TV_HEIGHT)
			p2.y += p2.vel;
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
void			load_audio(void){
	jo_audio_load_pcm("CHIME.PCM",JoSoundMono16Bit, &chime);
}
void load_pow_sprites(void){
	powSprites[0] =jo_sprite_add_image_pack("PMAXBALL", "PMB.TEX", JO_COLOR_Black);
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	p1.sprite = jo_sprite_add_tga("TEX", "P1.TGA", JO_COLOR_Transparent);
	p2.sprite = jo_sprite_add_tga("TEX", "P2.TGA", JO_COLOR_Transparent);
	selSprite = jo_sprite_add_tga("TEX","SEL.TGA",JO_COLOR_Green);
	sprite_ball = jo_sprite_add_tga("TEX","BALL.TGA",JO_COLOR_Black);
	ball.sprite = sprite_ball;
	load_pow_sprites();
	/* Then, you create the animation by giving the first sprite Id, the total of sprites, and the framerate */
	test.id=0;
	test.type=0;
	test.ttl=10;
	test.x=50;
	test.y=JO_TV_HEIGHT/2;
	test.w=0;
	test.sprite=0;
	MOD_ANIMATION = jo_create_sprite_anim(powSprites[test.sprite], 5, 5);
	test.animationId=MOD_ANIMATION;
	/* Finally, you chose the type of animation you wants => next step in my_draw() */
	jo_start_sprite_anim_loop(test.animationId);
	change_background("BAK.TGA");
	//jo_start_sprite_anim_loop(ring_anim_id);
	//
	load_audio();
	
	my_font = jo_font_load(JO_ROOT_DIR, "FONT.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_gamepad);
	jo_core_run();
}

/*
** END OF FILE
*/
