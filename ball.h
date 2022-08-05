typedef struct t_ball{
    int sprite ;
    float x ;
    float y ;
    int w;
    float speed;
    float velx;
    float vely;
} Ball;
const struct t_ball T_BALL_DEFAULT = {0,JO_TV_WIDTH/2 -8,JO_TV_HEIGHT/2 -8,16,3,3,0};

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


/*
** END OF FILE
*/
