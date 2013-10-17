#include "window_hash.h"

int main()
{
	Window root, overlay;
	window_t *w;
	Display *dsp = XOpenDisplay(NULL);
	if (dsp == NULL)
		exit(EXIT_FAILURE);
	
	root = DefaultRootWindow(dsp);
	
	printf("Root window at: %lu\n", root);
	
	add_window(root);
	
	w = get_window(root);
	
	printf("Root window structure %p has id %lu\n", w, w->id);
	
	remove_window(w);
	
	return EXIT_SUCCESS;
}