#include "event_thread.h"

#include "scene.h"
#include "viewport.h"
#include "object.h"

void* event_function(void* param)
{
	int damage_event, damage_error;
	XEvent ev;
	
	printf("[event] thread started.\n");
	
	XDamageQueryExtension(conn, &damage_event, &damage_error);
	
	XSelectInput(conn, DefaultRootWindow(conn), SubstructureNotifyMask);

	while (1)
	{
		XNextEvent(conn, &ev);
		
		printf("[event] Event of type %d received.\n", ev.type);
		
		if (ev.type == CreateNotify) {
			printf("[event] Creating object with id: %lu\n", ev.xcreatewindow.window);
			object_create(ev.xcreatewindow);
		} else if (ev.type == DestroyNotify) {
			object_t *object;
			
			object = object_destroy(ev.xdestroywindow);
			free(object);
		} else if (ev.type == ConfigureNotify) {
			object_configure(ev.xconfigure);
		} else if (ev.type == MapNotify) {
			object_t *object;
			
			printf("[event] Mapping object with id: %lu\n", ev.xmap.window);
			
			/* Set the scene state to dirty and wake up the render thread */
			pthread_mutex_lock(&scene_mutex);
			
			object = object_map(ev.xmap);
			scene_add_object(scene, object);
			scene->dirty = TRUE;
			
			pthread_cond_signal(&scene_dirty);
			pthread_mutex_unlock(&scene_mutex);
		} else if (ev.type == UnmapNotify) {
			object_t *object;
			
			pthread_mutex_lock(&scene_mutex);
			
			object = object_unmap(ev.xunmap);
			scene_remove_object(scene, object);
			scene->dirty = TRUE;
			
			pthread_cond_signal(&scene_dirty);
			pthread_mutex_unlock(&scene_mutex);
		} else if (ev.type == damage_event + XDamageNotify) {
			XDamageNotifyEvent *devent = (XDamageNotifyEvent*)&ev;
			XDamageSubtract(conn, devent->damage, None, None);
		
			printf("[event] Received damage event, marking scene dirty.\n");
			
			/* Set the scene state to dirty and wake up the render thread */
			pthread_mutex_lock(&scene_mutex);
			scene->dirty = TRUE;
			pthread_cond_signal(&scene_dirty);
			pthread_mutex_unlock(&scene_mutex);
		}
	}
	
	printf("[event] thread stopped.\n");
	
	return NULL;
}