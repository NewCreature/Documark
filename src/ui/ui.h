#ifndef DM_UI_H
#define DM_UI_H

#include "t3f/t3f.h"
#include "menu.h"

#define DM_MAX_MENUS 8

#define DM_MENU_MAIN 0
#define DM_MENU_FILE 1
#define DM_MENU_EDIT 2

typedef struct
{

	ALLEGRO_MENU * menu[DM_MAX_MENUS];

} DM_UI;

DM_UI * dm_create_ui(void * data);
void dm_destroy_ui(DM_UI * uip);

void dm_process_ui(DM_UI * uip);
void dm_render_ui(DM_UI * uip);

#endif
