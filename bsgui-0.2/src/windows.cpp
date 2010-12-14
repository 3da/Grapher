/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Window::Window(char *caption)
	: Control(screen)
{
	moving = resizing = resizeable = false;
	minWidth = 50;
	minHeight = 25;
	resized = NULL;
	place(10, 10, 200, 160);
	if (!caption)
	{
		this->caption = strdup(""); TRACKALLOC(this->caption);
	}
	else
	{
		this->caption = strdup(caption); TRACKALLOC(this->caption);
	}
	padLeft = padRight = padBottom = 2;
	padTop = 6 + BSGUIDraw::stringHeight();
	menu = NULL;
	icon = NULL;
}

Window::Window(Screen *screen, char *caption)
	: Control(screen)
{
	moving = resizing = resizeable = false;
	minWidth = 50;
	minHeight = 25;
	place(10, 10, 200, 160);
	if (!caption)
	{
		this->caption = strdup(""); TRACKALLOC(this->caption);
	}
	else
	{
		this->caption = strdup(caption); TRACKALLOC(this->caption);
	}
	padLeft = padRight = padBottom = 2;
	padTop = 6 + BSGUIDraw::stringHeight();
	menu = NULL;
	icon = NULL;
}

Window::~Window()
{
	FREE(caption);
	if (icon)
		DELETE(icon);
	if (menu)
		DELETE(menu);
}

void Window::setCaption(char *newCaption)
{
	FREE(caption);
	caption = strdup(newCaption); TRACKALLOC(caption);
}

void Window::render()
{
	int	x1, y1, x2, y2;
	int	clen, titleY2;
	getBounds(x1, y1, x2, y2);
	
	titleY2 = BSGUIDraw::stringHeight() + y1 + 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BSGUIDraw::color(120, 134, 135, 204);
	BSGUIDraw::fillRect(x1, y1, x2, titleY2);
	BSGUIDraw::color(100, 114, 115, 204);
	BSGUIDraw::fillRect(x1, titleY2, x2, y2);

	clen = (int)BSGUIDraw::stringWidth(caption);
 	if (focused())
 	{
		BSGUIDraw::color(220, 234, 235);
		BSGUIDraw::drawRect(x1, y1, x2, y2);
		BSGUIDraw::color(219, 225, 236);
	}
	else
	{
		BSGUIDraw::color(129, 135, 146);
		BSGUIDraw::drawRect(x1, y1, x2, y2);
		BSGUIDraw::color(169, 175, 186);
	}
	BSGUIDraw::drawString((x2-x1)/2 + x1 - clen/2, y1 + 1, caption);
	
	glDisable(GL_BLEND);

	Control::render();
	
	if (resizeable)
	{
		BSGUIDraw::color(150, 164, 165);
		BSGUIDraw::drawLine(x2 - 10, y2, x2, y2 - 10);
	}
	
	if (icon)
	{
		icon->draw(x1 + 1, y1 + (titleY2-y1)/2 - icon->height/2);
	}
}

void Window::onMouseDown(int x, int y, int b)
{
	int	x1, y1, x2, y2, titleY2;
	getBounds(x1, y1, x2, y2);
	
	titleY2 = BSGUIDraw::stringHeight() + y1 + 2;

	if (b == 1)
		focus();
	if (resizeable && x >= x2-10 && y >= y2-10)
	{
		resizing = true;
		mX = x;
		mY = y;
	}
	else
	if (icon && menu && (x > x1 && x < x1 + (int)icon->width && y >
			y1 + (titleY2-y1)/2 - (int)icon->height/2 &&
			y < y1 + (titleY2-y1)/2 - (int)icon->height/2 +
			(int)icon->height))
	{
		menu->run(x1 + 4, y1 + (titleY2-y1)/2 + icon->height -
			icon->height/2);
		return;
	}
	else
	{
		moving = true;
		mX = x-x1;
		mY = y-y1;
	}
	setMouseTrackingControl(this);
}

void Window::onMouseMoved(int x, int y)
{
	if (moving)
		move(x-mX, y-mY);
	if (resizing)
	{
		resize((x2-x1+1)+x-mX, (y2-y1+1)+y-mY);
		mX = x;
		mY = y;
	}
}

void Window::onMouseUp(int x, int y, int b)
{
 	if (!(moving || resizing))
 		return;
	moving = resizing = false;
	setMouseTrackingControl(NULL);
}
