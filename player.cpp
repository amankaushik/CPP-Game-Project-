#include "player.h"


void player :: initiate(ALLEGRO_BITMAP * image) 
{
	character :: initiate(100, 300, 5, 5, 0, 0, 10, 12);
	set_ID(PLAYER);
	set_alive(true);
	player::image = image;
	lives = 30;
	score = 0;
	ani_row = 1;

	max_frame = 3;
	cur_frame = 0;
	frame_width = 46;
	frame_height = 41;
	ani_col = 3;
	ani_dir = 1;
}

void player :: update()
{
    character :: update();

	if(origin_x < 0)
		origin_x = 0;
	else if(origin_x > screen_width)
		origin_x = screen_width;

	if(origin_y < 0)
		origin_y = 0;
	else if(origin_y > screen_height)
		origin_y = screen_height;
}

void player :: render()
{
	int frame_x = (cur_frame % ani_col) * frame_width;
	int frame_y = ani_row * frame_height;

	al_draw_bitmap_region(image, frame_x, frame_y, frame_width, frame_height,
		origin_x - frame_width / 2, origin_y - frame_height / 2, 0);
}

void player :: move_up()
{
	ani_row = 0;
	dir_y = -1;
}
void player::move_down()
{
	ani_row = 2;
	dir_y = 1;
}

void player :: move_left()
{
	cur_frame = 2;
	dir_x = -1;
}
void player :: move_right()
{
	cur_frame = 1;
	dir_x = 1;
}

void player :: reset_original_position(int pos)
{
	if(pos == 1)
	{
		ani_row = 1;
		dir_y = 0;
	}
	else
	{
		cur_frame = 0;
		dir_x = 0;
	}
}

void player :: collided(int objectID)
{
	if(objectID == ENEMY)
		lives--;
} 

int player :: get_score()
{
	return(score);
}

int player :: get_lives()
{
	return(lives);
}

void player :: get_life()
{
	lives--;
}

void player :: inc_score()
{
	score +=10;
}

void player :: inc_life()
{
	lives++;
}