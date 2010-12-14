/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

ListboxItem::ListboxItem(Control *parent)
	: Control(parent)
{
	box = (Listbox*)parent;
	text = strdup(""); TRACKALLOC(text);
	minimumWidth = 16;
	itemHeight = BSGUIDraw::stringHeight() + 5;
	data = NULL;
	selected = NULL;
}

ListboxItem::~ListboxItem()
{
	FREE(text);
	BSGUI_FREEACTION(selected);
}

void ListboxItem::setText(char *newText)
{
	FREE(text);
	text = strdup(newText); TRACKALLOC(text);
	minimumWidth = BSGUIDraw::stringWidth(newText) + 16;
}

void ListboxItem::render()
{
	int			x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);

	if (box->active == this)
	{
		BSGUIDraw::color(190, 204, 205);
		BSGUIDraw::fillRect(x1, y1, x2, y2);
		BSGUIDraw::color(80, 94, 95);
		if (box->focused())
			BSGUIDraw::drawRect(x1 + 2, y1 + 1, x2 - 1, y2 - 2);
	}
	else
	{
		if (mouseOverControl)
			BSGUIDraw::color(210, 224, 225);
		else
			BSGUIDraw::color(190, 204, 205);
	}
	BSGUIDraw::drawString(x1 + 8,
   		(y2-y1 + 1)/2 + y1 - BSGUIDraw::stringHeight()/2,
     		text);
}

void ListboxItem::focus()
{
	if (box->active != this)
	{
		box->active = this;
		BSGUI_RUNACTION(selected);
		BSGUI_RUNACTION_OF(box, box->modified);
	}
	box->focus();
}


Listbox::Listbox(Control *parent, int x1, int y1, int x2, int y2)
	: Scrollbox(parent, x1, y1, x2, y2)
{
	active = NULL;
}

Listbox::~Listbox()
{
}

void Listbox::addItem(char *text)
{
	(NEW(ListboxItem, this))->setText(text);
	layout();
}

void Listbox::addItem(ListboxItem *item)
{
	addChild(item);
}

ListboxItem *Listbox::findItem(char *text)
{
	for (ListboxItem *child=(ListboxItem*)firstChild;child;
		child = (ListboxItem*)child->next)
		if (!strcmp(child->text, text))
		        return child;

	return NULL;
}

ListboxItem *Listbox::getItem(unsigned int index)
{
	for (ListboxItem *child=(ListboxItem*)firstChild;child;
		child = (ListboxItem*)child->next)
		if (index == child->index)
		        return child;

	return NULL;
}

void Listbox::removeItem(ListboxItem *item)
{
	if (!item)
	        return;
	if (item == active)
	{
		if (item->next)
		        active = (ListboxItem*)item->next;
		else
		        active = (ListboxItem*)item->previous;
	}
	removeChild(item);
	DELETE(item);
}

void Listbox::selectItem(unsigned int index)
{
	selectItem(getItem(index));
}

void Listbox::selectItem(ListboxItem *item)
{
	if (!item || item == active)
		return;
	active = item;
	BSGUI_RUNACTION_OF(active, active->selected);
	BSGUI_RUNACTION(modified);
}

void Listbox::layout()
{
	int     	w, h, y = 0, i = 0;
	
	getClientSize(w, h);
	w--;
	for (ListboxItem *child=(ListboxItem*)firstChild;child;
		child = (ListboxItem*)child->next)
	        if ((int)child->minimumWidth > w)
	                w = child->minimumWidth;
	for (ListboxItem *child=(ListboxItem*)firstChild;child;
		child = (ListboxItem*)child->next)
	{
		child->index = i++;
		child->place(0, y, w, y + child->itemHeight - 1);
		y += child->itemHeight;
	}
	if (!active)
	        active = (ListboxItem*)firstChild;
	        
	Scrollbox::layout();
}
