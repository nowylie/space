#include "message.h"

#include <string.h>

void message_send_window( Display *conn, Window target, message_t message )
{
   XEvent send;
   long mask = SubstructureNotifyMask | SubstructureRedirectMask;
   
   send.type = ClientMessage;
   send.xclient.type = ClientMessage;
   send.xclient.window = message.window;
   send.xclient.message_type = message.type;
   send.xclient.format = 32;
   memcpy( send.xclient.data.l, message.data, sizeof(long) * 5 );
   
   XSendEvent( conn, target, False, mask, &send );
}

void message_send_wm( Display *conn, message_t message )
{
   message_send_window( conn, DefaultRootWindow( conn ), message );
}