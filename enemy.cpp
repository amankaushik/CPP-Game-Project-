#include "enemy.h"

enemy::enemy(float x, float y, ALLEGRO_BITMAP *image, void (*take_life)(void))
{
	character::initiate(x, y, 4, 0, -1, 0, 40, 40);

	set_ID(ENEMY);

	max_frame = 64;
	cur_frame = 0;
	frame_count = 0;
	frame_delay = 2;
	frame_width = 64;
	frame_height = 64;
	ani_col = 8;

	if( rand() % 2)
		ani_dir = 1;
	else
		ani_dir = -1;

	enemy::image = image;

	enemy::take_life = take_life;
}

void enemy::update()
{
	character::update();

	if(++frame_count >= frame_delay)
	{
		cur_frame += ani_dir;
		if(cur_frame >= max_frame)
			cur_frame = 0;
		else if(cur_frame <= 0)
			cur_frame = max_frame;

		frame_count = 0;
	}

	if(origin_x + frame_width < 0)
		collided(BORDER);
}

void enemy::render()
{
	int fx = (cur_frame % ani_col) * frame_width;
	int fy = (cur_frame / ani_col) * frame_height;

	al_draw_bitmap_region(image, fx, fy, frame_width, frame_height, 
		origin_x - frame_width / 2, origin_y - frame_height / 2, 0);
}

void enemy::collided(int objectID)
{
	if(objectID == BORDER)
	{
		take_life();
	}

	if(objectID != ENEMY)
	{
		set_alive(false);
	}
}

void enemy::initiate()
{
	std::cout<<"Comet !!"<<"\n";
}