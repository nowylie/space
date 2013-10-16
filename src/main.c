#include "common.h"

#include <X11/extensions/Xcomposite.h>

#include <pthread.h>

#include "render_thread.h"
#include "event_thread.h"

void process_opts(int, char**);

int main(int argc, char** argv)
{
	server_t server;
	pthread_t event_thread, render_thread;
	XWindowAttributes attribs;
	
	process_opts(argc, argv);
	
	if (XInitThreads() == FALSE) {
		fprintf(stderr, "[main] Server does not support threads.\n");
		exit(EXIT_FAILURE);
	}
	
	server.conn = XOpenDisplay(NULL);
	if (server.conn == NULL) {
		fprintf(stderr, "[main] Could not connect to server.\n");
		exit(EXIT_FAILURE);
	}
	
	server.root = DefaultRootWindow(server.conn);
	XGetWindowAttributes(server.conn, server.root, &attribs);
	
	server.overlay = XCompositeGetOverlayWindow(server.conn, server.root);
	
	server.visual = attribs.visual;
	server.width = attribs.width;
	server.height = attribs.height;
	
	server.output = cairo_xlib_surface_create(server.conn, server.overlay,
			server.visual, server.width, server.height);
	
	printf("[main] Connection opened to server.\n");
	printf("[main] Root window [%lu] found.\n", server.root);
	printf("[main] Overlay window [%lu] found.\n", server.overlay);
	printf("\t screen size: %dx%d\n", server.width, server.height);
	
	XCompositeRedirectSubwindows(server.conn, server.root, CompositeRedirectManual);
	
	pthread_create(&render_thread, NULL, &render_function, &server);
	pthread_create(&event_thread, NULL, &event_function, &server);
	
	pthread_join(render_thread, NULL);
	pthread_join(event_thread, NULL);
	
	XCloseDisplay(server.conn);
	
	return EXIT_SUCCESS;
}

void process_opts(int argc, char** argv)
{
	
}