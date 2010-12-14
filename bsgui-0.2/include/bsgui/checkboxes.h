/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_CHECKBOXES_H_INCLUDED__
#define __BSGUI_CHECKBOXES_H_INCLUDED__

struct Checkbox : public Control
{
	char			*caption;
	bool			hover;
	bool			pushed;
	DataManager<bool>	checked;

	Checkbox(Control *parent, int x1, int y1, int x2, int y2,char *caption);
	virtual ~Checkbox();

	virtual void setCaption(char *newCaption);

	virtual void render();
	
	virtual void setState(bool checked);

	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseUp(int x, int y, int b);
	virtual void onMouseEnter();
	virtual void onMouseExit();
};

#endif
