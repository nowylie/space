#include "window_hash.h"

window_t *windows = NULL;

void add_window(Window id)
{
	window_t *w = malloc(sizeof(window_t));
	
	w->id = id;
	w->store = None;
	w->width = 0;
	w->height = 0;
	w->x = 0;
	w->y = 0;
	
	HASH_ADD(hh, windows, id, sizeof(Window), w);
}

window_t *get_window(Window id)
{
	window_t *w;
	
	HASH_FIND(hh, windows, &id, sizeof(Window), w);
	
	return w;
}

void remove_window(window_t *w)
{
	HASH_DELETE(hh, windows, w);
}
