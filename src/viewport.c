#include "viewport.h"

#include "object.h"

viewport_t *viewport_create(Window root)
{
	viewport_t *viewport = malloc(sizeof(viewport_t));
	XRenderPictFormat *format;
	
	if (viewport == NULL)
		return NULL;	
	viewport->x = 0;
	viewport->y = 0;
	viewport->width = width;
	viewport->lwidth = width;
	viewport->height = height;
	viewport->lheight = height;
	viewport->zoom = 1.0f;
	viewport->start = NULL;
	
	viewport->buff_pixmap = XCreatePixmap(conn, root, width, height, depth);
	
	format = XRenderFindStandardFormat(conn, PictStandardRGB24);
	
	viewport->buff_picture = XRenderCreatePicture(conn,
			viewport->buff_pixmap, format, 0, NULL);
	
	return viewport;
}

void viewport_clear(viewport_t *viewport)
{
	XRenderFillRectangle(conn, PictOpSrc, viewport->buff_picture, &green,
			0, 0, viewport->width, viewport->height);
}

void viewport_render(viewport_t *viewport)
{
	object_t *object;
	
	pthread_mutex_lock(&scene_mutex);
	object = viewport->start;
	
	while (object != NULL) {
		object_render(viewport, object);
		printf("[render] Object drawn with id: %lu\n", object->id);
		printf("\tat (%d, %d) dimensions: %dx%d\n", object->x, object->y,
				object->width, object->height);
		
		object = object->next;
	}
	
	pthread_mutex_unlock(&scene_mutex);
}

void viewport_draw(viewport_t *viewport, Picture dest)
{
	XRenderComposite(conn, PictOpSrc, viewport->buff_picture, None, dest,
			0, 0, 0, 0, 0, 0, viewport->width, viewport->height);
}