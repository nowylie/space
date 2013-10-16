#include <stdio.h>
#include <stdlib.h>

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

#ifndef _COMMON_H_
#define _COMMON_H_

typedef char bool;
#define FALSE  0
#define TRUE   1

typedef struct {
	Display *conn;
	Window root, overlay;
	Visual *visual;
	cairo_surface_t *output;
	int width, height;
} server_t;

#endif