#include "ui.h"
#include "menu_init.h"
#include "menu_proc.h"

bool dm_setup_menus(DM_UI * uip)
{
	uip->menu[DM_MENU_FILE] = al_create_menu();
	if(!uip->menu[DM_MENU_FILE])
	{
		goto fail;
	}
	t3f_add_menu_item(uip->menu[DM_MENU_FILE], "New", 0, NULL, dm_menu_file_new, NULL);
	t3f_add_menu_item(uip->menu[DM_MENU_FILE], "Open", 0, NULL, dm_menu_file_open, NULL);
	t3f_add_menu_item(uip->menu[DM_MENU_FILE], "Save", 0, NULL, dm_menu_file_save, NULL);
	#ifndef ALLEGRO_MACOSX
		t3f_add_menu_item(uip->menu[DM_MENU_FILE], NULL, 0, NULL, dm_menu_file_save, NULL);
		t3f_add_menu_item(uip->menu[DM_MENU_FILE], "Quit", 0, NULL, dm_menu_file_quit, NULL);
	#endif

	uip->menu[DM_MENU_EDIT] = al_create_menu();
	if(!uip->menu[DM_MENU_EDIT])
	{
		goto fail;
	}
	t3f_add_menu_item(uip->menu[DM_MENU_EDIT], "Add Text", 0, NULL, dm_menu_edit_add_text, NULL);
	t3f_add_menu_item(uip->menu[DM_MENU_EDIT], "Add Image", 0, NULL, dm_menu_edit_add_image, NULL);
	t3f_add_menu_item(uip->menu[DM_MENU_EDIT], "Delete", 0, NULL, dm_menu_edit_delete, NULL);

	uip->menu[DM_MENU_MAIN] = al_create_menu();
	if(!uip->menu[DM_MENU_MAIN])
	{
		goto fail;
	}
	t3f_add_menu_item(uip->menu[DM_MENU_MAIN], "File", 0, uip->menu[DM_MENU_FILE], NULL, NULL);
	t3f_add_menu_item(uip->menu[DM_MENU_MAIN], "Edit", 0, uip->menu[DM_MENU_EDIT], NULL, NULL);
	return true;

	fail:
	{
		dm_destroy_menus(uip);
		return false;
	}
}

void dm_destroy_menus(DM_UI * uip)
{
	int i;

	for(i = 0; i < DM_MAX_MENUS; i++)
	{
		if(uip->menu[i])
		{
			al_destroy_menu(uip->menu[i]);
		}
	}
}
