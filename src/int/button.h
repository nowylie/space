#include "common/includes.h"
#include "common/uthash.h"

#ifndef _S_BUTTON_H_
#define _S_BUTTON_H_

#define INACTIVE 0
#define ACTIVE 1


typedef struct {
   Window id;
   int state;
   GC off, on, pen, current;
   int text_x, text_y;
   char *text;
   Pixmap buffer;
   unsigned width, height;
   UT_hash_handle hh;
} button_t;

button_t *button_get( Window id );

button_t * button_create(
   Display*    conn,
   Window      parent,
   int         x,
   int         y,
   unsigned    width,
   unsigned    height
);

void button_set_fills(
   Display*    conn,
   ulong       off,
   ulong       on,
   button_t*   button
);
void button_set_text(
   Display*       conn,
   ulong          color,
   char*          font,
   char*          text,
   button_t*      button
);

void button_render(
   Display*    conn,
   button_t*   button
);

void button_enter( Display *conn, button_t *button );
void button_leave( Display *conn, button_t *button );

void button_draw( Display *conn, button_t *button );

#endif