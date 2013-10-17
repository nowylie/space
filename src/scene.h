#include "common.h"

#ifndef _SCENE_H_
#define _SCENE_H_

typedef struct scene_obj scene_obj;

struct scene_obj {
	Window id;
	int centre, edge;
	scene_obj *prev;
	scene_obj *next;
};

typedef struct {
	scene_obj *hlist;
	scene_obj *vlist;
} scene_t;

void scene_init();
void scene_free();
void scene_add_object(scene_obj *obj);
scene_obj *scene_get_nearest(scene_obj *head, int position);
scene_obj *scene_drop_object(Window id);

extern scene_t *scene;

#endif