/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SCREEN_H_INCLUDED__
#define __BSGUI_SCREEN_H_INCLUDED__

struct Screen : public Control
{
	int			width;
	int			height;
	int     		mouseX;
	int     		mouseY;
	
	Screen();
	virtual ~Screen();
	
	virtual Screen *activate();
};

extern Screen	*screen;

#endif
