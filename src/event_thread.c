#include "event_thread.h"
#include "window_hash.h"

#include <X11/extensions/Xcomposite.h>

void* event_function(void* param)
{
	server_t *server = (server_t*)param;
	XEvent ev;
	
	printf("[event] thread started.\n");
	
	XSelectInput(server->conn, server->root, SubstructureNotifyMask);

	while (1)
	{
		XNextEvent(server->conn, &ev);
		if (ev.type == CreateNotify) {
			add_window(ev.xcreatewindow.window, ev.xcreatewindow.width,
				ev.xcreatewindow.height);
		} else if (ev.type == DestroyNotify) {
			window_t *w;
			
			w = get_window(ev.xdestroywindow.window);
			
			if (w->surface != NULL)
				cairo_surface_destroy(w->surface);
			
			if (w->store != None)
				XFreePixmap(server->conn, w->store);
			
			remove_window(w);
		} else if (ev.type == MapNotify) {
			XWindowAttributes attr;
			window_t *w;
			
			w = get_window(ev.xmap.window);
			
			w->store = XCompositeNameWindowPixmap(server->conn,
				ev.xmap.window);
			
			XGetWindowAttributes(server->conn, ev.xmap.window,
				&attr);
			
			w->surface = cairo_xlib_surface_create(server->conn,
				w->store, attr.visual, w->width, w->height);
			
			/* Add the window to the rendering space */
		} else if (ev.type == UnmapNotify) {
			window_t *w;
			
			w = get_window(ev.xunmap.window);
			
			cairo_surface_destroy(w->surface);
			XFreePixmap(server->conn, w->store);
			
			w->surface = NULL;
			w->store = None;
		}
		
		printf("[event] Event of type %d received.\n", ev.type);
	}
	
	printf("[event] thread stopped.\n");
	
	return NULL;
}