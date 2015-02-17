#include <iostream>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <list>
#include <string>
#include <conio.h>
#include "Global.h"
#include "character.h"
#include "player.h"
#include "Background.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"

bool keys[] = {false, false, false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, ESC};
char name[15];

void __cdecl take_life();
void __cdecl score();
void change_state(int &state, int new_state);

background *ptr_back;
player *ptr_ply;
std::list<character *> chr;
std::list<character *>::iterator trav;
std::list<character *>::iterator trav_1;

background *ptr_start_screen;
background *ptr_lost_screen;

ALLEGRO_SAMPLE_INSTANCE *music;

int main()
{
	std::cout<<"Enter your name : ";
	gets_s(name);

	bool done = false;
	bool render = false;

	int state = -1;
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font;
	ALLEGRO_BITMAP *back_image = NULL;
	ALLEGRO_BITMAP *ship_image = NULL;
	//ALLEGRO_BITMAP *middle_image = NULL;
	//ALLEGRO_BITMAP *front_image = NULL;
	ALLEGRO_BITMAP *enemy_image = NULL;
	ALLEGRO_BITMAP *explosion_image = NULL;
	ALLEGRO_BITMAP *lost_screen = NULL;
	ALLEGRO_BITMAP *start_screen = NULL;
	ALLEGRO_SAMPLE *bullet_fire = NULL;
	ALLEGRO_SAMPLE *expl = NULL;
	ALLEGRO_SAMPLE *back_music = NULL;

	ptr_back = new background();
    ptr_start_screen = new background();
	ptr_lost_screen = new background();
	//background middle;
	//background front;
	//character *ptr_chr
	//player obj_ply
		
	ptr_ply = new player();
	//ptr_chr = &obj_ply

	//const int max_no_of_bul = 8;
	//Bullet (*ptr_bul)[max_no_of_bul];
	//int size = 0;
	
	if(!al_init())			
	{
		al_show_native_message_box(NULL, NULL, "Error", "Could not initialiae Allegro 5 ", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;	
	}

	display = al_create_display(screen_width, screen_height);	

	if(!display)
	{
		al_show_native_message_box(display, "sample title", "display settings", "Could not create Allegro 5 display", NULL, NULL);
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	font = al_load_font("horrendo.ttf", 20, 0);
	al_reserve_samples(15);

	back_image = al_load_bitmap("space_bg_frederator8569.png");
	//middle_image = al_load_bitmap("reach_for_it_by_ambientconceptions-d3cqzna.jpg");
    //front_image = al_load_bitmap("banner.jpg");	
	//al_convert_mask_to_alpha(middle_image, al_map_rgb(255, 255, 255));
	start_screen = al_load_bitmap("1280x960-space-invaders-press-start-wallpaper.jpg");
	lost_screen	= al_load_bitmap("Normandy_SR1_Destroyed.png");

	ship_image = al_load_bitmap("spaceship.png");
	al_convert_mask_to_alpha(ship_image, al_map_rgb(255, 0, 255));
	
	enemy_image = al_load_bitmap("asteroid.bmp");
	al_convert_mask_to_alpha(enemy_image, al_map_rgb(255, 0, 255));

	explosion_image = al_load_bitmap("explosion.png");
	//cout<<back_image<<endl;
	//cout<<middle_image<<endl;

	ptr_back -> initiate_background(0, 0, 0.5, 0, al_get_bitmap_width(back_image), al_get_bitmap_height(back_image), -1, 1, back_image);
	ptr_start_screen -> initiate_background(0, 0, 0.5, 0, 800, 600, -1, 1, start_screen);
	ptr_lost_screen -> initiate_background(0, 0, 0, 0, al_get_bitmap_width(lost_screen), al_get_bitmap_height(lost_screen), -1, 1, lost_screen);
	//middle.initiate_background(0, 0, 3, 0, al_get_bitmap_width(middle_image), al_get_bitmap_height(middle_image), -1, 1, middle_image);
	//front.initiate_background(0, 600, 0, 0, al_get_bitmap_width(front_image), al_get_bitmap_height(front_image), -1, 1, front_image);

	//ptr_char1 -> initiate(ship_image);
	ptr_ply -> initiate(ship_image);
	chr.push_back(ptr_ply);

	bullet_fire = al_load_sample("Slash8-Bit.ogg");
	expl = al_load_sample("MediumExplosion8-Bit.ogg");
	back_music = al_load_sample("Boss_Battle_Theme_Song_Sound_Clip_and_Quote_Hark.ogg");

	music = al_create_sample_instance(back_music);
	al_set_sample_instance_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(music, al_get_default_mixer());

	change_state(state, START);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	
	al_hide_mouse_cursor(display);

	al_start_timer(timer);

	while(!done)
	{
		//cout<<"In While ..."<<endl;
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if(events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(events.keyboard.keycode)
		 {
			case ALLEGRO_KEY_ESCAPE:
				//cout<<"Esc key pressed ..."<<endl;
				keys[ESC] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				break;
     	 }
		}

		else if(events.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch(events.keyboard.keycode)
				 {
					case ALLEGRO_KEY_ESCAPE:
						//cout<<"Esc key pressed ..."<<endl;
						done = true;
						break;
					case ALLEGRO_KEY_LEFT:
						keys[LEFT] = false;
						break;
					case ALLEGRO_KEY_RIGHT:
						keys[RIGHT] = false;
						break;
					case ALLEGRO_KEY_UP:
						keys[UP] = false;
						break;
					case ALLEGRO_KEY_DOWN:
						keys[DOWN] = false;
						break;
					case ALLEGRO_KEY_SPACE:
						if(state == START)
							change_state(state, PLAYING);
						else if(state == PLAYING)
						{
							Bullet *ptr_bul = new Bullet(ptr_ply ->get_origin_x() + 15, ptr_ply ->get_origin_y(), &score);
							chr.push_back(ptr_bul);
							al_play_sample(bullet_fire, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						}
						else if(state == LOST)
						{
							change_state(state, PLAYING);
						}
							
						break;
				 }
			}

		else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		else if(events.type == ALLEGRO_EVENT_TIMER)
		{
			//cout<<"updating ..."<<endl;
			if(state == PLAYING)
			{
			if(keys[UP])
				ptr_ply ->  move_up();
			else if(keys[DOWN])
				ptr_ply ->  move_down();
			else
				ptr_ply -> reset_original_position(1);

			if(keys[LEFT])
				ptr_ply ->  move_left();
			else if(keys[RIGHT])
				ptr_ply ->  move_right();
			else
				ptr_ply -> reset_original_position(0);
			
			if(rand() % 100 == 0)
			{
				enemy * ptr_ene = new enemy(screen_width, 26 + (rand() % (screen_height - 55)), enemy_image, &take_life);
				chr.push_back(ptr_ene);
			}

			ptr_back -> update_background();
			//ptr_middle -> update_background();
			//ptr_front -> update_background();
			
			//ptr_ply -> update();

			for(trav = chr.begin(); trav != chr.end(); ++trav)
					(*trav)->update();

			for(trav = chr.begin(); trav != chr.end(); ++trav)
				{
					if( ! (*trav)->coll_idable() ) continue;

					for(trav_1 = trav; trav_1 != chr.end(); ++trav_1)
					{
						if( !(*trav_1)->coll_idable() ) continue;
						if( (*trav)->get_ID() == (*trav_1)->get_ID()) continue;

						if( (*trav)->check_collisions((*trav_1)))
						{
							(*trav)->collided( (*trav_1)->get_ID());
							(*trav_1)->collided((*trav)->get_ID());

							Explosion *explosion = new Explosion(((*trav)->get_origin_x() + (*trav)->get_origin_x()) / 2,
																((*trav_1)->get_origin_y() + (*trav_1)->get_origin_y()) / 2, explosion_image);

							chr.push_back(explosion);
							//ptr_ply -> inc_score();
							al_play_sample(expl, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						}
						
					}
				}

			if(ptr_ply -> get_lives() <= 0)
				change_state(state, LOST);

			/*for(trav = chr.begin(); trav != chr.end(); ++trav)
			{
				if( ! (*trav)->coll_idable() ) continue;
				if( (*trav)->get_ID() == BULLET || (*trav)->get_ID() == EXPLOSION) continue;
				if((*trav)->get_ID() == ENEMY)
				{
					if( ptr_ply->check_collisions((*trav)))
			        {
						ptr_ply->collided((*trav)->get_ID());
						(*trav)->collided(ptr_ply->get_ID());
						Explosion *explosion = new Explosion((ptr_ply->get_origin_x() + ptr_ply->get_origin_x()) / 2,((*trav)->get_origin_y() + (*trav)->get_origin_y()) / 2, explosion_image);
						chr.push_back(explosion);
						//ptr_ply -> get_life();
						//ptr_ply -> inc_life();
						//if(ptr_ply -> get_lives() < 1)
							//exit(0);
						//al_play_sample(boom, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}
			}*/

			
			}	
			for(trav = chr.begin(); trav != chr.end(); )
			{
				if(! (*trav)->get_alive())
				{
					delete (*trav);
					trav = chr.erase(trav);
				}
				else
					trav++;
			}

			render = true;
		}

		/*else if(events.type == ALLEGRO_EVENT_MOUSE_AXES) // event fires when mouse inside the game window
		{
			pos_x = events.mouse.x;
			pos_y = events.mouse.y;			
		}*/

		else if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // event fires when mouse inside the game window
		{
			if(events.mouse.button & 2)	// left mouse button = 1, right mouse button = 2 	
				done = true;
		}

		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			//cout<<"rendering ..."<<endl;
			
			if(state == START)
			{
    			ptr_start_screen->render_background();
			}
			if(state == PLAYING)
			{
				ptr_back -> render_background();
				//middle.render_background();
				//front.render_background();
				//ptr_ply -> render();

				for(trav = chr.begin(); trav != chr.end(); ++trav)
						(*trav) -> render();

				al_draw_textf(font, al_map_rgb(255, 0, 0), 5, 5, 0, "%s has %i lives left. %s has destroyed %i objects. %s's score : %i",name, ptr_ply -> get_lives(),name, ptr_ply -> get_score() /10, name, ptr_ply -> get_score());
		    }
			if(state == LOST)
			{
				ptr_lost_screen->render_background();
				al_draw_text(font, al_map_rgb(255, 0, 0), 350, 300, 0, "YOU LOSE");
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
    }

	for(trav_1 = chr.begin(); trav != chr.end(); ++trav)
	{
	   (*trav)-> ~character();
	   trav = chr.erase(trav);
	}

	ptr_back->~background();
	delete ptr_back;
	ptr_start_screen->~background();
	delete ptr_start_screen;
	ptr_lost_screen->~background();
	delete ptr_lost_screen;

	al_destroy_bitmap(back_image);
	al_destroy_bitmap(start_screen);
	al_destroy_bitmap(start_screen);
	al_destroy_sample(bullet_fire);
	al_destroy_sample(expl);
	al_destroy_sample(back_music);
	al_destroy_sample_instance(music);
	//al_destroy_bitmap(middle_image);
	//al_destroy_bitmap(front_image);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);	

	system("pause");
	return(0);
}

void __cdecl take_life()
{
	ptr_ply->get_life();
}
void __cdecl score()
{
	ptr_ply->inc_score();
}

void change_state(int &state, int new_state)
{
	if(state == START)
	{}
	else if(state == PLAYING)
	{
		for(trav = chr.begin(); trav != chr.end(); ++trav)
		{
			if( (*trav)->get_ID() != PLAYER && (*trav)->get_ID() != MISC)
				(*trav)->set_alive(false);			
		}

		al_stop_sample_instance(music);
	}
	else if(state == LOST)
	{}

	state = new_state;

	if(state == START)
	{}
	else if(state == PLAYING)
	{
		//ptr_ply -> initiate();
		al_play_sample_instance(music);
	}
	else if(state == LOST)
	{}
}