#include "common.h"

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

viewport_t *viewport_create(Window root);

void viewport_clear(viewport_t *viewport);

void viewport_render(viewport_t *viewport);

void viewport_draw(viewport_t *viewport, Picture dest);

#endif