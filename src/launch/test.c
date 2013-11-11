#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <unistd.h>

#define __DEBUG__

pid_t spawn( const char *cmd )
{
	pid_t pid;
	const char *shell;
	
	pid = fork(); /* fork the process */
	
	if ( pid ) /* if pid != 0 we're in the parent process */
		return pid;
	
	/* Get the shell, if we can't find it, default to /bin/sh */
	shell = getenv( "SHELL" );
	if ( !shell )
		shell = "/bin/sh";
	
	setsid(); /* create a new session to run in */
	execl( shell, shell, "-c", cmd, (char*)NULL );
	exit( EXIT_SUCCESS ); /* We only hit this if an error occured. We
									 don't care if one did. */
	
	return pid; /* We never hit this, it just keeps the compiler happy */
}

typedef struct {
	Atom wm_state;
   Atom wm_name;
   Atom wm_icon_name;
   Atom net_wm_name;
   Atom net_wm_icon_name;
   Atom net_client_list_stacking;
} Atoms;

typedef struct {
   long size;
   Window *windows;
} xa_window_list;

typedef struct {
	long state;
   unsigned states;
	Window icon_window;
} xa_wm_state;

#define STATE_MODAL        (0x01)
#define STATE_MAXIMIZED    (0x02)
#define STATE_HIDDEN       (0x04)
#define STATE_ATTENTION    (0x08)
#define STATE_FOCUSED      (0x10)

Display *conn = NULL;
Atoms atoms;

void
atoms_init()
{
   /* Atoms defined in ICCCM */
	atoms.wm_state = XInternAtom( conn, "WM_STATE", False );
   atoms.wm_name = XInternAtom( conn, "WM_NAME", False);
   atoms.wm_icon_name = XInternAtom( conn, "WM_ICON_NAME", False);
   
   /* Atoms defined in EWMH */
   atoms.net_wm_name = XInternAtom( conn, "_NET_WM_NAME", False );
   atoms.net_wm_icon_name = XInternAtom( conn, "_NET_WM_ICON_NAME", False );
	atoms.net_client_list_stacking = XInternAtom( conn, "_NET_CLIENT_LIST_STACKING", False );
}

int
get_client_stacking_list( Window root, xa_window_list *list )
{
   int status;
   Atom actual_type;
   int actual_format;
   long items;
   long bytes;
   long *data;
   
   status = XGetWindowProperty( conn, root, atoms.net_client_list_stacking,
         0, (~0L), False, XA_WINDOW, &actual_type,
         &actual_format, &items, &bytes, (unsigned char**) &data );
   
   if ( status == Success ) {
      #ifdef __DEBUG__
      printf( "[get_client_stacking_list]\n" );
      printf( "type: %d, format: %d, items: %d, bytes_after: %d\n", actual_type,
            actual_format, items, bytes );
      #endif
      list->size = items;
      list->windows = data;
   }
   
   return status;
}

int
get_window_state( Window window, xa_wm_state *state )
{
	int status;
	Atom actual_type;
	int actual_format;
	long items;
	long bytes;
	long *data;
		
   status = XGetWindowProperty( conn, window, atoms.wm_state,
         0, 2L, False, atoms.wm_state, &actual_type, &actual_format, &items,
         &bytes, (unsigned char**) &data );
	
	if ( status == Success ) {
      #ifdef __DEBUG__
      printf( "[get_window_state]\n");
      printf( "type: %d, format: %d, items: %d, bytes_after: %d\n",
            actual_type, actual_format, items, bytes);
      #endif
      state->state = data[0];
      state->icon_window = data[1];
	}
   
   return status;
}

int main()
{
   xa_window_list list;
   int status;
	int i;
	
	conn = XOpenDisplay( NULL );
	if ( conn == NULL ) {
		fprintf( stderr, "Could not open connection to server.\n" );
		exit( EXIT_FAILURE );
	}
   
   atoms_init();
   
	status = get_client_stacking_list( DefaultRootWindow(conn), &list );
   if ( status != Success ) {
      fprintf( stderr, "[error=%d] Couldn't retrieve window list.\n", status );
      exit( EXIT_FAILURE );
	}
	
	for ( i = 0; i < list.size; i++ ) {
      xa_wm_state window_state;
      
      printf( "Window id: %d\n", i, list.windows[i] );
      if ( get_window_state(list.windows[i], &window_state) == Success ) {
         printf( "\tState: %u, Icon window: %lu\n", window_state.state,
               window_state.icon_window );
      }
	}
	
	//printf( "xterm pid = %d\n", spawn( "chromium" ) );
	
	return EXIT_SUCCESS;
}
