/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Control::Control(Control *parent)
{
	x1 = y1 = x2 = y2 = padLeft = padTop = padRight = padBottom = xShift =
		yShift = 0;
	minWidth = minHeight = 0;
	maxWidth = maxHeight = 0x7FFFFFFF;
	name = NULL;
	firstChild = lastChild = this->parent = previous = next = NULL;
	if (parent)
	        parent->addChild(this);
        performLayout = true;
        mouseOverControl = false;
        popupMenu = NULL;
	exclusiveChild = NULL;
	clicked = modified = selected = moved = resized = NULL;
}

Control::~Control()
{
	invalidateControl(this);
	removeAllChildren();

	if (parent)
	{
		if (parent->exclusiveChild == this)
			parent->exclusiveChild = NULL;
		parent->removeChild(this);
	}
	
	BSGUI_FREEACTION(clicked);
	BSGUI_FREEACTION(modified);
	BSGUI_FREEACTION(selected);
	BSGUI_FREEACTION(moved);
	BSGUI_FREEACTION(resized);
}

void Control::setName(char *newName)
{
	if (name)
	        FREE(name);
	if (newName)
	{
	        name = strdup(newName); TRACKALLOC(name);
	}
	else
	        name = NULL;
}

Control *Control::findChild(char *name)
{
	if (!name || !name[0])
		return NULL;
	if (this->name && !strcmp(name, this->name))
		return this;
	for (Control *child=firstChild;child;child = child->next)
	{
		Control	*r = child->findChild(name);
		if (r)
			return r;
	}
	return NULL;
}

void Control::addChild(Control *child)
{
	if (!child)
		return;
	if (child->parent)
	        child->parent->removeChild(child);
	child->parent = this;
	child->previous = lastChild;
	child->next = NULL;
	if (lastChild)
	        lastChild->next = child;
	else
		firstChild = child;
	lastChild = child;
	performLayout = true;
}

void Control::removeChild(Control *child)
{
	if (!child || child->parent != this)
		return;
	child->parent = NULL;
	if (child->next)
		child->next->previous = child->previous;
	else
		lastChild = child->previous;
	if (child->previous)
		child->previous->next = child->next;
	else
		firstChild = child->next;
	child->previous = child->next = NULL;
	performLayout = true;
}

void Control::removeAllChildren()
{
	while (firstChild)
	{
		lastChild = firstChild->next;
		DELETE(firstChild);
		firstChild = lastChild;
	}
}

Control *Control::childAt(int x, int y)
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	if (!(x >= x1 + padLeft && y >= y1 + padTop && x <= x2 - padRight &&
		y <= y2 - padBottom))
		return this;
	for (Control *child = lastChild;child;child = child->previous)
	{
		child->getBounds(x1, y1, x2, y2);
		if (!(x >= x1 && y >= y1 && x <= x2 && y <= y2))
			continue;
		return child->childAt(x, y);
	}
	return this;
}

Screen *Control::getScreen()
{
	Control	*control = this;
	while (control->parent)
		control = control->parent;
	return (Screen*)control;
}

void Control::makeExclusive()
{
	if (parent)
		parent->exclusiveChild = this;
}

void Control::tick()
{
	if (performLayout)
	{
	        layout();
	        performLayout = false;
	}
	for (Control *child = firstChild;child;child = child->next)
	        child->tick();
}

void Control::render()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	
	BSGUIDraw::openClipArea(x1 + padLeft, y1 + padTop, x2 - padRight,
		y2 - padBottom);
	for (Control *child = firstChild;child;child = child->next)
	{
		if (child == exclusiveChild)
			continue;
		child->openClip();
		child->render();
		child->closeClip();
	}
	
	if (exclusiveChild)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		BSGUIDraw::color(0, 0, 0, 96);
		BSGUIDraw::fillRect(x1, y1, x2, y2);
		
		glDisable(GL_BLEND);
		
		exclusiveChild->openClip();
		exclusiveChild->render();
		exclusiveChild->closeClip();
	}
	
	BSGUIDraw::closeClipArea();
}

void Control::focus()
{
	if (!parent)
		return;
	parent->focus();
	
	if (parent->lastChild == this)
		return;
		
	Control *oldFocus = parent->lastChild;

	Control	*theParent = parent;
	parent->removeChild(this);
	parent = theParent;
	next = NULL;
	previous = parent->lastChild;
	if (parent->lastChild)
		parent->lastChild->next = this;
	else
		parent->firstChild = this;
	parent->lastChild = this;
	
	oldFocus->onFocusExit();
	onFocusEnter();
}

bool Control::focused()
{
	if (!parent)
		return true;
	return (parent->lastChild == this && parent->focused());
}

void Control::place(int x1, int y1, int x2, int y2)
{
	int	oldX1 = this->x1;
	int	oldY1 = this->y1;
	int	oldW = this->x2 - this->x1;
	int	oldH = this->y2 - this->y1;
	if (x2 - x1 + 1 < minWidth)
		x2 = x1 + minWidth - 1;
	if (y2 - y1 + 1 < minHeight)
		y2 = y1 + minHeight - 1;
	if (x2 - x1 + 1 > maxWidth)
		x2 = x1 + maxWidth - 1;
	if (y2 - y1 + 1 > maxHeight)
		y2 = y1 + maxHeight - 1;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	if (x2 - x1 != oldW || y2 - y1 != oldH)
	{
		performLayout = true;
		BSGUI_RUNACTION(resized);
	}
	if (x1 != oldX1 || y1 != oldY1)
		BSGUI_RUNACTION(moved);
}

void Control::move(int x, int y)
{
	int	dx = x2 - x1, dy = y2 - y1;
	place(x, y, x + dx, y + dy);
}

void Control::resize(int width, int height)
{
	place(x1, y1, x1 + width - 1, y1 + height - 1);
}

void Control::center(bool horizontal, bool vertical)
{
	int	w, h, pw, ph;
	if (!parent)
		return;
	
	parent->getClientSize(pw, ph);
	getClientSize(w, h);

	if (horizontal)
		move(pw/2 - w/2, y1);
	if (vertical)
		move(x1, ph/2 - h/2);
}

void Control::translate(int &x, int &y)
{
	for (Control *parent = this->parent;parent;parent=parent->parent)
	{
		x += parent->x1 + parent->padLeft;
		y += parent->y1 + parent->padTop;
	}
}

void Control::getBounds(int &x1, int &y1, int &x2, int &y2)
{
	x1 = this->x1 - xShift;
	y1 = this->y1 - yShift;
	x2 = this->x2 - xShift;
	y2 = this->y2 - yShift;
	translate(x1, y1);
	translate(x2, y2);
}

void Control::setPadding(int left, int top, int right, int bottom)
{
	if (left != -1)
		padLeft = left;
	if (top != -1)
		padTop = top;
	if (right != -1)
		padRight = right;
	if (bottom != -1)
		padBottom = bottom;
}

void Control::getClientSize(int &w, int &h)
{
	w = x2 - x1 - padLeft - padRight + 1;
	h = y2 - y1 - padTop - padBottom + 1;
}

void Control::openClip()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	BSGUIDraw::openClipArea(x1, y1, x2, y2);
}

void Control::closeClip()
{
	BSGUIDraw::closeClipArea();
}

void Control::drawOwnFrame()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	BSGUIDraw::drawFrame(x1, y1, x2, y2);
}

void Control::drawOwnFocus()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	BSGUIDraw::color(80, 94, 95);
	BSGUIDraw::drawRect(x1 + 5, y1 + 5, x2 - 5, y2 - 5);
}

void Control::onMouseEnter()
{
        mouseOverControl = true;
}

void Control::onMouseMoved(int x, int y)
{
}

void Control::onMouseExit()
{
        mouseOverControl = false;
}

void Control::onMouseDown(int x, int y, int b)
{
	if (b == 1)
	{
		setKeyboardFocusControl(this);
		focus();
		BSGUI_RUNACTION(clicked);
	}
	if (b == 3)
	{
		if (popupMenu)
		        popupMenu->run();
	}
}

void Control::onMouseUp(int x, int y, int b)
{
}

void Control::onKeyDown(int key, unsigned char ascii)
{
}

void Control::onKeyUp(int key, unsigned char ascii)
{
}
