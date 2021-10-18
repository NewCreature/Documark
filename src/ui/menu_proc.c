#include "t3f/t3f.h"
#include "../instance.h"

int dm_menu_file_new(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	ALLEGRO_FILECHOOSER * fc;

	fc = al_create_native_file_dialog(NULL, "Choose Base Image File", "*.png;*.jpg;*.bmp;*.tga", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	if(al_show_native_file_dialog(t3f_display, fc))
	{
		if(al_get_native_file_dialog_count(fc))
		{
			if(app->document)
			{
				dm_destroy_document(app->document);
			}
			app->document = dm_create_document(al_get_native_file_dialog_path(fc, 0));
		}
	}
	return 0;
}

int dm_menu_file_open(int id, void * data)
{
	return 0;
}

int dm_menu_file_save(int id, void * data)
{
	return 0;
}

int dm_menu_file_quit(int id, void * data)
{
	return 0;
}

int dm_menu_edit_add_text(int id, void * data)
{
	return 0;
}

int dm_menu_edit_add_image(int id, void * data)
{
	return 0;
}

int dm_menu_edit_delete(int id, void * data)
{
	return 0;
}
