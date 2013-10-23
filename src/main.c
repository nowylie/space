#include "common.h"

#include "event_thread.h"
#include "render_thread.h"

#include "scene.h"
#include "viewport.h"

XRenderColor green = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0xffff, .blue = 0x0000};
XRenderColor blue = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0x0000, .blue = 0xffff};

Display *conn = NULL;
Visual *visual = NULL;
unsigned width, height, depth;
Picture output = None;
scene_t *scene = NULL;

pthread_mutex_t scene_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t scene_dirty = PTHREAD_COND_INITIALIZER;

void process_opts(int, char**);

void allow_input_passthrough(Window overlay);

int main(int argc, char** argv)
{
	pthread_t event_thread, render_thread;
	Window root, overlay;
	XWindowAttributes attr;
	XRenderPictFormat *format;
	XRenderPictureAttributes pa;
	
	process_opts(argc, argv);
	
	if (XInitThreads() == FALSE) {
		fprintf(stderr, "[main] Server does not support threads.\n");
		exit(EXIT_FAILURE);
	}
	
	conn = XOpenDisplay(NULL);
	if (conn == NULL) {
		fprintf(stderr, "[main] Could not connect to server.\n");
		exit(EXIT_FAILURE);
	}
	
	visual = DefaultVisual(conn, DefaultScreen(conn));
	
	root = DefaultRootWindow(conn);
	XGetWindowAttributes(conn, root, &attr);
	
	width = attr.width;
	height = attr.height;
	depth = attr.depth;
	printf("Depth: %d\n", depth);
	
	overlay = XCompositeGetOverlayWindow(conn, root);
	
	allow_input_passthrough(overlay);
	
	scene = scene_create();
	
	format = XRenderFindVisualFormat(conn, attr.visual);
	pa.subwindow_mode = IncludeInferiors;

	output = XRenderCreatePicture(conn, overlay, format,
			CPSubwindowMode, &pa);
	
	printf("[main] Connection opened to server.\n");
	printf("[main] Root window [%lu] found.\n", root);
	printf("[main] Overlay window [%lu] found.\n", overlay);
	printf("\t screen size: %dx%d\n", attr.width, attr.height);
	
	XCompositeRedirectSubwindows(conn, root, CompositeRedirectManual);
	
	pthread_create(&render_thread, NULL, &render_function, NULL);
	pthread_create(&event_thread, NULL, &event_function, NULL);
	
	pthread_join(render_thread, NULL);
	pthread_join(event_thread, NULL);
	
	XCloseDisplay(conn);
	
	return EXIT_SUCCESS;
}

void process_opts(int argc, char** argv)
{
	
}

void allow_input_passthrough(Window overlay)
{
	XserverRegion region = XFixesCreateRegion(conn, NULL, 0);
	
	XFixesSetWindowShapeRegion(conn, overlay, ShapeBounding, 0, 0, 0);
	XFixesSetWindowShapeRegion(conn, overlay, ShapeInput, 0, 0, region);
	
	XFixesDestroyRegion(conn, region);
}