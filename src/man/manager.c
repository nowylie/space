#include "manager.h"

#include "common/properties.h"

#include "atoms.h"
#include "frame.h"

Window manager_init( Display *conn )
{
   Window root, win;
   property_t values;
   
   root = DefaultRootWindow( conn );
   win = XCreateSimpleWindow( conn, root, 0, 0, 1, 1, 0, 0, 0 );
   
   /* _NET_SUPPORTED */
   Atom supported[] = {
      EA_NET_WM_NAME,
      EA_NET_WM_ICON_NAME,
      EA_NET_WM_WINDOW_TYPE,
      EA_NET_WM_WINDOW_TYPE_DESKTOP,
      EA_NET_WM_WINDOW_TYPE_DIALOG,
      EA_NET_WM_WINDOW_TYPE_NORMAL,
      EA_NET_WM_STATE,
      EA_NET_WM_STATE_MODAL,
      EA_NET_WM_STATE_MAXIMIZED_VERT,
      EA_NET_WM_STATE_MAXIMIZED_HORZ,
      EA_NET_WM_STATE_HIDDEN,
      EA_NET_WM_STATE_FOCUSED,
      EA_NET_ALLOWED_ACTIONS,
      EA_NET_WM_ACTION_MOVE,
      EA_NET_WM_ACTION_RESIZE,
      EA_NET_WM_ACTION_MINIMIZE,
      EA_NET_WM_ACTION_MAXIMIZE_VERT,
      EA_NET_WM_ACTION_MAXIMIZE_HORZ,
      EA_NET_WM_ACTION_CLOSE,
      EA_NET_SUPPORTED,
      EA_NET_CLIENT_LIST,
      EA_NET_CLIENT_LIST_STACKING,
      EA_NET_NUMBER_OF_DESKTOPS,
      EA_NET_DESKTOP_GEOMETRY,
      EA_NET_DESKTOP_VIEWPORT,
      EA_NET_CURRENT_DESKTOP,
      EA_NET_ACTIVE_WINDOW,
      EA_NET_WORKAREA,
      EA_NET_SUPPORTING_WM_CHECK,
      EA_NET_CLOSE_WINDOW,
      EA_NET_MOVERESIZE_WINDOW,
      EA_NET_WM_MOVERESIZE,
   };
   
   values.type = XA_ATOM;
   values.format = 32;
   values.size = sizeof( supported ) / sizeof( supported[0] );
   values.data = (unsigned char *) supported;
   
   property_set( conn, root, EA_NET_SUPPORTED, values );
   
   /* _NET_NUMBER_OF_DESKTOPS */
   long num_desktops = 1;
   
   values.type = XA_CARDINAL;
   values.format = 32;
   values.size = 1;
   values.data = (unsigned char *) &num_desktops;
   
   property_set( conn, root, EA_NET_NUMBER_OF_DESKTOPS, values );
   
   /* _NET_DESKTOP_GEOMETRY */
   long geometry[2];
   
   geometry[0] = DisplayWidth( conn, DefaultScreen( conn ) );
   geometry[1] = DisplayHeight( conn, DefaultScreen( conn ) );
   values.size = 2;
   values.data = (unsigned char *) geometry;
   
   property_set( conn, root, EA_NET_DESKTOP_GEOMETRY, values );
   
   /* _NET_DESKTOP_VIEWPORT */
   long viewport[2] = {0, 0};
   
   values.data = (unsigned char *) viewport;
   
   property_set( conn, root, EA_NET_DESKTOP_VIEWPORT, values );
   
   /* _NET_CURRENT_DESKTOP */
   long desktop = 0;
   
   values.size = 1;
   values.data = (unsigned char *) &desktop;
   
   property_set( conn, root, EA_NET_CURRENT_DESKTOP, values );
   
   /* _NET_ACTIVE_WINDOW */
   Window active = None;
   values.type = XA_WINDOW;
   values.data = (unsigned char *) &active;
   
   property_set( conn, root, EA_NET_ACTIVE_WINDOW, values );
   
   /* _NET_WORKAREA */
   long workarea[4];
   
   workarea[0] = 0;
   workarea[1] = 0;
   workarea[2] = geometry[0];
   workarea[3] = geometry[1];
   values.type = XA_CARDINAL;
   values.size = 4;
   values.data = (unsigned char *) workarea;
   
   property_set( conn, root, EA_NET_WORKAREA, values);
   
   /* _NET_SUPPORTING_WM_CHECK */
   values.type = XA_WINDOW;
   values.format = 32;
   values.size = 1;
   values.data = (unsigned char *) &win;
   
   property_set( conn, win, EA_NET_SUPPORTING_WM_CHECK, values );
   property_set( conn, root, EA_NET_SUPPORTING_WM_CHECK, values );
   
   /* _NET_WM_NAME on _NET_SUPPORTING_WM_CHECK window */
   values.type = XA_STRING;
   values.format = 8;
   values.size = 10;
   values.data = "space-man";
   
   property_set( conn, win, EA_NET_WM_NAME, values );
   
   XFlush( conn );
   
   return win;
}

void manager_interpret_message( Display *conn, XClientMessageEvent event )
{
   frame_t *frame = frame_get( event.window );
   if (event.message_type == EA_NET_CLOSE_WINDOW ) {
      printf( "\t_NET_CLOSE_WINDOW\n" );
      /* Close the application */
      Window window = frame->child->id;
      Atom *protocols;
      int num, i;
      
      XUnmapWindow( conn, window );
      XGetWMProtocols( conn, window, &protocols, &num );
      
      for (i = 0; i < num; i++ ) {
         if ( protocols[i] == XA_WM_DELETE_WINDOW ) {
            XEvent event_send;
            event_send.type = ClientMessage;
            event_send.xclient.type = ClientMessage;
            event_send.xclient.message_type = XA_WM_PROTOCOLS;
            event_send.xclient.format = 32;
            event_send.xclient.data.l[0] = XA_WM_DELETE_WINDOW;
            event_send.xclient.data.l[1] = event.data.l[0];
            
            XSendEvent( conn, window, False, 0, &event_send );
            
            window = None;
         }
      }
      
      if ( window != None )
         XKillClient( conn, window );
   } else if ( event.message_type == EA_NET_MOVERESIZE_WINDOW ) {
      /* Change the window position on client request */
      if ( event.data.l[0] & (MR_FLAG_X | MR_FLAG_Y) )
         frame_move( conn, frame, event.data.l[1], event.data.l[2] );
   } else if ( event.message_type == EA_NET_ACTIVE_WINDOW ) {
      /* Change the window focus on client request */
      frame_focus( conn, frame );
   }
}