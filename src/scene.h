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

void scene_add_object(scene_obj *obj);

#endif