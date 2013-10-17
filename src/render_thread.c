#include "render_thread.h"
#include "window_hash.h"
#include "scene.h"

#include <time.h>

XRenderColor green = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0xffff, .blue = 0x0000};
XRenderColor blue = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0x0000, .blue = 0xffff};


void* render_function(void* param)
{
	server_t *server = (server_t*) param;
	
	printf("[render] thread started.\n");
	
	XRenderFillRectangle(server->conn, PictOpSrc, server->output,
		&green, 0, 0, server->width, server->height);
					
	while (1) {
		scene_obj *current = scene->hlist;
		
		while (current != NULL) {
			window_t *w = get_window(current->id);
			
			if (w->store != None) {
				XRenderFillRectangle(server->conn, PictOpSrc, server->output,
					&blue, w->x, w->y, w->width, w->height);
			}
			current = current->next;
		}
		
	}
		
	printf("[render] thread stopped.\n");
	
	return NULL;
}