#include "Bullet.h"

Bullet::Bullet(float x, float y/*, ALLEGRO_BITMAP *image*/, void (*score)(void))
{
	character::initiate(x, y, 10, 0, 1, 0, 0, 0);
	//Bullet::image = image;
	set_ID(BULLET);

	//max_frame = 4;
	//cur_frame = 0;
	//frame_count = 0;
	//frame_width = 39;
	//frame_height = 39;
	//ani_col = 4;
	//ani_row = 2;
	//ani_dir = 1;

	Bullet::score = score;
}

void Bullet::update()
{
	character::update();
    
	if(origin_x > screen_width)
		collided(BORDER);
}

void Bullet::render()
{
	al_draw_filled_circle(origin_x, origin_y, 2, al_map_rgb(242, 238, 10));
}

void Bullet::collided(int objectID)
{
	if(objectID == ENEMY || objectID == BORDER)
		set_alive(false);

	if(objectID == ENEMY)
		score();
}

void Bullet::initiate()
{
	std::cout<<"Bullet!!"<<"\n";
}

//int Bullet :: size = 0;