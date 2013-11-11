#include "window.h"

#include "common/properties.h"

#include "atoms.h"

window_t * window_init( Display *conn, Window window )
{
   window_t *w_return;
   property_t p_value;
   int length;
   XSizeHints hints;
   long flags;
   
   w_return = malloc( sizeof(window_t) );
   
   w_return->id = window;
   
   /* Set name of window */
   length = property_size( conn, window, EA_NET_WM_NAME );
   if ( length != 0) {
      p_value = property_get( conn, window, EA_NET_WM_NAME, length );
   } else {
      length = property_size( conn, window, XA_WM_NAME );
      p_value = property_get( conn, window, XA_WM_NAME, length );
   }
   
   w_return->name = malloc( p_value.size );
   snprintf( w_return->name, p_value.size, "%s", p_value.data );
   
   /* Set name of icon */
   length = property_size( conn, window, EA_NET_WM_ICON_NAME );
   if ( length != 0) {
      p_value = property_get( conn, window, EA_NET_WM_ICON_NAME, length );
   } else {
      length = property_size( conn, window, XA_WM_ICON_NAME );
      p_value = property_get( conn, window, XA_WM_ICON_NAME, length );
   }
   
   w_return->icon_name = malloc( p_value.size );
   snprintf( w_return->icon_name, p_value.size, "%s", p_value.data );
   
   XGetWMNormalHints( conn, window, &hints, &w_return->flags );
   w_return->min_width = hints.min_width;
   w_return->min_height = hints.min_height;
   w_return->max_width = hints.max_width;
   w_return->max_height = hints.max_height;
   w_return->width_inc = hints.width_inc;
   w_return->height_inc = hints.height_inc;
   w_return->base_width = hints.base_width;
   w_return->base_height = hints.base_height;
   
   return w_return;
}

void window_activate( Display *conn, Window window )
{
   property_t active;
   
   active.type = XA_WINDOW;
   active.format = 32;
   active.size = 1;
   active.data = (unsigned char *) &window;
   
   property_set( conn, DefaultRootWindow( conn ), EA_NET_ACTIVE_WINDOW, active );
}

void window_destroy( window_t *window )
{
   free( window->name );
   free( window->icon_name );
   free( window );
}

int window_get_state( Display *conn, Window window )
{
   property_t value = property_get( conn, window, XA_WM_STATE, 1 );
   int state = 0;
   
   if ( value.type != None )
      state = ((int*)value.data)[0];
   
   return state;
}