#include "t3f/t3f.h"
#include "instance.h"
#include "defines.h"

void dm_editor_center_view(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
}

static int get_hover_element(APP_INSTANCE * app)
{
	int i;
	int mouse_x, mouse_y;

	mouse_x = t3f_mouse_x - app->view_x;
	mouse_y = t3f_mouse_y - app->view_y;
	for(i = 0; i < app->document->element_count; i++)
	{
		if(mouse_x >= app->document->element[i].x && mouse_x < app->document->element[i].x + app->document->element[i].width && mouse_y >= app->document->element[i].y && mouse_y < app->document->element[i].y + app->document->element[i].height)
		{
			return i;
		}
	}
	return -1;
}

static void idle_logic(APP_INSTANCE * app)
{
	float speed;

	speed = (1.0 / app->view_zoom) * DM_SCROLL_SPEED;
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		app->view_x -= speed;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		app->view_x += speed;
	}
	if(t3f_key[ALLEGRO_KEY_UP])
	{
		app->view_y -= speed;
	}
	if(t3f_key[ALLEGRO_KEY_DOWN])
	{
		app->view_y += speed;
	}
	if(t3f_key[ALLEGRO_KEY_MINUS])
	{
		if(app->view_zoom > 1.0)
		{
			app->view_zoom -= 0.5;
		}
		else
		{
			app->view_zoom *= 0.75;
		}
		t3f_key[ALLEGRO_KEY_MINUS] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_EQUALS])
	{
		if(app->view_zoom > 1.0)
		{
			app->view_zoom += 0.5;
		}
		else
		{
			app->view_zoom /= 0.75;
		}
		t3f_key[ALLEGRO_KEY_EQUALS] = 0;
	}
	app->hover_element = get_hover_element(app);
	if(t3f_mouse_button[0])
	{
		if(app->hover_element >= 0)
		{
			if(app->document->element[app->hover_element].type == DM_ELEMENT_TEXT)
			{
				app->state = DM_STATE_RESIZE_TEXT;
			}
			else
			{
				app->state = DM_STATE_RESIZE_IMAGE;
			}
		}
		else
		{
			if(app->mode == DM_MODE_TEXT)
			{
				app->document->element[app->document->element_count].type = DM_ELEMENT_TEXT;
				app->document->element[app->document->element_count].x = app->view_x + t3f_mouse_x;
				app->document->element[app->document->element_count].y = app->view_y + t3f_mouse_y;
				app->document->element[app->document->element_count].width = 0;
				app->document->element[app->document->element_count].height = 0;
				app->state = DM_STATE_CREATE_TEXT;
			}
		}
	}
}

static void create_text_logic(APP_INSTANCE * app)
{
	if(t3f_mouse_button[0])
	{
		app->document->element[app->document->element_count].width = t3f_mouse_x + app->view_x - app->document->element[app->document->element_count].x;
		app->document->element[app->document->element_count].height = t3f_mouse_y + app->view_y - app->document->element[app->document->element_count].y;
	}
	else
	{
		if(!app->default_font)
		{
			app->default_font = al_load_font("data/OpenSans-Regular.ttf", app->document->element[app->document->element_count].height, 0);
		}
		app->document->element[app->document->element_count].support_data = app->default_font;
		app->document->element[app->document->element_count].data = "Test";
		app->document->element_count++;
		app->state = DM_STATE_IDLE;
	}
}

void dm_editor_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->document)
	{
		switch(app->state)
		{
			case DM_STATE_IDLE:
			{
				idle_logic(app);
				break;
			}
			case DM_STATE_CREATE_TEXT:
			{
				create_text_logic(app);
				break;
			}
		}
	}
}

static void render_bg_tiles(APP_INSTANCE * app)
{
	int i, j;
	int tw, th, ts;

	ts = 16;
	tw = t3f_default_view->width / ts + 1;
	th = t3f_default_view->height / ts + 1;
	for(i = 0; i < th; i++)
	{
		for(j = 0; j < tw; j++)
		{
			t3f_draw_scaled_bitmap(app->bg_tile, t3f_color_white, j * ts, i * ts, 0, ts, ts, 0);
		}
	}
}

void dm_editor_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	render_bg_tiles(app);
	if(app->document)
	{
		dm_render_document(app->document, -app->view_x, -app->view_y, app->view_zoom);
		if(app->state == DM_STATE_CREATE_TEXT)
		{
			al_draw_filled_rectangle(app->document->element[app->document->element_count].x, app->document->element[app->document->element_count].y, app->document->element[app->document->element_count].x + app->document->element[app->document->element_count].width, app->document->element[app->document->element_count].y + app->document->element[app->document->element_count].height, al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
		}
	}
}
