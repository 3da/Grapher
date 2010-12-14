/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SCROLLBOXES_H_INCLUDED__
#define __BSGUI_SCROLLBOXES_H_INCLUDED__

struct Scrollbox : public Control
{
	float   clientWidth;
	float   clientHeight;
	float   hScrollMax;
	float   vScrollMax;
	float   hScroll;
	float   vScroll;
	int	scrolling;      // 0=no, 1=vertical, 2=horizontal
	Action  *scrolled;
	
	Scrollbox(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~Scrollbox();
	
	virtual void setScrollingBounds(float hMax, float vMax);
	
	virtual void render();
	virtual void place(int x1, int y1, int x2, int y2);
	virtual void layout();

	virtual void onScroll(float hd, float vd);
	
	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseMoved(int x, int y);
	virtual void onMouseUp(int x, int y, int b);
};

#endif

