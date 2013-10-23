#include "render_thread.h"

#include "scene.h"
#include "viewport.h"
#include "object.h"

#include <time.h>

#define NANO_SECOND_MULTIPLIER 100000

void* render_function(void* param)
{
	struct timespec time;
	
	printf("[render] thread started.\n");
	
	scene->viewport = viewport_create(DefaultRootWindow(conn));
	scene->viewport->start = scene->objects;
	XFlush(conn);
					
	while (1) {
		pthread_mutex_lock(&scene_mutex);
		
		while (scene->dirty != TRUE)
			pthread_cond_wait(&scene_dirty, &scene_mutex);
		
		scene->dirty = FALSE;
		pthread_mutex_unlock(&scene_mutex);
		
		/* Redraw the entire scene, including all objects */
		viewport_clear(scene->viewport);
		viewport_render(scene->viewport);
		viewport_draw(scene->viewport, output);
		XFlush(conn);
	}
		
	printf("[render] thread stopped.\n");
	
	return NULL;
}