/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Label::Label(Control *parent, int x, int y, char *text)
	: Control(parent)
{
	move(x, y);
	resize(BSGUIDraw::stringWidth(text), BSGUIDraw::stringHeight());
	this->text = strdup(text); TRACKALLOC(this->text);
}

Label::~Label()
{
	FREE(text);
}

void Label::setText(char *newText)
{
	FREE(text);
	text = strdup(newText); TRACKALLOC(text);
	resize(BSGUIDraw::stringWidth(text), BSGUIDraw::stringHeight());
}

void Label::render()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	
	BSGUIDraw::color(180, 194, 195);
	BSGUIDraw::drawString(x1, y1, text);
}

