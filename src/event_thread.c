#include "event_thread.h"
#include "window_hash.h"
#include "scene.h"

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
			add_window(ev.xcreatewindow.window);
		} else if (ev.type == DestroyNotify) {
			window_t *w;
			
			w = get_window(ev.xdestroywindow.window);
			
			if (w->store != None)
				XFreePixmap(server->conn, w->store);
			
			remove_window(w);
		} else if (ev.type == MapNotify) {
			XRenderPictFormat *format;
			XRenderPictureAttributes pa;
			XWindowAttributes attr;
			scene_obj *obj;
			window_t *w;
			
			
			w = get_window(ev.xmap.window);
			
			w->store = XCompositeNameWindowPixmap(server->conn,
				ev.xmap.window);
			
			XGetWindowAttributes(server->conn, ev.xmap.window,
				&attr);
			
			w->x = attr.x;
			w->y = attr.y;
			w->width = attr.width;
			w->height = attr.height;
			
			format = XRenderFindVisualFormat(server->conn, attr.visual);
			pa.subwindow_mode = IncludeInferiors;

			w->picture = XRenderCreatePicture(server->conn, w->store,
				format, CPSubwindowMode, &pa);
			
			/* Add the window to the rendering space */
			obj = malloc(sizeof(scene_obj));
			
			obj->id = ev.xmap.window;
			obj->edge = attr.width / 2;
			obj->centre = attr.x + obj->edge;
			
			scene_add_object(obj);
		} else if (ev.type == UnmapNotify) {
			scene_obj *obj;
			window_t *w;
			
			w = get_window(ev.xunmap.window);
			
			XRenderFreePicture(server->conn, w->picture);
			XFreePixmap(server->conn, w->store);

			w->store = None;
			
			/* Remove the window from the rendering space */
			obj = scene_drop_object(ev.xunmap.window);
			free(obj);
		}
		printf("[event] Event of type %d received.\n", ev.type);
	}
	
	printf("[event] thread stopped.\n");
	
	return NULL;
}