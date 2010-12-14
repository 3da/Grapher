/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

static void closeMessageBox(Control *sender)
{
	DELETE(sender->parent);
}

void messageBox(char *msg, char *caption, bool exclusive)
{
	Window	*w = NEW(Window, caption);
	Label	*l = NEW(Label, w, 20, 20, msg);
	Button	*b = NEW(Button, w, 0, 0, 100, 25, "Ok");
	w->resize((l->x2 - l->x1) + 40, 80);
	l->center();
	b->move(0, l->y2 + 15);
	b->center(true, false);
	w->resize(w->x2 - w->x1 + 1, w->y2 - w->y1 + 16 + b->y2 - b->y1);
	b->clicked = NEW(CallbackAction, closeMessageBox);
	if (exclusive)
		w->makeExclusive();
	w->center();
}
