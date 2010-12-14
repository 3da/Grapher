/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Button::Button(Control *parent, int x1, int y1, int x2, int y2, char *caption,
	Bitmap *bitmap)
	: Control(parent)
{
	place(x1, y1, x2, y2);
	this->caption = strdup(caption); TRACKALLOC(this->caption);
	pushed = highlighted = false;
	this->bitmap = bitmap;
	autoDeleteBitmap = true;
}

Button::~Button()
{
	if (autoDeleteBitmap && bitmap)
		DELETE(bitmap);
	FREE(caption);
}

void Button::setCaption(char *newCaption)
{
	FREE(caption);
	caption = strdup(newCaption); TRACKALLOC(caption);
}

void Button::setBitmap(Bitmap *newBitmap, bool autoDelete)
{
	if (autoDeleteBitmap && bitmap)
		DELETE(bitmap);
	bitmap = newBitmap;
	autoDeleteBitmap = autoDelete;
}

void Button::render()
{
	int			x1, y1, x2, y2;
	BSGUIDraw::FrameStyle	fs = BSGUIDraw::FS_FLAT;
	getBounds(x1, y1, x2, y2);
	
	if (mouseOverControl)
		fs = pushed?BSGUIDraw::FS_LOWERED:BSGUIDraw::FS_RAISED;
	else
		fs = BSGUIDraw::FS_FLAT;

	BSGUIDraw::drawFrame(x1, y1, x2, y2, fs);
	if (highlighted)
	{
		BSGUIDraw::color(150, 164, 165);
		BSGUIDraw::fillRect(x1, y1 + 1, x2 - 1, y2);
	}

	if (highlighted)
	{
		BSGUIDraw::color(0, 0, 0);
	}
	else
	if (focused())
	{
		drawOwnFocus();
		BSGUIDraw::color(220, 234, 235);
	}
	else
		BSGUIDraw::color(190, 204, 205);
	x1 = (x2 - x1 + 1)/2 + x1;
	if (bitmap)
	{
		x1 -= (BSGUIDraw::stringWidth(caption) + bitmap->width + 5)/2;
		x2 = bitmap->width + 5;
	}
	else
	{
		x1 -= BSGUIDraw::stringWidth(caption)/2;
		x2 = 0;
	}
	BSGUIDraw::drawString(x1 + x2, (y2-y1 + 1)/2 + y1 - 
		BSGUIDraw::stringHeight()/2, caption);
	if (bitmap)
		bitmap->draw(x1, (y2 - y1 + 1)/2 + y1 - bitmap->height/2);
}

void Button::onMouseDown(int x, int y, int b)
{
	focus();
	if (b == 1)
	{
		setMouseTrackingControl(this);
		pushed = true;
	}
}

void Button::onMouseUp(int x, int y, int b)
{
	if (!focused())
		return;
	if (b == 1 && pushed)
	{
		setMouseTrackingControl(NULL);
		if (mouseOverControl)
			BSGUI_RUNACTION(clicked);
	}
	pushed = false;
}
