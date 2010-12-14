/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_LISTBOXES_H_INCLUDED__
#define __BSGUI_LISTBOXES_H_INCLUDED__

struct ListboxItem : public Control
{
	struct Listbox  *box;
	char            *text;
	unsigned int    index;
	unsigned int    minimumWidth;
	unsigned int    itemHeight;
	void            *data;

	ListboxItem(Control *parent);
	virtual ~ListboxItem();
	
	virtual void setText(char *newText);
	
	virtual void render();
	virtual void focus();
};

struct Listbox : public Scrollbox
{
	ListboxItem     *active;

	Listbox(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~Listbox();
	
	virtual void addItem(char *text);
	virtual void addItem(ListboxItem *item);
	virtual ListboxItem *findItem(char *text);
	virtual ListboxItem *getItem(unsigned int index);
	virtual void removeItem(ListboxItem *item);
	virtual void selectItem(unsigned int index);
	virtual void selectItem(ListboxItem *item);
	
	virtual void layout();
};

#endif
