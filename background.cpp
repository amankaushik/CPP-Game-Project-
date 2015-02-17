#include "Global.h"
#include "Background.h"

void background :: initiate_background(float ox, float oy, float vx, float vy, int w, int h, int dx, int dy, ALLEGRO_BITMAP * image)
{
	origin_x = ox;
	origin_y = oy;
	vel_x = vx;
	vel_y = vy;
	width = w;
	height = h;
	dir_x = dx;
	dir_y = dy;

	im = image;
}

void background :: update_background()
{
	origin_x += vel_x * dir_x;

	if(origin_x + width <= 0)
		origin_x = 0;
}

void background :: render_background()
{
	al_draw_bitmap(im, origin_x, origin_y, 0);
	
	if(origin_x + width < screen_width)
		al_draw_bitmap(im, origin_x + width, origin_y, 0);
}


