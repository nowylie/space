#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/shape.h>

#include "uthash.h"

#ifndef _COMMON_H_
#define _COMMON_H_

typedef char bool;
#define FALSE  0
#define TRUE   1

extern XRenderColor green;
extern XRenderColor blue;
extern XRenderColor black;

typedef struct window_def window_t;

struct window_def {
	Window id;
	int x, y;
	unsigned width, height;
	Pixmap pixmap;
	Picture picture;
	bool mapped;
	bool dirty;
	window_t *prev, *next;
	UT_hash_handle hh;
};

typedef struct object object_t;

struct object {
	Window id;
	int x, y;
	unsigned width, height;
	Pixmap back_pixmap, buff_pixmap;
	Picture back_picture, buff_picture;
	Damage damage;
	object_t *prev, *next;
	UT_hash_handle hh;
};

typedef struct {
	int x, y;
	unsigned width, height;
	int lwidth, lheight;
	float zoom;
	object_t *start;
	Pixmap buff_pixmap;
	Picture buff_picture;
} viewport_t;

typedef struct {
	object_t *objects;
	viewport_t *viewport;
	bool dirty;
} scene_t;

extern Display *conn;
extern Visual *visual;
extern unsigned width, height, depth;
extern Picture output;

#endif