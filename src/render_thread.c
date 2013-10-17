#include "render_thread.h"
#include "window_hash.h"
#include "scene.h"

#include <time.h>

#define NANO_SECOND_MULTIPLIER 100000

XRenderColor green = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0xffff, .blue = 0x0000};
XRenderColor blue = (XRenderColor){.alpha = 0xffff, .red = 0x0000, .green = 0x0000, .blue = 0xffff};


void* render_function(void* param)
{
	server_t *server = (server_t*) param;
	struct timespec time;
	Pixmap back_pix;
	Picture back_pic;
	XRenderPictFormat *format;
	int backx, backy;
	
	backx = backy = 0;
	
	printf("[render] thread started.\n");
	
	back_pix = XCreatePixmap(server->conn, server->root, server->width,
		server->height, 32);
	
	format = XRenderFindStandardFormat(server->conn, PictStandardARGB32);
	
	back_pic = XRenderCreatePicture(server->conn, back_pix, format, 0, NULL);
	XRenderSetPictureFilter(server->conn, back_pic, FilterBilinear, 0, 0 );
					
	while (1) {
		scene_obj *current = scene->hlist;
		
		XRenderFillRectangle(server->conn, PictOpSrc, back_pic,
			&green, 0, 0, server->width, server->height);
		
		while (current != NULL) {
			window_t *w = get_window(current->id);
			
			if (w->store != None) {
				XRenderFillRectangle(server->conn, PictOpSrc, back_pic,
					&blue, w->x - 2, w->y - 2, w->width + 4, w->height + 4);
				XRenderComposite(server->conn, PictOpSrc, w->picture,
					None, back_pic, 0, 0, 0, 0, w->x, w->y,
					w->width, w->height);
			}
			current = current->next;
		}
		
		XRenderComposite(server->conn, PictOpOver, back_pic, None,
			server->output, 0, 0, 0, 0, backx, backy, server->width,
			server->height);
		
		XFlush(server->conn);
		
		time.tv_sec = 0;
		time.tv_nsec = 41 * NANO_SECOND_MULTIPLIER;
		
		nanosleep(&time, NULL);
		/*sleep here please*/
	}
		
	printf("[render] thread stopped.\n");
	
	return NULL;
}