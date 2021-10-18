#ifndef DM_MENU_PROC_H
#define DM_MENU_PROC_H

int dm_menu_file_new(int id, void * data);
int dm_menu_file_open(int id, void * data);
int dm_menu_file_save(int id, void * data);
int dm_menu_file_quit(int id, void * data);

int dm_menu_edit_add_text(int id, void * data);
int dm_menu_edit_add_image(int id, void * data);
int dm_menu_edit_delete(int id, void * data);

#endif
