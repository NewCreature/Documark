#ifndef DM_INSTANCE_H
#define DM_INSTANCE_H

#include "document.h"
#include "ui/ui.h"

typedef struct
{

	ALLEGRO_BITMAP * bg_tile;
	DM_DOCUMENT * document;
	float view_x, view_y, view_zoom;
	DM_UI * ui;

} APP_INSTANCE;


#endif
