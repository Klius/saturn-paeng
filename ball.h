typedef struct t_ball{
    int sprite ;
    float x ;
    float y ;
    int w;
    float speed;
    float velx;
    float vely;
	int big_ball;
} Ball;
const struct t_ball T_BALL_DEFAULT = {0,JO_TV_WIDTH/2 -8,JO_TV_HEIGHT/2 -8,16,3,3,0,0};

int			ball_collision(int px, int py,int pw, int ph,Ball* ball){
	if((ball->x < px+pw) && (px < ball->x+ball->w) && (ball->y < py+ph) && (py < ball->y+ball->w))
	{
		float relativeIntersectY = (py+(ph/2)) - (ball->y+(ball->w/2));
    	float normalizedRelativeIntersectionY = (relativeIntersectY/(ph/2));
    	float bounceAngle = normalizedRelativeIntersectionY * 60;
    	ball->velx *= -1;
		ball->vely = ball->speed*-jo_sinf(bounceAngle);
		ball->x += ball->velx;
		ball->y += ball->vely;
		
		return 10;
		//self:setSpeed(self.speed*-1,self.speed*-math.sin(bounceAngle))

	}
	return 0;
}

void ball_move(Ball* ball,int *score){
	ball->x += ball->velx; 
	ball->y += ball->vely;
	if( ball->x > JO_TV_WIDTH){
		ball->x = JO_TV_WIDTH/2 -8;
		ball->velx *= -1;
		*(score+0) = *(score+0) +1;/*
		if (score[0] == scoreLimit){
			winner = 1;
		}*/
	}else if( ball->x+ball->w < 0 ){
		ball->x = JO_TV_WIDTH/2 -8;
		ball->velx *= -1;
		*(score+1) = *(score+1)+1;
		/*if (score[1] == scoreLimit){
			winner = 2;
		}*/
	}
	if (ball->y < 0){
		ball->vely *=-1;
		ball->y = 2;
	}
	if(ball->y+ball->w > JO_TV_HEIGHT){
		ball->vely *=-1;
		ball->y = JO_TV_HEIGHT -ball->w;
	}
}

void ball_draw(Ball* ball){
	if (ball->big_ball > 0)
		jo_sprite_change_sprite_scale(2);
	jo_sprite_draw3D2(ball->sprite,ball->x,ball->y,500);
	jo_sprite_change_sprite_scale(1);
}

void ball_powerup(int type,Ball* ball){
	if (type == BIG_BALL){
		ball->big_ball = 50000;
		ball->w = 32;
	}
}
/*
** END OF FILE
*/

