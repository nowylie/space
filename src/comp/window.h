#include "common.h"

#ifndef _WINDOW_H_
#define _WINDOW_H_

void window_list_populate();

void window_create(Window id);
void window_destroy(Window id);
void window_configure(XConfigureEvent event);
void window_set_mapped(Window id, bool mapped);

void window_damaged(Window id);
void window_draw(Window id, Picture surface);
void window_list_draw(Picture surface);

#endif