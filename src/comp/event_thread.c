#include "event_thread.h"

#include "window.h"

void event_function()
{
	int damage_event, damage_error;
	Pixmap buffer_pixmap;
	Picture buffer;
	XRenderPictFormat *format;
	XEvent ev;
	
	printf("[event] function started.\n");
	
	XDamageQueryExtension(conn, &damage_event, &damage_error);
	
	XSelectInput(conn, DefaultRootWindow(conn), SubstructureNotifyMask);
	
	/* Create the buffer pixmap/picture for drawing onto. After each drawing
		operation, we will copy this onto the overlay window. */
   buffer_pixmap = XCreatePixmap(conn, DefaultRootWindow(conn), width, height,
			depth);
	
	format = XRenderFindStandardFormat(conn, PictStandardRGB24);
	
	buffer = XRenderCreatePicture(conn, buffer_pixmap, format, 0, NULL);
	
	/* clear the output window */
	XRenderFillRectangle(conn, PictOpSrc, buffer, &black,	0, 0, width, height);
	XRenderComposite(conn, PictOpSrc, buffer, None, output, 0, 0, 0, 0, 0, 0,
						width, height);
	XFlush(conn);
	printf("[event] test.\n");
	while (1)
	{
		/*while (XCheckTypedEvent(conn, DestroyNotify, &ev) == True) {
			printf("[event] window %lu destroyed.\n", ev.xdestroywindow.window);
			window_destroy(ev.xdestroywindow.window);
		}
		while (XCheckTypedEvent(conn, ReparentNotify, &ev) == True) {
			printf("[event] window %lu destroyed due to reparenting.\n",
					ev.xreparent.window);
			window_destroy(ev.xreparent.window);
		}*/
		
		XNextEvent(conn, &ev);
		
		printf("[event] Event of type %d received.\n", ev.type);
		if (ev.type == CreateNotify) {
			printf("[event] window %lu created.\n", ev.xcreatewindow.window);
			window_create(ev.xcreatewindow.window);
		/*} else if (ev.type == DestroyNotify) {
			printf("[event] window %lu destroyed.\n", ev.xdestroywindow.window);
			window_destroy(ev.xdestroywindow.window);*/
		} else if (ev.type == ConfigureNotify) {
			printf("[event] window %lu configured.\n", ev.xconfigure.window);
			window_configure(ev.xconfigure);
		} else if (ev.type == MapNotify) {
			printf("[event] window %lu mapped.\n", ev.xmap.window);
			window_set_mapped(ev.xmap.window, TRUE);
			if (ev.xmap.override_redirect == True)
				printf("[event] override redirect is true\n");
		} else if (ev.type == UnmapNotify) {
			printf("[event] window %lu unmapped.\n", ev.xmap.window);
			window_set_mapped(ev.xunmap.window, FALSE);
		} else if (ev.type == damage_event + XDamageNotify) {
			XDamageNotifyEvent *devent = (XDamageNotifyEvent*)&ev;
			XDamageSubtract(conn, devent->damage, None, None);
		}
		XRenderFillRectangle(conn, PictOpSrc, buffer, &black,	0, 0, width, height);
		window_list_draw(buffer);
		XRenderComposite(conn, PictOpSrc, buffer, None, output, 0, 0, 0, 0, 0, 0,
						width, height);
	}
	
	printf("[event] function ended.\n");
}