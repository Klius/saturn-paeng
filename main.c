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
static jo_font      *my_font;
static jo_sound     chime;
int p1x = 30;
int p1y = JO_TV_HEIGHT/2-31;
int p1w = 8;
int p1h = 62;
int p1vel = 2;
int p1Sprite = 0;
int p1hit = 0;
int p2x = JO_TV_WIDTH - 38;
int p2y = JO_TV_HEIGHT/2-31;
int p2w = 8;
int p2h = 62;
int p2vel = 2;
int p2Sprite = 0;
int p2hit = 0;
int ballSprite = 0;
float bx = JO_TV_WIDTH/2 -8;
float by = JO_TV_HEIGHT/2 -8;
int bw = 16;
float bs = 3;
float bvelx = 3;
float bvely = 0;
int score[] = {0,0};
int scoreLimit = 10;
short winner = 0;
//Audio
bool is_cd_playing = 0;
//main menu
enum MAIN_OP {MULTIPLAYER,MEN_OPTIONS};
enum MAIN_OP currentMenuOption = MULTIPLAYER;
int p1Timeout = 0;
int p2Timeout = 0;
int iptimeout = 10;
int cursorPos[2][4] = {
	{98,110,208,120},
	{120,142,188,150}
};
int selSprite=0;
//STATES
enum STATE {MAIN, GAME, OPTIONS};
enum STATE currentState = MAIN;
void 			ball_move(void){
	bx += bvelx; 
	by += bvely;
	if( bx > JO_TV_WIDTH){
		bx = JO_TV_WIDTH/2 -8;
		bvelx *= -1;
		++score[0];
		if (score[0] == scoreLimit){
			winner = 1;
		}
	}else if( bx+bw < 0 ){
		bx = JO_TV_WIDTH/2 -8;
		bvelx *= -1;
		++score[1];
		if (score[1] == scoreLimit){
			winner = 2;
		}
	}
	if (by < 0){
		bvely *=-1;
		by = 2;
	}
	if(by+bw > JO_TV_HEIGHT){
		bvely *=-1;
		by = JO_TV_HEIGHT -bw;
	}
}
int			ball_collision(int px, int py,int pw, int ph){
	if((bx < px+pw) && (px < bx+bw) && (by < py+ph) && (py < by+bw))
	//jo_hitbox_detection (pSprite, px, py, ballSprite, bx, by) )
	{
		float relativeIntersectY = (py+(ph/2)) - (by+(bw/2));
    	float normalizedRelativeIntersectionY = (relativeIntersectY/(ph/2));
    	float bounceAngle = normalizedRelativeIntersectionY * 60;
    	bvelx *= -1;
		bvely = bs*-jo_sinf(bounceAngle);
		bx += bvelx;
		by += bvely;
		jo_audio_play_sound(&chime);
		return 10;
		//self:setSpeed(self.speed*-1,self.speed*-math.sin(bounceAngle))

	}
	return 0;
}
void			game_draw(void){
	jo_font_printf(my_font, JO_TV_WIDTH/2-80, 20, 4.0f, "%d",score[0]);
	jo_font_printf(my_font, JO_TV_WIDTH/2+60, 20, 4.0f, "%d",score[1]);
	if (winner == 0){
		if(p1hit == 0){
			p1hit = ball_collision(p1x,p1y,p1w,p1h);
		}else{
			--p1hit;
		}if(p2hit == 0 ){
			p2hit = ball_collision(p2x,p2y,p2w,p2h);
		}else{
			--p2hit;
		}
		ball_move();
	}
	else{
		jo_font_printf(my_font, 30, JO_TV_HEIGHT/2-20, 2.0f, "PLAYER %d WINS",winner);
	}
	jo_sprite_draw3D2(p1Sprite, p1x, p1y, 500);
	jo_sprite_draw3D2(p2Sprite, p2x, p2y, 500);
	jo_sprite_draw3D2(ballSprite,bx,by,500);
	
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
void			my_draw(void)
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
	jo_printf(0,0,"bx: %d by:%d",bx);
	jo_printf(0,1,"p1hit: %d",p1hit);
	jo_printf(0,2, "p2hit:%d",p2hit);
	jo_printf(0, 3, "Sprite memory usage: %d%% ", jo_sprite_usage_percent()); 
	jo_printf(0, 4, "Dynamic memory usage: %d%%  ", jo_memory_usage_percent());
	jo_printf(0,5, "p1y:%d  TVHEIGHT:%d",p1y+p1h+p1vel,JO_TV_HEIGHT);
	jo_printf(0,6, "currentMenuOption:%d ",currentMenuOption);
	jo_printf(0,9, "cursor1 X:%d Y:%d",cursorPos[currentMenuOption][0],cursorPos[currentMenuOption][1]);
	jo_printf(0,10, "cursor2 X:%d Y:%d",cursorPos[currentMenuOption][2],cursorPos[currentMenuOption][3]);
}
void reset(void){
	bs = 3;
	bvelx = 3;
	bvely = 0;
	score[0] = 0,score[1]=0;
	winner = 0;
	bx = JO_TV_WIDTH/2 -8;
	by = JO_TV_HEIGHT/2 -8;
	p1y = JO_TV_HEIGHT/2-31;
	p2y = JO_TV_HEIGHT/2-31;
}
void			my_gamepad(void)
{
	if(currentState == GAME){
		if (!jo_is_pad1_available())
			return ;
		if (jo_is_pad1_key_pressed(JO_KEY_UP) && p1y-p1vel > 0)
			p1y -= p1vel;
		if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && p1y+p1h+p1vel < JO_TV_HEIGHT)
			p1y += p1vel;
		if (jo_is_pad1_key_pressed(JO_KEY_START) && (winner > 0 ) )
			reset();
	/* if (jo_is_pad1_key_pressed(JO_KEY_A))
			jo_audio_play_cd_track(2, 2, 1);
		if (jo_is_pad1_key_down(JO_KEY_B))
			draw_circle_at_cursor_pos(40);
		
		*/
		//Player2
		if(!jo_is_pad2_available())
			return;
		if (jo_is_pad2_key_pressed(JO_KEY_UP) && p2y-p2vel > 0)
			p2y -= p2vel;
		if (jo_is_pad2_key_pressed(JO_KEY_DOWN) && p2y+p2h+p2vel < JO_TV_HEIGHT)
			p2y += p2vel;
	}
	if(currentState == MAIN){
		if (!jo_is_pad1_available())
			return ;
		if(p1Timeout < 0){
			if (jo_is_pad1_key_pressed(JO_KEY_A)){
				if(currentMenuOption == MULTIPLAYER)
					currentState = GAME;
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
				p1Timeout = iptimeout;
				if (currentMenuOption == -1)
					currentMenuOption = 1;
			}
			if (jo_is_pad1_key_pressed(JO_KEY_DOWN)){
				currentMenuOption++;
				p1Timeout = iptimeout;
				if (currentMenuOption > 1)
					currentMenuOption = 0;
			}
			
		}else{
			p1Timeout--;
		}
	}
}
void			load_audio(void){
	jo_audio_load_pcm("CHIME.PCM",JoSoundMono16Bit, &chime);
}

void change_background(char background[]){
	//Background
    jo_img      bg;
    bg.data = NULL;
    jo_tga_loader(&bg, "BG", background, JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);	
}
void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	p1Sprite = jo_sprite_add_tga("TEX", "P1.TGA", JO_COLOR_Transparent);
	p2Sprite = jo_sprite_add_tga("TEX", "P2.TGA", JO_COLOR_Transparent);
	selSprite = jo_sprite_add_tga("TEX","SEL.TGA",JO_COLOR_Green);
	ballSprite = jo_sprite_add_tga("TEX","BALL.TGA",JO_COLOR_Black);

	load_audio();
	
	my_font = jo_font_load(JO_ROOT_DIR, "FONT.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_gamepad);
	jo_core_run();
}

/*
** END OF FILE
*/
