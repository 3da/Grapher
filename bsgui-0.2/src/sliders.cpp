/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Slider::Slider(Control *parent, int x1, int y1, int x2, int y2)
	: Control(parent)
{
	sliding = false;
	place(x1, y1, x2, y2);
	max = 5;
	value.setControl(this);
	value.set(0);
}

Slider::~Slider()
{
}

void Slider::render()
{
	int			x1, y1, x2, y2, width, height, sx;
	BSGUIDraw::FrameStyle	fs;
	getBounds(x1, y1, x2, y2);
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	sx = (int)(x1 + (width-9)*value.get()/max + 4);
	
	BSGUIDraw::color(40, 54, 55);
 	BSGUIDraw::drawLine(x1 + 4, y1 + height/2, x2 - 4, y1 + height/2);
 		
	if (mouseOverControl)
		fs = BSGUIDraw::FS_RAISED;
	else
		fs = BSGUIDraw::FS_FLAT;

	BSGUIDraw::drawFrame(sx - 4, y1, sx + 4, y2, fs);
}

void Slider::setValue(float newValue)
{
	value.set(newValue);
	BSGUI_RUNACTION(modified);
}

void Slider::setMax(float newMax)
{
	max = newMax;
	if (value.get() > max)
		setValue(max);
}

void Slider::onMouseDown(int x, int y, int b)
{
	if (b != 1)
		return;
	focus();
	sliding = true;
	setMouseTrackingControl(this);
	onMouseMoved(x, y);
}

void Slider::onMouseMoved(int x, int y)
{
	int	x1, y1, x2, y2, width, height;
	float   oldValue = value.get();
	
	if (!sliding)
		return;
	
	getBounds(x1, y1, x2, y2);
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;
	
	if (x < x1+3)
		x = x1+3;
	else
		if (x > x2-4)
			x = x2-4;
			
 	value.set(max*(float)(x-3-x1)/(float)(width-8));
 	if (value.get() < 0.0f)
 		value.set(0.0f);
 	else
 		if (value.get() > max)
 			value.set(max);
 			
	if (value.get() != oldValue)
		BSGUI_RUNACTION(modified);
}

void Slider::onMouseUp(int x, int y, int b)
{
	if (b != 1 || !sliding)
		return;
	sliding = false;
	setMouseTrackingControl(NULL);
}
