#include "common/includes.h"
#include "common/properties.h"

#include <math.h>

#include "button.h"
#include "message.h"

#define MODE_WAIT    0
#define MODE_MOVE    1
#define MODE_RESIZE  2

typedef struct {
   int x, y;
   uint width, height;
} window_geom_t;

Window window_get_active( Display *conn, Window root );

window_geom_t window_get_geometry( Display *conn, Window window );

void window_move( Display *conn, Window window, int x, int y );

void window_close( Display *conn, Window window, Time timestamp );

ulong color_get( Display *conn, char *name )
{
   static Colormap cmap = None;
   XColor xc, xc2;
   
   if (cmap == None)
      cmap = DefaultColormap( conn, DefaultScreen( conn ) );
   
   XAllocNamedColor( conn, cmap, name, &xc, &xc2 );
   
   return xc.pixel;
}

int main()
{
   Display *conn;
   Window root, menu, overlay, active;
   Cursor cursor;
   ulong Black, White, Gray, Blue, Yellow, Green;
   XSetWindowAttributes attr;
   ulong attr_mask;
   button_t *launch, *move, *resize, *close, *pan, *cancel;
   window_geom_t geom;
   int start_x, start_y;
   int delta_x, delta_y;
   uint width, height;
   
   conn = XOpenDisplay( NULL );
   if ( conn == NULL ) {
      fprintf( stderr, "[error] Could not open connection to server.\n" );
      exit( EXIT_FAILURE );
   }
   
   root = DefaultRootWindow( conn );
   
   cursor = XCreateFontCursor( conn, 24 );
   
   Black = BlackPixel( conn , DefaultScreen( conn ) );
   White = WhitePixel( conn , DefaultScreen( conn ) );
   Gray = color_get( conn, "Gray" );
   Blue = color_get( conn, "Blue" );
   Yellow = color_get( conn, "Yellow" );
   Green = color_get( conn, "Green" );
   
   /* Create the menu window */
   attr.override_redirect = True;
   attr.cursor = cursor;
   attr_mask = CWOverrideRedirect | CWCursor;
   menu = XCreateWindow( conn, root, 200, 0, 140, 240, 1, CopyFromParent,
         InputOutput, CopyFromParent, attr_mask, &attr );
   
   /* Create the overlay window */
   attr.background_pixel = White;
   attr.border_pixel = Green;
   attr_mask = CWBackPixel | CWBorderPixel | CWOverrideRedirect | CWCursor;
   overlay = XCreateWindow( conn, root, 0, 0, 1, 1, 2, CopyFromParent,
         InputOutput, CopyFromParent, attr_mask, &attr );
   
   launch = button_create( conn, menu, 0, 0, 140, 40 );
   button_set_fills( conn, Gray, Yellow, launch );
   button_set_text( conn, Black, "fixed", "Apps", launch );
   button_render( conn, launch );
   launch->state = INACTIVE;
   
   pan = button_create( conn, menu, 0, 160, 140, 40 );
   button_set_fills( conn, Gray, Yellow, pan );
   button_set_text( conn, Black, "fixed", "Pan", pan );
   button_render( conn, pan );
   pan->state = INACTIVE;
   
   move = button_create( conn, menu, 0, 40, 140, 40 );
   button_set_fills( conn, Blue, Yellow, move );
   button_set_text( conn, Black, "fixed", "Move", move );
   button_render( conn, move );
   
   resize = button_create( conn, menu, 0, 80, 140, 40 );
   button_set_fills( conn, Blue, Yellow, resize );
   button_set_text( conn, Black, "fixed", "Resize", resize );
   button_render( conn, resize );
   
   close = button_create( conn, menu, 0, 120, 140, 40 );
   button_set_fills( conn, Blue, Yellow, close );
   button_set_text( conn, Black, "fixed", "Close", close );
   button_render( conn, close );
   
   cancel = button_create( conn, menu, 0, 200, 140, 40 );
   button_set_fills( conn, Blue, Yellow, cancel );
   button_set_text( conn, Black, "fixed", "Cancel", cancel );
   button_render( conn, cancel );
   
   cursor = XCreateFontCursor( conn, 0 );
   XGrabButton( conn, 3, AnyModifier, root, True, ButtonPressMask, GrabModeAsync,
         GrabModeAsync, root, cursor );
   
   
   width = DisplayWidth( conn, DefaultScreen( conn ) );
   height = DisplayHeight( conn, DefaultScreen( conn ) );
   
   XEvent event;
   
   while ( 1 ) {
      button_t *button;
      XNextEvent( conn, &event );
      printf("[int] Event received: %d ", event.type );
         
      if ( event.type == ButtonPress) {
         if ( event.xbutton.button == 3 && event.xbutton.state & Mod1Mask  ) {
            /* Open the menu up */
            int x = event.xbutton.x - 70;
            int y = event.xbutton.y - 120;
            printf( "\t(ButtonPress)\n" );
            
            /* Keep x inside the screen */
            if (x < 0)
               x = 0; 
            else if (x > width - 140)
               x = width - 140;
            /* Keep y inside the screen */
            if (y < 0)
               y = 0;
            else if (y > height - 240) 
               y = height - 240;
               
            XMoveWindow( conn, menu, x, y );
            XRaiseWindow( conn, menu );
            XMapWindow( conn, menu );
            XSelectInput( conn, root, ButtonReleaseMask );
         } else if ( event.xbutton.button == 1 ) {
            /* Start the move */
            start_x = event.xbutton.x_root;
            start_y = event.xbutton.y_root;
            delta_x = 0;
            delta_y = 0;
         }
      } else if ( event.type == ButtonRelease ) {
         printf( "\t(ButtonRelease)\n" );
         button = button_get( event.xbutton.window );
         
         /* Process button release here */
         if ( event.xbutton.button == 3 ) {
            /* The end of menu item selection*/
            printf( "\tbutton: '%s' pushed\n", button->text );
            active = window_get_active( conn, root );
               
            if ( button != NULL && active != None) {
               if ( button == move ) {
                  uint event_mask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
                     
                  geom = window_get_geometry( conn, active );
                  XMoveResizeWindow( conn, overlay, geom.x, geom.y,
                           geom.width, geom.height );
                     
                  XRaiseWindow( conn, overlay );
                  XMapWindow( conn, overlay );
                     
                  XSelectInput( conn, active, StructureNotifyMask );
                     
                  XGrabPointer( conn, overlay, True, event_mask, GrabModeAsync,
                           GrabModeAsync, None, None, CurrentTime );
               } else if ( button == close ) {
                  window_close( conn, active, event.xbutton.time );
               }
            } else {
               printf( "\tno active window...aborting.\n" );
            }
            /* Hide the menu and stop listening for button release on root */
            XSelectInput( conn, root, NoEventMask );
            XUnmapWindow( conn, menu );
         } else if ( event.xbutton.button == 1 ) {
            /* Maybe end of a move or resize event */
            int xdiff, ydiff;
            xdiff = event.xbutton.x_root - start_x;
            ydiff = event.xbutton.y_root - start_y;
               
            if ( delta_x < 3 && delta_y < 3) {
               XSelectInput( conn, active, NoEventMask );
               XUngrabPointer( conn, CurrentTime );
               XUnmapWindow( conn, overlay );
            } else {
               geom = window_get_geometry( conn, overlay );
            }
         }
      } else if ( event.type == MotionNotify ) {
         printf( "\t(MotionNotify)\n" );
         if ( event.xmotion.state & Button1Mask ) {
            /* Move the window around */
            int xdiff, ydiff;
            while ( XCheckTypedEvent( conn, MotionNotify, &event ) );
            xdiff = event.xbutton.x_root - start_x;
            ydiff = event.xbutton.y_root - start_y;
            
            if ( abs(xdiff) > 2 || abs(ydiff) > 2 ) {
               int new_x = geom.x + xdiff ;
               int new_y = geom.y + ydiff;
               /* Keep x inside display */
               if ( new_x < 0 )
                  new_x = 0;
               else if ( new_x > width - geom.width )
                  new_x = width - geom.width;
               /* Keep y inside display */
               if ( new_y < 0 )
                  new_y = 0;
               else if ( new_y > height - geom.height )
                  new_y = height - geom.height;
               
               delta_x += abs(xdiff);
               delta_y += abs(ydiff);
               window_move( conn, active, new_x - 2, new_y - 2 );
            }
         }
      } else if ( event.type == EnterNotify ) {
         printf( "\t(EnterNotify)\n" );
         button = button_get( event.xcrossing.window );
         button_enter( conn, button );
         button_draw( conn, button );
      } else if ( event.type == LeaveNotify ) {
         printf( "\t(LeavyNotify)\n" );
         button = button_get( event.xcrossing.window );
         button_leave( conn, button );
         button_draw( conn, button );
      } else if ( event.type == Expose ) {
         printf( "\t(Expose)\n" );
         if ( event.xexpose.count == 0 ) {
            if ( event.xexpose.window == overlay ) {
               XCopyArea( conn, button->buffer, button->id, button->on, 0, 0,
                     button->width, button->height, 0, 0 );
            } else {
               button = button_get( event.xexpose.window );
               button_draw( conn, button );
            }
         }
      } else if ( event.type == ConfigureNotify ) {
         printf( "\t(ConfigureNotify)\n" );
         XMoveResizeWindow( conn, overlay, event.xconfigure.x,
               event.xconfigure.y, event.xconfigure.width,
               event.xconfigure.height );
      }
   }
}

Window window_get_active( Display *conn, Window root )
{
   Atom _NET_ACTIVE_WINDOW = XInternAtom( conn, "_NET_ACTIVE_WINDOW", False );
   property_t property = property_get( conn, root, _NET_ACTIVE_WINDOW, 1);
   Window window;
   
   if ( property.type == None )
      window = None;
   else
      window = *((long*)property.data);
   
   return window;
}

window_geom_t window_get_geometry( Display *conn, Window window )
{
   window_geom_t geom;
   Window root;
   int x, y;
   uint width, height, border, depth;
   
   XGetGeometry( conn, window, &root, &x, &y, &width, &height, &border, &depth );
   
   geom.x = x;
   geom.y = y;
   geom.width = width;
   geom.height = height;
   
   return geom;
}

void window_move( Display *conn, Window window, int x, int y )
{
   Atom _NET_MOVERESIZE_WINDOW = XInternAtom( conn, "_NET_MOVERESIZE_WINDOW", False );
   message_t message;
   
   message.window = window;
   message.type = _NET_MOVERESIZE_WINDOW;
   message.data[0] = 5 + (0x0100 | 0x0200);
   message.data[1] = x;
   message.data[2] = y;
   message.data[3] = 0;
   message.data[4] = 0;
   
   message_send_wm( conn, message );
   
   printf( "[message] '_NET_WM_MOVERESIZE_WINDOW'\n" );
   printf( "\tdata[0] = %ld\n", message.data[0] );
   printf( "\tdata[1] = %ld\n", message.data[1] );
   printf( "\tdata[2] = %ld\n", message.data[2] );
   printf( "\tdata[3] = %ld\n", message.data[3] );
   printf( "\tdata[4] = %ld\n", message.data[4] );
}

void window_close( Display *conn, Window window, Time timestamp )
{
   Atom _NET_CLOSE_WINDOW = XInternAtom( conn, "_NET_CLOSE_WINDOW", False );
   message_t message;
   
   message.window = window;
   message.type = _NET_CLOSE_WINDOW;
   message.data[0] = timestamp;
   message.data[1] = 2;
   
   message_send_wm( conn, message );
}