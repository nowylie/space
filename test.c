/* This is a test that prints otu current input devices and identifies
   any touch enabled ones. It will then create a simple window and attempt
   to read touch events on that window */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

int main()
{
	int major = 2, minor = 2;
	int opcode, event, error;
	
	Display *dsp;
	XIDeviceInfo *info;
	int i, ndevices;
	
	if ((dsp = XOpenDisplay(0x0)) == NULL) {
		fprintf(stderr, "Display could not be opened.\n");
		return EXIT_FAILURE;
	}
	
	if (!XQueryExtension(dsp, "XInputExtension", &opcode, &event, &error)) {
		fprintf(stderr, "XInput extension not available.\n");
		return EXIT_FAILURE;
	}
	
	XIQueryVersion(dsp, &major, &minor);
	
	if (major * 1000 + minor < 2002)
		fprintf(stderr, "Server does not support XI 2.2\n");
	
	info = XIQueryDevice(dsp, XIAllDevices, &ndevices);
	
   int touchDev = 0;

	for (i = 0; i < ndevices; i++) {
		int j;
		XIDeviceInfo *dev = &info[i];
		
		printf("Device name %s\n", dev->name);
		
		for (j = 0; j < dev->num_classes; j++) {
			XIAnyClassInfo *class;
			XITouchClassInfo *t;
			
			class = dev->classes[j];
			t = (XITouchClassInfo*)class;
			
			if (class->type != XITouchClass) {
				printf("\t not a touch class.\n");
				continue;
			}
			
         touchDev = dev->deviceid;

			printf("%s touch device, supporting %d touches.\n",
					 (t->mode == XIDirectTouch) ? "direct" : "dependent",
					 t->num_touches);
		}
	}

   if (touchDev == 0) {
      fprintf(stderr, "Could not find a touch device!\n");
      XCloseDisplay(dsp);
      exit(EXIT_FAILURE);
	} else {
      printf("Selected touch device with id: %d\n", touchDev);
   }

   Window win = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, 600, 400, 0, 0, 0);
   
   XEvent ev;

   XSelectInput(dsp, win, ExposureMask);
   XMapWindow(dsp, win);
   XMaskEvent(dsp, ExposureMask, &ev);



   XIEventMask eventmask;
   unsigned char mask[XIMaskLen(XI_LASTEVENT)];

   eventmask.mask = mask;
   eventmask.mask_len = sizeof(mask);
   memset(mask, 0, sizeof(mask));
   eventmask.deviceid = XIAllMasterDevices;

   XISetMask(mask, XI_TouchBegin);
   XISetMask(mask, XI_TouchUpdate);
   XISetMask(mask, XI_TouchEnd);
   XISetMask(mask, XI_TouchOwnership);

   XISelectEvents(dsp, win, &eventmask, 1);

   XSync(dsp, False);

   while (1)
   {   
      XNextEvent(dsp, &ev);
      if (ev.xcookie.extension == opcode && XGetEventData(dsp, &ev.xcookie)) {
         XIDeviceEvent *tEvent;
         switch (ev.xcookie.evtype) {
            case XI_TouchBegin:
               tEvent = ev.xcookie.data;
               printf("TouchBegin event with id: %d\n", tEvent->detail);
               break;
            case XI_TouchUpdate:
               tEvent = ev.xcookie.data;
               printf("TouchUpdate event with id: %d\n", tEvent->detail);
               break;
            case XI_TouchEnd:
               tEvent = ev.xcookie.data;
               printf("TouchEnd event with id: %d\n", tEvent->detail);
               break;
         }
      }
      XFreeEventData(dsp, &ev.xcookie);
   }

	XCloseDisplay(dsp);
	
	return EXIT_SUCCESS;
}
