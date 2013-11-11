#include "frame.h"

#include "common/properties.h"

#include "atoms.h"

frame_t *frame_map = NULL;

frame_t * frame_get( Window id )
{
   frame_t *frame;
   
   HASH_FIND(hh, frame_map, &id, sizeof(Window), frame);
   
   return frame;
}

frame_t * frame_create( Display *conn, Window child )
{
   frame_t *frame = malloc( sizeof(frame_t) );
   Window root = DefaultRootWindow( conn );
   unsigned width = DisplayWidth( conn, DefaultScreen( conn ) ) / 2;
   unsigned height = DisplayHeight( conn, DefaultScreen( conn ) ) / 2;
   unsigned long white = WhitePixel( conn, DefaultScreen( conn ) );
   unsigned long black = BlackPixel( conn, DefaultScreen( conn ) );
   
   frame->id = XCreateSimpleWindow( conn, root, 0, 0, width, height, 1, white, black );
   XSelectInput( conn, frame->id, SubstructureNotifyMask | EnterWindowMask);
   
   frame->x = 0;
   frame->y = 0;
   frame->width = width;
   frame->height = height;
   
   frame->child = window_init( conn, child );
   
   XReparentWindow( conn, child, frame->id, 0, 0 );
   XAddToSaveSet( conn, child );
   
   frame_fit_child( conn, frame );
   
   frame_show( conn, frame );
   
   HASH_ADD(hh, frame_map, id, sizeof(Window), frame);
   
   return frame;
}

void frame_show( Display *conn, frame_t *frame )
{
   if ( frame == NULL )
      return;
      
   XMapWindow( conn, frame->child->id );
   XMapWindow( conn, frame->id );
}

void frame_focus( Display *conn, frame_t *frame )
{
   property_t focus;
   
   if ( frame == NULL ) {
      printf( "[error] Cannot set focus to null frame.\n" );
      return;
   }
   
   /* First check window state. If hidden, unhide it. */
   
   focus.type = XA_WINDOW;
   focus.format = 32;
   focus.size = 1;
   focus.data = (unsigned char *) &frame->id;
   printf( "[focus] Settings active window to: %lu\n", ((ulong*)focus.data)[0] );
   
   property_set( conn, DefaultRootWindow( conn ), EA_NET_ACTIVE_WINDOW, focus );
   
   XSetInputFocus( conn, frame->id, RevertToPointerRoot, CurrentTime );
}

void frame_move( Display *conn, frame_t *frame, int x, int y )
{
   int width, height;
   XWindowChanges changes;
   
   if ( frame == NULL )
      return;
   
   width = DisplayWidth( conn, DefaultScreen( conn ) ) / 2;
   height = DisplayHeight( conn, DefaultScreen( conn ) ) / 2;
   
   if ( (x + (frame->width / 2)) > (width / 2) )
      changes.x = width / 2;
   else
      changes.x = 0;
   
   if ( (y + (frame->height / 2)) > (height / 2) )
      changes.y = height / 2;
   else
      changes.y = 0;
   
   XConfigureWindow( conn, frame->id, CWX | CWY, &changes );
}

void frame_fit_child( Display *conn, frame_t *frame )
{
   unsigned width, height;
   unsigned base_width, base_height;
   XWindowChanges values;
   unsigned value_mask;
   
   if ( frame == NULL )
      return;
   
   if ( frame->child->flags & PBaseSize ) {
      base_width = frame->child->base_width;
      base_height = frame->child->base_height;
   } else {
      base_width = frame->child->min_width;
      base_width = frame->child->min_height;
   }
   
   width = (frame->width - base_width) / frame->child->width_inc;
   height = (frame->height - base_height) / frame->child->height_inc;
   
   width = base_width + (frame->child->width_inc * width);
   height = base_height + (frame->child->height_inc * height);
   
   values.x = (frame->width / 2) - (width / 2);
   values.y = (frame->height / 2) - (height / 2);
   values.width = width;
   values.height = height;
   values.border_width = 0;
   value_mask = CWX | CWY | CWWidth | CWHeight | CWBorderWidth;
   
   XConfigureWindow( conn, frame->child->id, value_mask, &values );
}

void frame_destroy( Display *conn, frame_t *frame )
{
   Window id = frame->id;
   window_destroy( frame->child );
   
   HASH_DELETE( hh, frame_map, frame );
   
   XDestroyWindow( conn, id );
   
   free( frame );
}