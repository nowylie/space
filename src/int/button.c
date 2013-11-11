#include "button.h"

#include <string.h>

button_t *button_map = NULL;

button_t * button_get( Window id )
{
   button_t *button;
   
   HASH_FIND(hh, button_map, &id, sizeof(Window), button);
   
   return button;
}

button_t * button_create( Display* conn, Window parent, int x, int y, unsigned width, unsigned height )
{
   XSetWindowAttributes attr;
   button_t *button = malloc( sizeof(button_t) );
   
   button->state = ACTIVE;
   
   button->width = width;
   button->height = height;
   
   /* Create the pixmap for drawing into */
   button->buffer = XCreatePixmap( conn, parent, width, height, 24 );
   
   /* Fill the pixmaps */
   
   attr.event_mask = ButtonReleaseMask |
                     EnterWindowMask |
                     LeaveWindowMask |
                     ExposureMask;
   
   button->id = XCreateWindow( conn, parent, x, y, width, height, 0, 24,
         InputOutput, CopyFromParent, CWEventMask, &attr );
   
   XMapWindow( conn, button->id );
   
   HASH_ADD(hh, button_map, id, sizeof(Window), button);
   
   return button;
}

void button_set_fills( Display *conn, ulong off, ulong on, button_t *button )
{
   XGCValues values;
   ulong mask;
   
   values.foreground = off;
   values.fill_style = FillSolid;
   values.graphics_exposures = False;
   mask = GCForeground | GCFillStyle | GCGraphicsExposures;
   
   button->off = XCreateGC( conn, button->id, mask, &values );
   
   values.foreground = on;
   button->on = XCreateGC( conn, button->id, mask, &values );
   
   button->current = button->off;
}

void button_set_text( Display *conn, ulong color, char *font, char *text, button_t *button )
{
   XFontStruct *fontstruct;
   int text_width;
   XGCValues values;
   ulong mask;
   
   fontstruct = XLoadQueryFont( conn, font );
   text_width = XTextWidth( fontstruct, text, strlen(text) );
   
   values.foreground = color;
   values.font = fontstruct->fid;
   values.graphics_exposures = False;
   mask = GCForeground | GCFont | GCGraphicsExposures;
   
   button->pen = XCreateGC( conn, button->id, mask, &values );
   
   button->text_x = (button->width - text_width) / 2;
   button->text_y = (button->height + fontstruct->ascent) / 2;
   
   button->text = text;
}


void button_render( Display* conn, button_t* button )
{
   XFillRectangle( conn, button->buffer, button->current, 0, 0,
         button->width, button->height );
         
   XDrawString( conn, button->buffer, button->pen, button->text_x,
         button->text_y, button->text, strlen(button->text) );
}


void button_enter( Display *conn, button_t *button )
{
   if ( button->state == ACTIVE ) {
      button->current = button->on;
      button_render( conn, button );
   }
}

void button_leave( Display *conn, button_t *button )
{
   if ( button->state == ACTIVE ) {
      button->current = button->off;
      button_render( conn, button );
   }
}

void button_draw( Display *conn, button_t *button )
{
   XCopyArea( conn, button->buffer, button->id, button->on, 0, 0,
         button->width, button->height, 0, 0 );
}