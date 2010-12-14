/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_BSGUI_H_INCLUDED__
#define __BSGUI_BSGUI_H_INCLUDED__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#ifdef DELETE
#undef DELETE
#endif
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

typedef unsigned int	uint;

// Note: define this if you want to use my libbsmemtrack memory tracking library
#ifdef BSGUI_ENABLE_BSMEMORYTRACKER
#include <bsmemtrack/memory.h>
#else
#define NEW(o,v...)	new o(v)
#define DELETE(o)	delete (o)
#define MALLOC(size)	malloc(size)
#define REALLOC(ptr,sz)	realloc(ptr, sz)
#define FREE(ptr)	free(ptr)
#define TRACKALLOC(p)
#define TRACKDEALLOC(p)
#endif

#include <bsgui/actions.h>
#include <bsgui/dataman.h>
#include <bsgui/controls.h>
#include <bsgui/screen.h>
#include <bsgui/images.h>
#include <bsgui/windows.h>
#include <bsgui/labels.h>
#include <bsgui/buttons.h>
#include <bsgui/checkboxes.h>
#include <bsgui/sliders.h>
#include <bsgui/scrollboxes.h>
#include <bsgui/listboxes.h>
#include <bsgui/inputboxes.h>
#include <bsgui/filebrowser.h>
#include <bsgui/renderers.h>
#include <bsgui/modelviews.h>
#include <bsgui/messageboxes.h>
#include <bsgui/menus.h>
#include <bsgui/progressbars.h>
#include <bsgui/draw.h>

extern Action	*terminateBSGUIApplicationAction;

bool initBSGUI();
void shutdownBSGUI();

bool initBSGUIApplication(char *caption="BSGUI Application", int width=800,
	int height=600, bool fullscreen=false);
void runBSGUIApplication(void (*idleFunc)()=NULL);
void terminateBSGUIApplication(void);
void shutdownBSGUIApplication();

void renderBSGUI();
void renderOffscreenBSGUIControls();
void tickBSGUI();
bool handleSDLEvent(SDL_Event *ev);
void setMouseTrackingControl(Control *control);
void invalidateControl(Control *control);
void setKeyboardFocusControl(Control *control);
bool hasKeyboardFocus(Control *control);

#endif
