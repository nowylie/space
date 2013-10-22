#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/shape.h>

#include <pthread.h>

#ifndef _COMMON_H_
#define _COMMON_H_

typedef char bool;
#define FALSE  0
#define TRUE   1

typedef struct {
	Display *conn;
	Window root, overlay;
	Visual *visual;
	Picture output;
	int width, height;
} server_t;

typedef struct object object_t;

struct object {
	Window id;
	int width, height;
	int x, y;
	Pixmap back_pixmap, buff_pixmap;
	Picture back_picture, buff_picture;
	object_t *prev, *next;
};

typedef struct {
	int width, height;
	int x, y, zoom;
	object_t *centre;
	Pixmap buff_pixmap;
	Picture buff_picture;
} viewport_t;

typedef struct {
	object_t *objects;
	viewport_t *viewport;
} scene_t;

#endif