#include "common.h"
#include "uthash.h"

#ifndef _WINDOW_HASH_H_
#define _WINDOW_HASH_H_

typedef struct {
	Window id;
	Pixmap store;
	Picture picture;
	int width, height;
	int x, y;
	UT_hash_handle hh;
} window_t;

void add_window(Window id);
window_t *get_window(Window id);
void remove_window(window_t *w);


#endif