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
int p1x = 30;
int p1y = 0;
int p1w = 8;
int p1h = 62;
int p1vel = 2;
int p1Sprite = 0;
int p1hit = 0;
int p2x = JO_TV_WIDTH - 38;
int p2y = 0;
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

void            draw_circle_at_cursor_pos( int radius)
{
    int         angle;
    while (radius >0 ){
    for (angle = 0; angle < 360; ++angle){
        jo_put_pixel_in_background(p1x + jo_cos_mult(radius, angle),
                                   p1y + jo_sin_mult(radius, angle),
                                   JO_COLOR_Blue);
	}
	 --radius;
	}
}
void 			ball_move(void){
	bx += bvelx; 
	by += bvely;
	if( bx > JO_TV_WIDTH){
		bx = JO_TV_WIDTH/2 -8;
		bvelx *= -1;
		++score[0];
	}else if( bx+bw < 0 ){
		bx = JO_TV_WIDTH/2 -8;
		bvelx *= -1;
		++score[1];
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
		return 10;
		//self:setSpeed(self.speed*-1,self.speed*-math.sin(bounceAngle))

	}
	return 0;
}
void			my_draw(void)
{
	jo_font_printf(my_font, JO_TV_HEIGHT/2-40, 20, 4.0f, "%d",score[0]);
	jo_font_printf(my_font, JO_TV_HEIGHT/2+60, 20, 4.0f, "%d",score[1]);
	jo_printf(0,1,"bx: %d by:%d",bx);
	jo_printf(0,2,"p1hit: %d",p1hit);
	jo_printf(0,3, "p2hit:%d",p2hit);
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
	jo_sprite_draw3D2(p1Sprite, p1x, p1y, 500);
	jo_sprite_draw3D2(p2Sprite, p2x, p2y, 500);
	jo_sprite_draw3D2(ballSprite,bx,by,500);
	
	jo_printf(0, 5, "Dynamic memory usage: %d%%  ", jo_memory_usage_percent());
}

void			my_gamepad(void)
{
	if (!jo_is_pad1_available())
		return ;
	if (jo_is_pad1_key_pressed(JO_KEY_UP) && p1y-p1vel > 0)
		p1y -= p1vel;
	if (jo_is_pad1_key_pressed(JO_KEY_DOWN) && p1y+p1h+p1vel < JO_TV_HEIGHT)
		p1y += p1vel;
    /*if (jo_is_pad1_key_pressed(JO_KEY_A)){
        ++bs;++bvelx;}
		jo_put_pixel_in_background(p1x, p1y, JO_COLOR_Red);
    if (jo_is_pad1_key_down(JO_KEY_B))
        draw_circle_at_cursor_pos(40);
    if (jo_is_pad1_key_pressed(JO_KEY_START))
        jo_clear_background(JO_COLOR_White);
	*/
	//Player2
	if(!jo_is_pad2_available())
		return;
	if (jo_is_pad2_key_pressed(JO_KEY_UP) && p2y+-p2vel > 0)
		p2y -= p2vel;
	if (jo_is_pad2_key_pressed(JO_KEY_DOWN) && p2y+p2h+p2vel < JO_TV_HEIGHT)
		p2y += p2vel;
    /*if (jo_is_pad2_key_pressed(JO_KEY_A))
        //jo_put_pixel_in_background(p2x, p2y, JO_COLOR_Red);
    if (jo_is_pad2_key_down(JO_KEY_B))
        //draw_circle_at_cursor_pos(40);
    if (jo_is_pad2_key_pressed(JO_KEY_START))
        //jo_clear_background(JO_COLOR_White);
	*/
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	p1Sprite = jo_sprite_add_tga("TEX", "P1.TGA", JO_COLOR_Transparent);
	p2Sprite = jo_sprite_add_tga("TEX", "P2.TGA", JO_COLOR_Transparent);
	ballSprite = jo_sprite_add_tga("TEX","BALL.TGA",JO_COLOR_Black);
	//Background
    jo_img      bg;
    bg.data = NULL;
    jo_tga_loader(&bg, "BG", "BAK.TGA", JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);
	
	my_font = jo_font_load(JO_ROOT_DIR, "FONT.TGA", JO_COLOR_Green, 8, 8, 2, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"?=%&',.()*+-/");
	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_gamepad);
	jo_core_run();
}

/*
** END OF FILE
*/
