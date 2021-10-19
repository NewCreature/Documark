#include "t3f/t3f.h"
#include "instance.h"
#include "defines.h"

void dm_editor_center_view(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
}

void dm_editor_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
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
	}
}
