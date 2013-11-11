#include "includes.h"

#ifndef _S_PROPERTIES_H_
#define _S_PROPERTIES_H_

typedef struct {
   Atom              type;
   int               format;
   long              size;
   unsigned char *   data;
} property_t;

void
property_set(
   Display*    conn,
   Window      window,
   Atom        property,
   property_t  value
);

property_t
property_get(
   Display*    conn,
   Window      window,
   Atom        property,
   long        size
);

int
property_size(
   Display*    conn,
   Window      window,
   Atom        property
);

#endif