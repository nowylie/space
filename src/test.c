#include "common.h"
#include "scene.h"

int main()
{
	scene_obj *first, *second, *third, *temp;
	
	scene_init();
	
	first = malloc(sizeof(scene_obj));
	second = malloc(sizeof(scene_obj));
	third = malloc(sizeof(scene_obj));
	
	first->id = 1;
	first->centre = -15;
	second->id = 2;
	second->centre = 10;
	third->id = 3;
	third->centre = 32;
	
	scene_add_object(second);
	scene_add_object(first);
	scene_add_object(third);
	
	temp = scene->hlist;
	
	while (temp != NULL) {
		printf("Window centre at x position: %d\n", temp->centre);
		temp = temp->next;
	}
	
	temp = scene_drop_object(1);
	free(temp);
	
	temp = scene_drop_object(2);
	free(temp);
	
	temp = scene_drop_object(3);
	free(temp);
	
	scene_free();
	
	return EXIT_SUCCESS;
}