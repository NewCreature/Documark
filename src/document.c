#include "t3f/t3f.h"
#include "t3f/file.h"
#include "document.h"

DM_DOCUMENT * dm_create_document(const char * image_fn)
{
	DM_DOCUMENT * dp;

	dp = malloc(sizeof(DM_DOCUMENT));
	if(!dp)
	{
		goto fail;
	}
	memset(dp, 0, sizeof(DM_DOCUMENT));
	if(image_fn)
	{
		dp->original = al_load_bitmap_flags(image_fn, ALLEGRO_NO_PREMULTIPLIED_ALPHA);
		if(!dp->original)
		{
			goto fail;
		}
	}
	return dp;

	fail:
	{
		dm_destroy_document(dp);
		return NULL;
	}
}

static bool load_element(ALLEGRO_FILE * fp, DM_DOCUMENT_ELEMENT * dep)
{
	dep->type = al_fgetc(fp);
	switch(dep->type)
	{
		case DM_ELEMENT_IMAGE:
		{
			dep->data = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
			dep->x = t3f_fread_float(fp);
			dep->y = t3f_fread_float(fp);
			dep->width = t3f_fread_float(fp);
			dep->height = t3f_fread_float(fp);
			return true;
		}
		case DM_ELEMENT_TEXT:
		{
			dep->data = t3f_load_string_f(fp);
			dep->support_data_fn = t3f_load_string_f(fp);
			dep->x = t3f_fread_float(fp);
			dep->y = t3f_fread_float(fp);
			dep->size = al_fread32le(fp);
			dep->support_data = al_load_font(dep->support_data_fn, dep->size, 0);
			return true;
		}
	}
	return false;
}

DM_DOCUMENT * dm_load_document(const char * document_fn)
{
	DM_DOCUMENT * dp;
	ALLEGRO_FILE * fp = NULL;
	char header[4];
	int i;

	dp = dm_create_document(NULL);
	if(!dp)
	{
		goto fail;
	}
	fp = al_fopen(document_fn, "rb");
	if(!fp)
	{
		goto fail;
	}
	al_fread(fp, header, 4);
	if(header[0] != 'D' || header[1] != 'M')
	{
		goto fail;
	}
	dp->original = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!dp->original)
	{
		goto fail;
	}
	dp->element_count = al_fread32le(fp);
	for(i = 0; i < dp->element_count; i++)
	{
		if(!load_element(fp, &dp->element[i]))
		{
			goto fail;
		}
	}
	return dp;

	fail:
	{
		if(fp)
		{
			al_fclose(fp);
		}
		dm_destroy_document(dp);
		return NULL;
	}
}

static void free_element(DM_DOCUMENT_ELEMENT * dep)
{
	switch(dep->type)
	{
		case DM_ELEMENT_IMAGE:
		{
			al_destroy_bitmap(dep->data);
			break;
		}
		case DM_ELEMENT_TEXT:
		{
			free(dep->data);
			al_destroy_font(dep->support_data);
			free(dep->support_data_fn);
		}
	}
}

void dm_destroy_document(DM_DOCUMENT * dp)
{
	int i;

	if(dp)
	{
		for(i = 0; i < dp->element_count; i++)
		{
			free_element(&dp->element[i]);
		}
		if(dp->original)
		{
			al_destroy_bitmap(dp->original);
		}
		free(dp);
	}
}

bool dm_add_image_to_document(DM_DOCUMENT * dp, const char * image_fn, int x, int y)
{
	if(dp->element_count < DM_MAX_DOCUMENT_ELEMENTS)
	{
		dp->element[dp->element_count].data = al_load_bitmap_flags(image_fn, ALLEGRO_NO_PREMULTIPLIED_ALPHA);
		if(!dp->element[dp->element_count].data)
		{
			return false;
		}
		dp->element[dp->element_count].x = x;
		dp->element[dp->element_count].y = y;
		dp->element[dp->element_count].width = al_get_bitmap_width(dp->element[dp->element_count].data);
		dp->element[dp->element_count].height = al_get_bitmap_height(dp->element[dp->element_count].data);
		dp->element_count++;
	}
	return false;
}

bool dm_add_text_to_document(DM_DOCUMENT * dp, const char * font_fn, const char * text, int size, int x, int y)
{
	if(dp->element_count < DM_MAX_DOCUMENT_ELEMENTS)
	{
		dp->element[dp->element_count].data = strdup(text);
		if(!dp->element[dp->element_count].data)
		{
			goto fail;
		}
		dp->element[dp->element_count].support_data_fn = strdup(font_fn);
		if(!dp->element[dp->element_count].support_data_fn)
		{
			goto fail;
		}
		dp->element[dp->element_count].support_data = al_load_font(font_fn, size, 0);
		if(!dp->element[dp->element_count].support_data)
		{
			goto fail;
		}
		dp->element[dp->element_count].size = size;
		dp->element[dp->element_count].x = x;
		dp->element[dp->element_count].y = y;
		dp->element_count++;
		return true;
	}
	fail:
	{
		if(dp->element[dp->element_count].support_data)
		{
			al_destroy_font(dp->element[dp->element_count].support_data);
		}
		if(dp->element[dp->element_count].support_data_fn)
		{
			free(dp->element[dp->element_count].support_data_fn);
		}
		if(dp->element[dp->element_count].data)
		{
			free(dp->element[dp->element_count].data);
		}
		return false;
	}
}

void dm_remove_element_from_document(DM_DOCUMENT * dp, int i)
{
	int j;

	if(i < dp->element_count)
	{
		free_element(&dp->element[i]);
		for(j = i; j < dp->element_count - 1; j++)
		{
			memcpy(&dp->element[j], &dp->element[j + 1], sizeof(DM_DOCUMENT_ELEMENT));
		}
	}
}

static void render_element(DM_DOCUMENT_ELEMENT * dep, float x, float y)
{
	switch(dep->type)
	{
		case DM_ELEMENT_IMAGE:
		{
			if(dep->data)
			{
				al_draw_scaled_bitmap(dep->data, 0, 0, al_get_bitmap_width(dep->data), al_get_bitmap_height(dep->data), dep->x + x, dep->y + y, dep->width, dep->height, 0);
			}
			break;
		}
		case DM_ELEMENT_TEXT:
		{
			if(dep->data && dep->support_data)
			{
				al_draw_text(dep->support_data, t3f_color_black, dep->x + x, dep->y + y, 0, dep->data);
			}
			break;
		}
	}
}

void dm_render_document(DM_DOCUMENT * dp, float x, float y, float scale)
{
	int i;

	if(dp->original)
	{
		al_draw_bitmap(dp->original, x, y, 0);
	}
	for(i = 0; i < dp->element_count; i++)
	{
		render_element(&dp->element[i], x, y);
	}
}
