#include "common.h"

#include "atoms.h"
#include "properties.h"
#include "manager.h"

int main( )
{
   Display *conn;
   Window root, manager;

   conn = XOpenDisplay( NULL );

   if ( conn == NULL ) {
      fprintf( stderr, "Error, couldn't open display.\n" );
      exit(EXIT_FAILURE);
   }
   
   root = DefaultRootWindow( conn );

   atoms_init( conn );
   
   manager = manager_init( conn );
   
   XDestroyWindow( conn, manager );
   XCloseDisplay( conn );

   return EXIT_SUCCESS;
}
