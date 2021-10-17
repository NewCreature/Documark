#ifndef DM_INSTANCE_H
#define DM_INSTANCE_H

#include "document.h"

typedef struct
{

	ALLEGRO_BITMAP * bg_tile;
	DM_DOCUMENT * document;
	float view_x, view_y, view_zoom;

} APP_INSTANCE;


#endif
