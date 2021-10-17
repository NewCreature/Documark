#ifndef DM_DOCUMENT_H
#define DM_DOCUMENT_H

#include "t3f/t3f.h"

#define DM_MAX_DOCUMENT_ELEMENTS 64

#define DM_ELEMENT_IMAGE          0
#define DM_ELEMENT_TEXT           1

typedef struct
{

	int type;
	void * data;
	char * data_fn;
	void * support_data;
	char * support_data_fn;
	float x, y;
	float width, height;
	int size;

} DM_DOCUMENT_ELEMENT;

typedef struct
{

	ALLEGRO_BITMAP * original;
	DM_DOCUMENT_ELEMENT element[DM_MAX_DOCUMENT_ELEMENTS];
	int element_count;

} DM_DOCUMENT;

DM_DOCUMENT * dm_create_document(const char * image_fn);
DM_DOCUMENT * dm_load_document(const char * document_fn);
void dm_destroy_document(DM_DOCUMENT * dp);

bool dm_add_image_to_document(DM_DOCUMENT * dp, const char * image_fn, int x, int y);
bool dm_add_text_to_document(DM_DOCUMENT * dp, const char * font_fn, const char * text, int size, int x, int y);
void dm_remove_element_from_document(DM_DOCUMENT * dp, int i);

void dm_render_document(DM_DOCUMENT * dp, float x, float y, float scale);

#endif
