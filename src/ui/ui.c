#include "t3f/t3f.h"
#include "ui.h"
#include "menu_init.h"

DM_UI * dm_create_ui(void * data)
{
	DM_UI * uip;

	uip = malloc(sizeof(DM_UI));
	if(!uip)
	{
		goto fail;
	}
	memset(uip, 0, sizeof(DM_UI));
	if(!dm_setup_menus(uip))
	{
		goto fail;
	}
	if(!t3f_attach_menu(uip->menu[DM_MENU_MAIN]))
	{
		goto fail;
	}
	return uip;

	fail:
	{
		dm_destroy_ui(uip);
		return NULL;
	}
}

void dm_destroy_ui(DM_UI * uip)
{
	if(uip)
	{
		dm_destroy_menus(uip);
		free(uip);
	}
}

void dm_process_ui(DM_UI * uip)
{

}

void dm_render_ui(DM_UI * uip)
{

}
