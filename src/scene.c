#include "scene.h"

scene_t *scene_create()
{
	scene_t *scene = malloc(sizeof(scene_t));
	
	if (scene == NULL)
		return NULL;
	
	scene->objects = NULL;
	scene->viewport = NULL;
	scene->dirty = TRUE;
	
	return scene;
}

void scene_add_object(scene_t *scene, object_t *object)
{
	object_t *current = scene->objects;
	
	if (current == NULL) {
		scene->objects = object;
		scene->viewport->start = object;
		return;
	} else if (current->x + (current->width / 2) >= 
				object->x + (object->width / 2)) {
		object->next = current;
		current->prev = object;
		scene->objects = object;
		scene->viewport->start = object;
		return;
	}
	
	while (current != NULL) {
		if (current->x + (current->width / 2) >= 
				object->x + (object->width / 2)) {
			object->next = current;
			object->prev = current->prev;
			object->prev->next = object;
			current->prev = object;
			return;
		} else {
			if (current->next == NULL) {
				current->next = object;
				object->prev = current;
				return;
			}
			current = current->next;
		}
	}
}

void scene_remove_object(scene_t *scene, object_t *object)
{
	if (object->prev == NULL) {
		if (object->next != NULL)
			object->next->prev = NULL;
		scene->objects = object->next;
		scene->viewport->start = object->next;
	} else {
		if (object->next != NULL)
			object->next->prev = object->prev;
		object->prev->next = object->next;
	}
}