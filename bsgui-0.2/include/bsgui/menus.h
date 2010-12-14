/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MENUS_H_INCLUDED__
#define __BSGUI_MENUS_H_INCLUDED__

struct MenuItem : public Control
{
	char    		*caption;
	Bitmap			*bitmap;
	Action                  *popup;
	struct MenuControl      *menuControl;
	struct PopupMenu        *subMenu;
	
	MenuItem(Control *owner, char *caption, Action *clickedAction,
		Bitmap *bitmap=NULL);
	virtual ~MenuItem();
	
	virtual MenuItem *addItem(char *caption, Action *clicked,
		Bitmap *bitmap=NULL);
	virtual void clear();

	virtual void render();
	virtual void onMouseEnter();
	virtual void onMouseDown(int x, int y, int b){};
	virtual void onMouseUp(int x, int y, int b);
};

struct MenuControl : public Control
{
	struct PopupMenu        *popupMenu;
	int			iconPad;

	MenuControl(Control *owner);
	virtual ~MenuControl();
	
	virtual void calcSize();

	virtual void layout();
	virtual void render();

	virtual void onFocusExit();
};

struct PopupMenu
{
	MenuControl     *rootMenuControl;
	MenuControl     *menu;
	
	PopupMenu();
	virtual ~PopupMenu();
	
	virtual MenuItem *addItem(char *caption, Action *clicked,
		Bitmap *bitmap=NULL);
	virtual void clear();
	virtual void run(int x=-10000, int y=-10000);
	virtual void cancel();
};

#endif
