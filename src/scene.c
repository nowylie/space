#include "scene.h"

scene_t *scene = NULL;

void scene_init()
{
	scene = malloc(sizeof(scene_t));
	
	scene->zoom = FALSE;
	scene->hlist = NULL;
	scene->vlist = NULL;
}

void scene_free()
{
	free(scene);
}

void scene_add_object(scene_obj *obj)
{
	scene_obj *current = scene->hlist;
	
	obj->prev = obj->next = NULL;
	
	if (current == NULL) {
		scene->hlist = obj;
		return;
	} else if (current->centre >= obj->centre) {
		obj->next = current;
		current->prev = obj;
		scene->hlist = obj;
		return;
	}
	
	while (current != NULL) {
		if (current->centre >= obj->centre) {
			/* obj goes first */
			obj->next = current;
			obj->prev = current->prev;
			current->prev->next = obj;
			current->prev = obj;
			return;
		} else {
			if (current->next == NULL) {
				current->next = obj;
				obj->prev = current;
				return;
			}/* go to next item */
			current = current->next;
		}
	}
}

scene_obj *scene_get_nearest(scene_obj *head, int position)
{
	return NULL;
}

scene_obj *scene_drop_object(Window id)
{
	scene_obj *current = scene->hlist;
	
	while (current != NULL) {
		if (current->id == id) {
			scene_obj *temp = current;
			if (current->prev != NULL)
				current->prev->next = temp->next;
			
			if (current->next != NULL)
				current->next->prev = temp->prev;
				
			return temp;
		}
		current = current->next;
	}
	
	return NULL;
}