#include "common.h"
#include "event_thread.h"

void* event_function(void* param)
{
	server_t *server = (server_t*)param;
	XEvent ev;
	
	printf("[event] thread started.\n");
	
	XSelectInput(server->conn, server->root, SubstructureNotifyMask);

	while (1)
	{
		XNextEvent(server->conn, &ev);
		printf("[event] Event of type %d received.\n", ev.type);
	}
	
	printf("[event] thread stopped.\n");
	
	return NULL;
}