/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_WINDOWS_H_INCLUDED__
#define __BSGUI_WINDOWS_H_INCLUDED__

struct Window : public Control
{
	char			*caption;
	bool			moving;
	bool			resizing;
	bool			resizeable;
	int			mX, mY;
	struct PopupMenu	*menu;
	Bitmap			*icon;
	
	Window(char *caption=NULL);
	Window(Screen *screen, char *caption=NULL);
	virtual ~Window();
	
	virtual void setCaption(char *newCaption);

	virtual void render();
	
	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseMoved(int x, int y);
	virtual void onMouseUp(int x, int y, int b);
};

#endif
