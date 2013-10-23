#include "common.h"

#ifndef _SCENE_H_
#define _SCENE_H_

scene_t *scene_create();

void scene_add_object(scene_t *scene, object_t *object);
void scene_remove_object(scene_t *scene, object_t *object);

#endif