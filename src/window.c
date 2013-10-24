#include "window.h"

window_t *window_map = NULL;
window_t *window_list = NULL;
window_t *window_list_last = NULL;

/* Retrieve a pointer to the window structure for specified id */
window_t *window_get(Window id)
{
	window_t *window;
	
	HASH_FIND(hh, window_map, &id, sizeof(Window), window);
	
	return window;
}

/* Remove the window from the list and return a pointer to it */
window_t *window_list_remove(Window id)
{
	window_t *window = window_get(id);
	
	if (window_list == window)
		window_list = window->next;
	
	if (window_list_last == window)
		window_list_last = window->prev;
	
	if (window->prev != NULL) {
		/* Fix the previous node in the list so that it does
			not point to an unallocated structure. */
		window->prev->next = window->next;
	}
	
	if (window->next != NULL) {
		/* Again, fix the list so that the next node does not
			point to an	unallocated structure. */
		window->next->prev = window->prev;
	}
	
	window->prev = NULL;
	window->next = NULL;
	
	return window;
}

void window_list_prepend(Window id)
{
	window_t *window = window_get(id);
	
	window->prev = NULL;
	window->next = window_list;
	
	if (window_list != NULL)
		window_list->prev = window;
	else
		window_list_last = window;
	window_list = window;
}

void window_list_append(Window id)
{
	window_t *window = window_get(id);
	
	window->prev = window_list_last;
	window->next = NULL;
	
	if (window_list_last != NULL)
		window_list_last->next = window;
	else
		window_list = window;
	window_list_last = window;
}

void window_list_move_before(Window id, Window sibling)
{
	window_t *window, *after;
	
	window = window_list_remove(id);
	after = window_get(sibling);
	
	window->prev = after->prev;
	window->next = after;
	
	if (after->prev != NULL)
		after->prev->next = window;
	
	after->prev = window;
}

void window_list_move_after(Window id, Window sibling)
{
	window_t *window, *before;
	
	window = window_list_remove(id);
	before = window_get(sibling);
	
	window->prev = before;
	window->next = before->next;
	
	if (before->next != NULL)
		before->next->prev = window;
	
	before->next = window;
}

void window_list_populate()
{
	Window root, parent, *children;
	unsigned num_children;
	int i;
	
	XQueryTree(conn, DefaultRootWindow(conn), &root, &parent, &children, &num_children);
	
	for (i = num_children - 1; i >= 0; i--) {
		window_create(children[i]);
	}
	
	if (children != NULL)
		XFree(children);
}

window_t *window_list_get()
{
	return window_list;
}

/* Allocate memory for the window and add it to the hash map */
void window_create(Window id)
{
	window_t *window = malloc(sizeof(window_t));
	
	window->id = id;
	window->mapped = FALSE;
	window->dirty = TRUE;
	
	HASH_ADD(hh, window_map, id, sizeof(Window), window);
	
	window->prev = NULL;
	window->next = NULL;
	
	/* need to insert the window into the window list. Going to just
		add it to the start of the list for convenience. */
	window_list_append(id);
}

void window_destroy(Window id)
{
	window_t *window;
	
	window = window_list_remove(id);
	
	HASH_DELETE(hh, window_map, window);
	
	free(window);
}

void window_configure(XConfigureEvent event)
{
	window_t *window = window_get(event.window);
	
	if (window == NULL)
		return;
	
	window->x = event.x;
	window->y = event.y;
	window->width = event.width;
	window->height = event.height;
	
	if (event.above != None) {
		window_list_remove(event.window);
		window_list_append(event.window);
	}
}

void window_set_mapped(Window id, bool mapped)
{
	window_t *window = window_get(id);
	
	if (window == NULL)
		return;
	
	window->mapped = mapped;
	
	if (mapped == TRUE) {
		XRenderPictFormat *format;
		XRenderPictureAttributes pa;
		
		format = XRenderFindVisualFormat(conn, visual);
		pa.subwindow_mode = IncludeInferiors;
		
		window->pixmap = XCompositeNameWindowPixmap(conn, id);
		window->picture = XRenderCreatePicture(conn, window->pixmap,
				format, CPSubwindowMode, &pa);
		XDamageCreate(conn, window->pixmap, XDamageReportNonEmpty);
		
		window_list_remove(id);
		window_list_append(id);
	} else {
		/*XRenderFreePicture(conn, window->picture);*/
		/*XFreePixmap(conn, window->pixmap);*/
	}
}

void window_damaged(Window id)
{
	window_t *window = window_get(id);
	
	if (window == NULL)
		return;
	
	window->dirty = TRUE;
}

void window_draw(Window id, Picture surface)
{
	window_t *window = window_get(id);
	
	if (window == NULL)
		return;
	
	XRenderComposite(conn, PictOpSrc, window->picture, None,	surface, 0, 0,
			0, 0, window->x, window->y, window->width, window->height);
}


void window_list_draw(Picture surface)
{
	window_t *window = window_list;
	
	while (window != NULL)
	{
		if (window->mapped == TRUE) {
			window_draw(window->id, surface);
		}
		
		window = window->next;
	}
}