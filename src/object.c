#include "object.h"

object_t *object_hash = NULL;

object_t *object_get(Window id)
{
	object_t *object;
	
	HASH_FIND(hh, object_hash, &id, sizeof(Window), object);
	
	return object;
}

object_t *object_create(XCreateWindowEvent event)
{
	object_t *object = malloc(sizeof(object_t));
	
	if (object == NULL)
		return NULL;
	
	object->id = event.window;
	object->x = 0;
	object->y = 0;
	object->width = 0;
	object->height = 0;
	object->back_pixmap = None;
	object->buff_pixmap = None;
	object->prev = NULL;
	object->next = NULL;
	
	HASH_ADD(hh, object_hash, id, sizeof(Window), object);
	
	return object;
}

object_t *object_destroy(XDestroyWindowEvent event)
{
	object_t *object;
	
	object = object_get(event.window);
	
	HASH_DELETE(hh, object_hash, object);
	
	return object;
}

object_t *object_configure(XConfigureEvent event)
{
	object_t *object;
	
	object = object_get(event.window);
	
	object->x = event.x;
	object->y = event.y;
	
	if (object->width != event.width || object->height != event.height) {
		/* Window size has changed, you may need to resize the picture */
		object->width = event.width + (event.border_width * 2);
		object->height = event.height + (event.border_width * 2);
		
		if (object->back_pixmap != None) {
			/* Free the old picture/pixmap and allocate new ones */
			XRenderPictFormat *format;
			XRenderPictureAttributes pa;
			
			XRenderFreePicture(conn, object->back_picture);
			XFreePixmap(conn, object->back_pixmap);
			
			object->back_pixmap = XCompositeNameWindowPixmap(conn, object->id);
			
			format = XRenderFindVisualFormat(conn, visual);
			pa.subwindow_mode = IncludeInferiors;
			
			object->back_picture = XRenderCreatePicture(conn, object->back_pixmap,
					format, CPSubwindowMode, &pa);
		}
	}
	
	return object;
}

object_t *object_map(XMapEvent event)
{
	object_t *object;
	XRenderPictFormat *format;
	XRenderPictureAttributes pa;
	
	object = object_get(event.window);
	
	/* Need to get the pixmap and create the picture that is now associated
	 * with this window */
	object->back_pixmap = XCompositeNameWindowPixmap(conn, object->id);
	
	format = XRenderFindVisualFormat(conn, visual);
	pa.subwindow_mode = IncludeInferiors;
	
	object->back_picture = XRenderCreatePicture(conn, object->back_pixmap,
			format, CPSubwindowMode, &pa);
	
	object->damage = XDamageCreate(conn, object->id, XDamageReportNonEmpty);
	
	return object;
}

object_t *object_unmap(XUnmapEvent event)
{
	object_t *object;
	
	object = object_get(event.window);
	
	XRenderFreePicture(conn, object->back_picture);
	XFreePixmap(conn, object->back_pixmap);
	
	return object;
}

void object_render(viewport_t *viewport, object_t *object)
{	
	XRenderComposite(conn, PictOpSrc, object->back_picture, None,
			viewport->buff_picture, 0, 0, 0, 0, object->x - viewport->x,
			object->y - viewport->y, object->width, object->height);
}