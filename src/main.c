#include "t3f/t3f.h"
#include "instance.h"
#include "document.h"
#include "ui/ui.h"
#include "ui/editor.h"
#include "defines.h"

/* main logic routine */
void app_logic(void * data)
{
	dm_editor_logic(data);
}

/* main rendering routine */
void app_render(void * data)
{
	dm_editor_render(data);
}

static ALLEGRO_BITMAP * make_checkerboard_bitmap(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM identity;
	ALLEGRO_BITMAP * bp;

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS | ALLEGRO_STATE_TARGET_BITMAP | ALLEGRO_STATE_TRANSFORM);
	al_set_new_bitmap_flags(0);
	bp = al_create_bitmap(2, 2);
	if(bp)
	{
		al_set_target_bitmap(bp);
		al_identity_transform(&identity);
		al_use_transform(&identity);
		al_lock_bitmap(bp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
		al_put_pixel(0, 0, c1);
		al_put_pixel(0, 1, c2);
		al_put_pixel(1, 0, c2);
		al_put_pixel(1, 1, c1);
		al_unlock_bitmap(bp);
	}
	al_restore_state(&old_state);
	return bp;
}

/* initialize our app, load graphics, etc. */
bool app_initialize(APP_INSTANCE * app, int argc, char * argv[])
{
	/* initialize T3F */
	if(!t3f_initialize(T3F_APP_TITLE, 1280, 720, 60.0, app_logic, app_render, T3F_USE_KEYBOARD | T3F_USE_MOUSE | T3F_RESIZABLE | T3F_NO_SCALE | T3F_USE_MENU, app))
	{
		printf("Error initializing T3F\n");
		return false;
	}
	memset(app, 0, sizeof(APP_INSTANCE));
	app->view_zoom = 1.0;
	app->bg_tile = make_checkerboard_bitmap(t3f_color_white, al_map_rgba_f(0.9, 0.9, 0.9, 1.0));
	if(!app->bg_tile)
	{
		return false;
	}
	app->ui = dm_create_ui(app);
	if(!app->ui)
	{
		return false;
	}
	app->state = DM_STATE_IDLE;
	return true;
}

int main(int argc, char * argv[])
{
	APP_INSTANCE app;

	if(app_initialize(&app, argc, argv))
	{
		t3f_run();
	}
	else
	{
		printf("Error: could not initialize T3F!\n");
	}
	t3f_finish();
	return 0;
}
