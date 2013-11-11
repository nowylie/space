#include "common/includes.h"

#ifndef _S_MANAGER_H_
#define _S_MANAGER_H_

/* This performs window manager initialization. It sets up values on the root *
 * window as per the EWMH specification.                                      */
Window manager_init( Display *conn );

void manager_interpret_message( Display *conn, XClientMessageEvent event );

#endif