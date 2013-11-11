#include "common/includes.h"

#ifndef _S_MESSAGE_H_
#define _S_MESSAGE_H_

typedef struct {
   Window window;
   Atom type;
   long data[5];
} message_t;

void message_send_window( Display *conn, Window target, message_t message );
void message_send_wm( Display *conn, message_t message );

#endif
