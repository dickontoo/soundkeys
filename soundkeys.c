/* soundkeys.c */

/*
 * Fed up with the sound keys not working, so attempting to fix it.
 */


#include <stdio.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <X11/XF86keysym.h>

#ifndef DEFAULT_VOLUME
#define DEFAULT_VOLUME (0x80);
#endif

void handlekeypress(Display *dpy, XEvent *ev)
{
	static int vol = DEFAULT_VOLUME;
	static int muted = 0;
	char cmd[128];
	KeySym ks;
	int svol;

	ks = XKeycodeToKeysym(dpy, ev->xkey.keycode, 0);

	if (muted) {
		svol = vol;
		muted = 0;
	} else {
		switch (ks) {
		case XF86XK_AudioMute:
			svol = 0;
			muted = 1;
			break;
		case XF86XK_AudioLowerVolume:
			svol = vol -= 10;
			break;
		case XF86XK_AudioRaiseVolume:
			svol = vol += 10;
			break;
		default:
			return;
		}
	}
	snprintf(cmd, sizeof(cmd), "amixer set PCM %d", svol);

	if (vol < 0) {
		vol = 0;
		return;
	} else if (vol > 255) {
		vol = 255;
		return;
	}

	system(cmd);
}



int main(void)
{
	Display *display;
	Window	root;
	XEvent	 ev;
	KeyCode	kc;

	display = XOpenDisplay(getenv("DISPLAY"));
	if (!display)
		exit(1);

	root = RootWindow(display, 0);
	XSelectInput(display, root, KeyPressMask);

	kc = XKeysymToKeycode(display, XF86XK_AudioLowerVolume);
	XGrabKey(display, kc, AnyModifier, root, True, /* lower */
		GrabModeAsync, GrabModeAsync);
	kc = XKeysymToKeycode(display, XF86XK_AudioMute);
	XGrabKey(display, kc, AnyModifier, root, True, /* mute */
		GrabModeAsync, GrabModeAsync);
	kc = XKeysymToKeycode(display, XF86XK_AudioRaiseVolume);
	XGrabKey(display, kc, AnyModifier, root, True, /* raise */
		GrabModeAsync, GrabModeAsync);

	for (;;) {
		XNextEvent(display, &ev);
		switch (ev.type) {
		case KeyPress:
			handlekeypress(display, &ev);
			break;
		}
	}
}
