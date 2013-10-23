#include "common.h"

#ifndef _OBJECT_H_
#define _OBJECT_H_

object_t *object_get(Window id);

object_t *object_create(XCreateWindowEvent event);

object_t *object_destroy(XDestroyWindowEvent event);

object_t *object_configure(XConfigureEvent event);

object_t *object_map(XMapEvent event);

object_t *object_unmap(XUnmapEvent event);

void object_render(viewport_t *viewport, object_t *object);

#endif