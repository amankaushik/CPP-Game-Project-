#include "character.h"

character :: character()
{
	origin_x = 0;
	origin_y = 0;
	vel_x = 0;
	vel_y = 0;
	dir_x = 0;
	dir_y = 0;
	bound_x = 0;
	bound_y = 0;
	max_frame = 0;
	cur_frame = 0;
	frame_count = 0;
	frame_delay = 0;
	frame_width = 0;
	frame_height = 0;
	ani_col = 0;
	ani_dir = 0;

	image = NULL;

	alive = true;
	collidable = true;
}

void character :: initiate(float x, float y, float vx, float vy, int dx, int dy, int bx, int by)
{
	character::origin_x = x;
	character::origin_y = y;

	character::vel_x = vx;
	character::vel_y = vy;

	character::dir_x = dx;
	character::dir_y = dy;

	character::bound_x = bx;
	character::bound_y = by;
}

void character :: update()
{
	origin_x += vel_x * dir_x;
	origin_y += vel_y * dir_y;
}

bool character::check_collisions(character *ob)
{
	float o_x = ob->get_origin_x();
	float o_y = ob->get_origin_y();

	int ob_x = ob->get_bound_x();
	int ob_y = ob->get_bound_y();

	if( origin_x + bound_x > o_x - ob_x &&
		origin_x - bound_x < o_x + ob_x &&
		origin_y + bound_y > o_y - ob_y &&
		origin_y - bound_y < o_y + ob_y)
		return true;
	else
		return false;
}

bool character :: coll_idable()
{
	return alive && collidable;
}

