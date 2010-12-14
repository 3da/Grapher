/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_BUTTONS_H_INCLUDED__
#define __BSGUI_BUTTONS_H_INCLUDED__

struct Button : public Control
{
	char	*caption;
	Bitmap	*bitmap;
	bool	autoDeleteBitmap;
	bool	pushed;
	bool	highlighted;

	Button(Control *parent, int x1, int y1, int x2, int y2, char *caption,
		Bitmap *bitmap=NULL);
	virtual ~Button();

	virtual void setCaption(char *newCaption);
	virtual void setBitmap(Bitmap *newBitmap, bool autoDelete);

	virtual void render();
	
	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseUp(int x, int y, int b);
};

#endif
