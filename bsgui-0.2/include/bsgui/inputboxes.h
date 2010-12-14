/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_INPUTBOXES_H_INCLUDED__
#define __BSGUI_INPUTBOXES_H_INCLUDED__

struct Inputbox : public Control
{
	DataManager<char*>	text;
	int     		tScroll;
	int     		cursor;
	int     		cursorX;
	
	Inputbox(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~Inputbox();
	
	virtual void setText(char *newText);
	virtual char *getText();

	virtual void render();
	
	virtual void onKeyDown(int key, unsigned char ascii);
	virtual void onKeyUp(int key, unsigned char ascii);
};

#endif
