#include "common/includes.h"
#include "common/properties.h"

#include "atoms.h"
#include "frame.h"
#include "manager.h"

void process_configure( Display *conn, XConfigureRequestEvent event )
{
   XWindowChanges changes;
   
   changes.x = event.x;
   changes.y = event.y;
   changes.width = event.width;
   changes.height = event.height;
   changes.border_width = event.border_width;
   changes.sibling = event.above;
   changes.stack_mode = event.detail;
   
   XConfigureWindow( conn, event.window, event.value_mask, &changes );
}

int main()
{
   Display *conn;
   Window root, manager;
   XEvent event;
   frame_t *frame;

   conn = XOpenDisplay( NULL );

   if ( conn == NULL ) {
      fprintf( stderr, "Error, couldn't open display.\n" );
      exit(EXIT_FAILURE);
   }
   
   root = DefaultRootWindow( conn );

   atoms_init( conn );
   
   manager = manager_init( conn );
   
   XSelectInput( conn, root, SubstructureRedirectMask );
   
   while ( 1 ) {
      XNextEvent( conn, &event );
      printf( "[man] type: %d ", event.type );
      
      if ( event.type == EnterNotify ) {
         /* Change the window focus on pointer enter events */
         printf( "(EnterNotify)\n" );
         if ( event.xcrossing.subwindow != None ) {
            frame = frame_get( event.xcrossing.window );
            frame_focus( conn, frame );
         }
      } else if ( event.type == UnmapNotify ) {
         /* A window has been removed, free the relative data */
         printf( "(UnmapNotify)\n" );
         frame = frame_get( event.xunmap.event );
         frame_destroy( conn, frame );
      } else if ( event.type == MapRequest ) {
         /* A new window has been created, set it up */
         printf( "(MapRequest)\n" );
         frame_create( conn, event.xmaprequest.window );
      } else if ( event.type == ConfigureRequest ) {
         printf( "(ConfigureRequest)\n" );
         process_configure( conn, event.xconfigurerequest );
      } else if ( event.type == ClientMessage ) {
         printf( "(ClientMessage)\n" );
         manager_interpret_message( conn, event.xclient );
      } else {
         printf( "\n" );
      }
   }
   
   XDestroyWindow( conn, manager );
   XCloseDisplay( conn );

   return EXIT_SUCCESS;
}
