/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Scrollbox::Scrollbox(Control *parent, int x1, int y1, int x2, int y2)
	: Control(parent)
{
	place(x1, y1, x2, y2);
	setPadding(2, 2, 14, 16);
	hScroll = 0.0f;
	vScroll = 0.0f;
	scrolling = 0;
	scrolled = NULL;
	setScrollingBounds(1, 1);
}

Scrollbox::~Scrollbox()
{
	BSGUI_FREEACTION(scrolled);
}

void Scrollbox::setScrollingBounds(float hMax, float vMax)
{
	float   oldHScroll = hScroll, oldVScroll = vScroll;
	if (hMax < 1.0f)
	        hMax = 1.0f;
	if (vMax < 1.0f)
	        vMax = 1.0f;
	hScrollMax = hMax;
	vScrollMax = vMax;
	clientWidth = hMax - (x2 - x1 - 16);
	clientHeight = vMax - (y2 - y1 - 16);
	if (clientWidth > 0.0f && hScroll > clientWidth)
	        hScroll = clientWidth;
	else
		if (clientWidth <= 0.0f)
			hScroll = 0;
	if (clientHeight > 0.0f && vScroll > clientHeight)
	        vScroll = clientHeight;
	else
		if (clientHeight <= 0.0f)
			vScroll = 0;
	if (oldHScroll != hScroll || oldVScroll != vScroll)
	{
		onScroll(hScroll - oldHScroll, vScroll - oldVScroll);
		BSGUI_RUNACTION(scrolled);
	}
}

void Scrollbox::render()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	drawOwnFrame();
	
 	BSGUIDraw::drawLine(x2 - 12, y1, x2 - 12, y2);
 	BSGUIDraw::drawLine(x1, y2 - 12, x2, y2 - 12);
 	
 	int	x = x1, y = y1 + 1;

	if (clientHeight > 0.0f)
	{
		if (vScroll != 0.0f)
			y = (int)((y2 - y1 - 24)*vScroll/clientHeight + y1 + 1);
	 	BSGUIDraw::fillRect(x2 - 12, y, x2, y + 11);
	}
	
	if (clientWidth > 0.0f)
	{
		if (hScroll != 0.0f)
			x = (int)((x2 - x1 - 24)*hScroll/clientWidth + x1 + 1);
	 	BSGUIDraw::fillRect(x, y2 - 12, x + 11, y2);
	}

	BSGUIDraw::color(100, 114, 115);
	BSGUIDraw::fillRect(x1, y1 + 1, x2 - 13, y2 - 13);

 	Control::render();
}

void Scrollbox::place(int x1, int y1, int x2, int y2)
{
	Control::place(x1, y1, x2, y2);
	clientWidth = hScrollMax - (x2 - x1 - 16);
	clientHeight = vScrollMax - (y2 - y1 - 16);
}

void Scrollbox::onMouseDown(int x, int y, int b)
{
	int   x1, y1, x2, y2;
	if (b != 1)
	        return;
	focus();
	getBounds(x1, y1, x2, y2);
	
	// vertical scrolling ?
	if (clientHeight > 0.0f && x >= x2 - 12 && y <= y2 - 12)
	{
		scrolling = 1;
		setMouseTrackingControl(this);
		onMouseMoved(x, y);
		return;
	}

	// horizontal scrolling ?
	if (clientWidth > 0.0f && y >= y2 - 12 && x <= x2 - 12)
	{
		scrolling = 2;
		setMouseTrackingControl(this);
		onMouseMoved(x, y);
		return;
	}
}

void Scrollbox::layout()
{
	float   hMax = 1.0f, vMax = 1.0f;
	for (Control *child = firstChild;child;child = child->next)
	{
	        if (child->x2 > hMax)
	                hMax = child->x2;
		if (child->y2 > vMax)
		        vMax = child->y2;
	}
	setScrollingBounds(hMax, vMax);
}

void Scrollbox::onScroll(float hd, float vd)
{
	for(Control *child = firstChild;child;child=child->next)
	{
		child->xShift = (int)hScroll;
		child->yShift = (int)vScroll;
	}
}

void Scrollbox::onMouseMoved(int x, int y)
{
	int	x1, y1, x2, y2;
	float	oldHScroll, oldVScroll;
	if (!scrolling)
		return;
	getBounds(x1, y1, x2, y2);
	
	oldHScroll = hScroll;
	oldVScroll = vScroll;

	if (scrolling == 1)
	{
		vScroll = (float)(y - y1 - 6)*clientHeight/(float)(y2 - y1 -24);
		if (vScroll > clientHeight)
			vScroll = clientHeight;
		else
		if (vScroll < 0.0f)
		        vScroll = 0.0f;
	}
	else
	{
		hScroll = (float)(x - x1 - 6)*clientWidth/(float)(x2 - x1 - 24);
		if (hScroll > clientWidth)
			hScroll = clientWidth;
		else
		if (hScroll < 0.0f)
		        hScroll = 0.0f;
	}
	
	if (oldHScroll != hScroll || oldVScroll != vScroll)
	{
		onScroll(hScroll - oldHScroll, vScroll - oldVScroll);
		BSGUI_RUNACTION(scrolled);
	}
}

void Scrollbox::onMouseUp(int x, int y, int b)
{
	if (!scrolling)
	        return;
	scrolling = 0;
	setMouseTrackingControl(NULL);
}
