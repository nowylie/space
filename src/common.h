#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>

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

#endif