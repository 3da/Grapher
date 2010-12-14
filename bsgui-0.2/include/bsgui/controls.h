/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_CONTROLS_H_INCLUDED__
#define __BSGUI_CONTROLS_H_INCLUDED__

struct Control
{
	Control 		*parent;
	Control			*firstChild;
	Control			*lastChild;
	Control			*previous;
	Control			*next;
	
	Control			*exclusiveChild;
	
	char            	*name;
	int			xShift;
	int			yShift;
	int			x1;
	int			y1;
	int			x2;
	int			y2;
	int			padLeft;
	int			padTop;
	int			padRight;
	int			padBottom;
	int			minWidth;
	int			minHeight;
	int			maxWidth;
	int			maxHeight;
	bool            	performLayout;
	bool			mouseOverControl;
	
	struct Action		*clicked;
	struct Action		*modified;
	struct Action		*selected;
	struct Action		*moved;
	struct Action		*resized;

	struct PopupMenu        *popupMenu;
	
	Control(Control *parent);
	virtual ~Control();

	virtual void setName(char *newName);
	virtual Control *findChild(char *name);
	
	virtual void addChild(Control *child);
	virtual void removeChild(Control *child);
	virtual void removeAllChildren();
	virtual void layout(){};
	
	virtual Control *childAt(int x, int y);
	
	struct Screen *getScreen();
		
	virtual void makeExclusive();
	
	virtual void tick();
	virtual void render();
	virtual void focus();
	virtual bool focused();
	virtual void place(int x1, int y1, int x2, int y2);
	virtual void move(int x, int y);
	virtual void resize(int width, int height);
	virtual void center(bool horizontal=true, bool vertical=true);
	virtual void translate(int &x, int &y);
	virtual void getBounds(int &x1, int &y1, int &x2, int &y2);
	virtual void setPadding(int left, int top=-1, int right=-1,
		int bottom=-1);
	virtual void getClientSize(int &w, int &h);
	
	// drawing helpers
	virtual void openClip();
	virtual void closeClip();
	virtual void drawOwnFrame();
	virtual void drawOwnFocus();
	// mouse events
	virtual void onMouseEnter();
	virtual void onMouseMoved(int x, int y);
	virtual void onMouseExit();
	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseUp(int x, int y, int b);
	// keyboard events
	virtual void onKeyDown(int key, unsigned char ascii);
	virtual void onKeyUp(int key, unsigned char ascii);
	// focus events
	virtual void onFocusEnter(){};
	virtual void onFocusExit(){};
};

typedef Control Widget;

#endif
