/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Inputbox::Inputbox(Control *parent, int x1, int y1, int x2, int y2)
	: Control(parent)
{
	text.setControl(this);
	text.set(strdup(""));	TRACKALLOC(text);
	place(x1, y1, x2, y2);
	tScroll = 0;
	cursor = cursorX = 0;
}

Inputbox::~Inputbox()
{
	FREE(text.get());
}

void Inputbox::setText(char *newText)
{
	int     w, h;
	getClientSize(w, h);
	FREE((char*)text.get());
	text.set(strdup(newText)); TRACKALLOC(text);
	cursor = strlen(newText);
	cursorX = BSGUIDraw::stringWidth(newText);
	tScroll = 0;
	if (cursorX - tScroll > w - 10)
		tScroll = cursorX - w + (w/2);
}

char *Inputbox::getText()
{
	return (char*)text.get();
}

void Inputbox::render()
{
	int			x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);
	BSGUIDraw::color(120, 134, 135);
	BSGUIDraw::fillRect(x1, y1 + 1, x2 - 1, y2);
	
	BSGUIDraw::color(20, 34, 35);
	BSGUIDraw::drawString(x1 + 5 - tScroll, y1 + (y2 - y1)/2 -
		BSGUIDraw::stringHeight()/2, (char*)text.get());

	if (hasKeyboardFocus(this))
	{
		BSGUIDraw::drawLine(x1 + 5 + cursorX - tScroll, y1 + 4,
			x1 + 5 + cursorX - tScroll, y2 - 4);
	}

	if (mouseOverControl)
		BSGUIDraw::color(210, 224, 225);
	else
		BSGUIDraw::color(170, 184, 185);
	BSGUIDraw::drawRect(x1, y1, x2, y2);
}

void Inputbox::onKeyDown(int key, unsigned char ascii)
{
	int     w, h;
	char	*text = (char*)this->text.get();
	getClientSize(w, h);
	switch (ascii)
	{
		case 8:
			if (*text)
			{
				text[strlen(text) - 1] = 0;
				text = (char*)REALLOC(text, strlen(text) + 1);
				cursorX = BSGUIDraw::stringWidth(text,--cursor);
				if (cursorX - tScroll < 0)
				{
					tScroll = cursorX - (w/2);
					if (tScroll < 0)
					        tScroll = 0;
				}
			}
			break;
		default:
			if (ascii < 32 || ascii > 127)
			        break;
			text = (char*)REALLOC(text, strlen(text) + 2);
			text[strlen(text) + 1] = 0;
			text[strlen(text)] = ascii;
			cursorX = BSGUIDraw::stringWidth(text, ++cursor);
			if (cursorX - tScroll > w - 10)
				tScroll = cursorX - w + (w/2);
			break;
	}
	this->text.set(text);
}

void Inputbox::onKeyUp(int key, unsigned char ascii)
{
}
