/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Screen	*screen = NULL;

Screen::Screen()
	: Control(NULL)
{
	GLint	vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x2 = vp[2] - 1;
	y2 = vp[3] - 1;
 	width = vp[2];
 	height = vp[3];
}

Screen::~Screen()
{
}

Screen *Screen::activate()
{
	Screen	*prevScreen = screen;
	screen = this;
	GLint	vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x2 = vp[2] - 1;
	y2 = vp[3] - 1;
 	width = vp[2];
 	height = vp[3];
	return prevScreen;
}
