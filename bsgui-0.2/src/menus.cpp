/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

MenuItem::MenuItem(Control *owner, char *caption, Action *clickedAction,
	Bitmap *bitmap)
	: Control(owner)
{
	this->caption = strdup(caption); TRACKALLOC(this->caption);
	this->bitmap = bitmap;
	clicked = clickedAction;
	subMenu = NULL;
	popup = NULL;
}

MenuItem::~MenuItem()
{
	if (subMenu)
		DELETE(subMenu);
	FREE(caption);
	if (bitmap)
		DELETE(bitmap);
}

MenuItem *MenuItem::addItem(char *caption, Action *clicked, Bitmap *bitmap)
{
	if (!subMenu)
	{
	        subMenu = NEW(PopupMenu);
	        subMenu->rootMenuControl = menuControl;
	}
	return subMenu->addItem(caption, clicked, bitmap);
}

void MenuItem::clear()
{
	if (subMenu)
		subMenu->clear();
}

void MenuItem::render()
{
	int     x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	
	if (caption[0] == '-' && !caption[1])
	{
		BSGUIDraw::color(220, 234, 235);
		BSGUIDraw::drawLine(x1, y1 + (y2 - y1 + 1)/2, x2 + 1,
			y1 + (y2 - y1 + 1)/2);
		return;
	}
	
	if (mouseOverControl || (subMenu && subMenu->menu->parent))
	{
		BSGUIDraw::color(220, 234, 235);
		BSGUIDraw::fillRect(x1, y1, x2, y2);
		BSGUIDraw::color(20, 34, 35);
	}
	else
	        BSGUIDraw::color(220, 234, 235);
	
	BSGUIDraw::drawString(x1 + ((MenuControl*)parent)->iconPad + 5, y1 + 1,
		caption);
	if (bitmap)
		bitmap->draw(x1 + 5, (y2 - y1 + 1)/2 - bitmap->height/2 + y1);
}

void MenuItem::onMouseEnter()
{
	Control::onMouseEnter();
	for (Control *child = parent->firstChild;child;child = child->next)
	{
		if (((MenuItem*)child)->subMenu &&
			((MenuItem*)child)->subMenu->menu->parent)
		{
			((MenuItem*)child)->subMenu->cancel();
			break;
		}
	}
	
	if (subMenu || popup)
	{
		int     x1, y1, x2, y2;
		getBounds(x1, y1, x2, y2);
		BSGUI_RUNACTION(popup);
		subMenu->run(x2 - 10, y1 + 5);
	}
}

void MenuItem::onMouseUp(int x, int y, int b)
{
	if (b == 1)
		BSGUI_RUNACTION(clicked);
	if (b < 4)
		menuControl->popupMenu->cancel();
}


MenuControl::MenuControl(Control *owner)
	: Control(owner)
{
	Control::setPadding(2, 2, 2, 2);
	iconPad = 0;
}

MenuControl::~MenuControl()
{
}

void MenuControl::calcSize()
{
	int     	w = 10, h = 3;

	for (Control *child=firstChild;child;child = child->next)
	{
		if (w < BSGUIDraw::stringWidth(((MenuItem*)child)->caption)+10)
		        w = BSGUIDraw::stringWidth(((MenuItem*)child)->caption)
				+ 10;
		if (((MenuItem*)child)->caption[0] == '-' &&
		        !((MenuItem*)child)->caption[1])
			h += 4;
		else
			h += BSGUIDraw::stringHeight() + 3;
	}
	
	resize(w + (iconPad?(iconPad+5):0), h);
}

void MenuControl::layout()
{
	int     	w, h, y = 0;

	iconPad = 0;
	getClientSize(w, h);
	w--;
	for (Control *child=firstChild;child;child = child->next)
	{
		int     height;
		if (((MenuItem*)child)->bitmap &&
			iconPad < (int)((MenuItem*)child)->bitmap->width)
			iconPad = ((MenuItem*)child)->bitmap->width + 5;
		if (((MenuItem*)child)->caption[0] == '-' &&
		        !((MenuItem*)child)->caption[1])
			height = 4;
		else
			height = BSGUIDraw::stringHeight() + 3;
		child->place(0, y, w, y + height - 1);
		y += height;
	}

	Control::layout();
}

void MenuControl::render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	BSGUIDraw::color(120, 134, 135, 192);
	BSGUIDraw::fillRect(x1, y1, x2, y2);
	BSGUIDraw::color(220, 234, 235);
	BSGUIDraw::drawRect(x1, y1, x2, y2);
	glDisable(GL_BLEND);
	
	Control::render();
}

void MenuControl::onFocusExit()
{
	Control::onFocusExit();
	if (popupMenu->rootMenuControl)
		popupMenu->rootMenuControl->popupMenu->cancel();
	else
	        popupMenu->cancel();
}


PopupMenu::PopupMenu()
{
	rootMenuControl = NULL;
	menu = NEW(MenuControl, NULL);
	menu->popupMenu = this;
}

PopupMenu::~PopupMenu()
{
	if (!menu->parent)
	        DELETE(menu);
}

MenuItem *PopupMenu::addItem(char *caption, Action *clicked, Bitmap *bitmap)
{
	MenuItem        *item = NEW(MenuItem, menu, caption, clicked, bitmap);
	item->menuControl = rootMenuControl?rootMenuControl:menu;
	menu->calcSize();
	menu->layout();
	return item;
}

void PopupMenu::clear()
{
	menu->removeAllChildren();
}

void PopupMenu::run(int x, int y)
{
	if (menu->parent)
	        cancel();
	if (x == -10000)
	        x = screen->mouseX;
	if (y == -10000)
	        y = screen->mouseY;
	menu->calcSize();
	menu->layout();
	screen->addChild(menu);
	menu->move(x, y);
	menu->focus();
}

void PopupMenu::cancel()
{
	for (Control *child = menu->firstChild;child;child = child->next)
	{
		if (((MenuItem*)child)->subMenu &&
			((MenuItem*)child)->subMenu->menu->parent)
		{
			((MenuItem*)child)->subMenu->cancel();
			break;
		}
	}

	if (menu->parent)
	        menu->parent->removeChild(menu);
}
