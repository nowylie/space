#include "atoms.h"

void atoms_init(Display *conn)
{
   XA_WM_STATE = XInternAtom( conn, "WM_STATE", False );
   XA_WM_PROTOCOLS = XInternAtom( conn, "WM_PROTOCOLS", False );
   XA_WM_DELETE_WINDOW = XInternAtom( conn, "WM_DELETE_WINDOW", False );
   EA_NET_WM_NAME = XInternAtom( conn, "_NET_WM_NAME", False );
   EA_NET_WM_ICON_NAME = XInternAtom( conn, "_NET_WM_ICON_NAME", False );
   EA_NET_WM_WINDOW_TYPE = XInternAtom( conn, "_NET_WM_WINDOW_TYPE", False );;
   EA_NET_WM_WINDOW_TYPE_DESKTOP = XInternAtom( conn, "_NET_WM_WINDOW_TYPE_DESKTOP", False );
   EA_NET_WM_WINDOW_TYPE_DIALOG = XInternAtom( conn, "_NET_WM_WINDOW_TYPE_DIALOG", False );
   EA_NET_WM_WINDOW_TYPE_NORMAL = XInternAtom( conn, "_NET_WM_WINDOW_TYPE_NORMAL", False );
   EA_NET_WM_STATE = XInternAtom( conn, "_NET_WM_STATE", False );
   EA_NET_WM_STATE_MODAL = XInternAtom( conn, "_NET_WM_STATE_MODAL", False );
   EA_NET_WM_STATE_MAXIMIZED_VERT = XInternAtom( conn, "_NET_WM_STATE_MAXIMIZED_VERT", False );
   EA_NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom( conn, "_NET_WM_STATE_MAXIMIZED_HORZ", False );
   EA_NET_WM_STATE_HIDDEN = XInternAtom( conn, "_NET_WM_STATE_HIDDEN", False );
   EA_NET_WM_STATE_FOCUSED = XInternAtom( conn, "_NET_WM_STATE_FOCUSED", False );
   EA_NET_ALLOWED_ACTIONS = XInternAtom( conn, "_NET_ALLOWED_ACTIONS", False );
   EA_NET_WM_ACTION_MOVE = XInternAtom( conn, "_NET_WM_ACTION_MOVE", False );
   EA_NET_WM_ACTION_RESIZE = XInternAtom( conn, "_NET_WM_ACTION_RESIZE", False );
   EA_NET_WM_ACTION_MINIMIZE = XInternAtom( conn, "_NET_WM_ACTION_MINIMIZE", False );
   EA_NET_WM_ACTION_MAXIMIZE_VERT = XInternAtom( conn, "_NET_WM_ACTION_MAXIMIZE_VERT", False );
   EA_NET_WM_ACTION_MAXIMIZE_HORZ = XInternAtom( conn, "_NET_WM_ACTION_MAXIMIZE_HORZ", False );
   EA_NET_WM_ACTION_CLOSE = XInternAtom( conn, "_NET_WM_ACTION_CLOSE", False );
   EA_NET_SUPPORTED = XInternAtom( conn, "_NET_SUPPORTED", False );
   EA_NET_CLIENT_LIST = XInternAtom( conn, "_NET_CLIENT_LIST", False );
   EA_NET_CLIENT_LIST_STACKING = XInternAtom( conn, "_NET_CLIENT_LIST_STACKING", False );
   EA_NET_NUMBER_OF_DESKTOPS = XInternAtom( conn, "_NET_NUMBER_OF_DESKTOPS", False );
   EA_NET_DESKTOP_GEOMETRY = XInternAtom( conn, "_NET_DESKTOP_GEOMETRY", False );
   EA_NET_DESKTOP_VIEWPORT = XInternAtom( conn, "_NET_DESKTOP_VIEWPORT", False );
   EA_NET_CURRENT_DESKTOP = XInternAtom( conn, "_NET_CURRENT_DESKTOP", False );
   EA_NET_ACTIVE_WINDOW = XInternAtom( conn, "_NET_ACTIVE_WINDOW", False );
   EA_NET_WORKAREA = XInternAtom( conn, "_NET_WORKAREA", False );
   EA_NET_SUPPORTING_WM_CHECK = XInternAtom( conn, "_NET_SUPPORTING_WM_CHECK", False );
   EA_NET_CLOSE_WINDOW = XInternAtom( conn, "_NET_CLOSE_WINDOW", False );
   EA_NET_MOVERESIZE_WINDOW = XInternAtom( conn, "_NET_MOVERESIZE_WINDOW", False );
   EA_NET_WM_MOVERESIZE = XInternAtom( conn, "_NET_WM_MOVERESIZE", False );
}