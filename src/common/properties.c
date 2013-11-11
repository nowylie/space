#include "properties.h"

void property_set( Display *conn, Window window, Atom property, property_t value )
{
   char *name = XGetAtomName( conn, property );
   
   XChangeProperty( conn, window, property, value.type, value.format,
         PropModeReplace, value.data, value.size );
}

property_t property_get( Display* conn, Window window, Atom property, long size )
{
   property_t value;
   
   int status;
   long bytes;
   
   status = XGetWindowProperty( conn, window, property, 0, size, False,
                                AnyPropertyType, &value.type, &value.format,
                                &value.size, &bytes, &value.data );
   
   return value;
}

int property_size( Display* conn, Window window, Atom property )
{
   property_t value = property_get( conn, window, property, 0 );
   
   if ( value.type == None )
      return 0;
   else
      return value.size;
}