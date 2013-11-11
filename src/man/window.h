#include "common/includes.h"

#ifndef _S_WINDOW_H_
#define _S_WINDOW_H_

typedef struct {
   Window   id;
   char*    name;
   char*    icon_name;
   long     flags;
   int      min_width;
   int      min_height;
   int      max_width;
   int      max_height;
   int      width_inc;
   int      height_inc;
   int      base_width;
   int      base_height;
} window_t;

window_t * window_init( Display *conn, Window window );

void window_activate( Display *conn, Window window );

void window_destroy( window_t *window );

int window_get_state( Display *conn, Window window );

#endif