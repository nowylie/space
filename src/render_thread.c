#include "render_thread.h"

#include <cairo.h>
#include <cairo-xlib.h>

#include <unistd.h>

void* render_function(void* param)
{
	server_t *server = (server_t*) param;
	cairo_t *context;
	
	printf("[render] thread started.\n");
	
	context = cairo_create(server->output);
	
	cairo_set_source_rgb(context, 0.0, 1.0, 0.0);
	cairo_paint(context);
	XFlush(server->conn);
	
	
	
	cairo_destroy(context);
	
	printf("[render] thread stopped.\n");
	
	return NULL;
}