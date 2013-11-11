#include "common/includes.h"

#include "common/uthash.h"

#include "window.h"

#ifndef _S_FRAME_H_
#define _S_FRAME_H_

typedef struct {
   Window            id;
   int               x;
   int               y;
   unsigned          width;
   unsigned          height;
   window_t*         child;
   UT_hash_handle    hh;
} frame_t;

frame_t * frame_get( Window window );

frame_t * frame_create( Display *conn, Window child );

void frame_show( Display *conn, frame_t *frame );

void frame_focus( Display *conn, frame_t *frame );

void frame_move( Display *conn, frame_t *frame, int x, int y );

void frame_fit_child( Display *conn, frame_t *frame );

void frame_destroy( Display *conn, frame_t *frame );

#endif