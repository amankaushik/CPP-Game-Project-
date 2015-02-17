#include "explosion.h"

Explosion::Explosion(float x, float y, ALLEGRO_BITMAP *image)
{
	character::initiate(x, y, 0, 0, 0, 0, 0, 0);

	set_ID(EXPLOSION);
	set_collidable(false);

	max_frame = 31;
	cur_frame = 0;
	frame_count = 0;
	frame_delay = 1;
	frame_width = 128;
	frame_height = 128;
	ani_col = 8;
	ani_dir = 1;

	Explosion::image = image;
}

void Explosion::update()
{
	character::update();

	if(++frame_count >= frame_delay)
	{
		cur_frame += ani_dir;
		if(cur_frame >= max_frame)
			set_alive(false);

		frame_count = 0;
	}
}

void Explosion::render()
{
	int fx = (cur_frame % ani_col) * frame_width;
	int fy = (cur_frame / ani_col) * frame_height;

	al_draw_bitmap_region(image, fx, fy, frame_width,
		frame_height, origin_x - frame_width / 2, origin_y - frame_height / 2, 0);
}

void Explosion :: initiate()
{
	std::cout<<"Explosion !!"<<"\n";
}

void Explosion :: collided(int a)
{
	std::cout<<"Collision !!"<<"\n";
}