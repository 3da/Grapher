/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Checkbox::Checkbox(Control *parent, int x1, int y1, int x2, int y2,
	char *caption)
	: Control(parent)
{
	place(x1, y1, x2, y2);
	this->caption = strdup(caption); TRACKALLOC(this->caption);
	hover = pushed = false;
	checked.setControl(this);
	checked.set(false);
}

Checkbox::~Checkbox()
{
	FREE(caption);
}

void Checkbox::setCaption(char *newCaption)
{
	FREE(caption);
	caption = strdup(newCaption);
}

void Checkbox::render()
{
	int			x1, y1, x2, y2;
	BSGUIDraw::FrameStyle	fs = BSGUIDraw::FS_FLAT;
	getBounds(x1, y1, x2, y2);

	if (hover)
		fs = pushed?BSGUIDraw::FS_LOWERED:BSGUIDraw::FS_RAISED;
	else
		fs = BSGUIDraw::FS_FLAT;

	BSGUIDraw::drawFrame(x1, y1 + 4, x1 + 12, y2 - 4, fs);

	if (checked.get())
	{
		BSGUIDraw::drawLine(x1, y1 + 4, x1 + 12, y2 - 4);
		BSGUIDraw::drawLine(x1 + 13, y1 + 4, x1, y2 - 3);
	}

	if (focused())
	{
		BSGUIDraw::color(50, 64, 65);
		BSGUIDraw::drawRect(x1 + 14, y1 + 4, x2, y2 - 4);
		BSGUIDraw::color(200, 214, 215);
	}
	else
		if (hover)
			BSGUIDraw::color(200, 214, 215);
		else
			BSGUIDraw::color(180, 194, 195);

	BSGUIDraw::drawString(x1 + 16,
   		(y2-y1 + 1)/2 + y1 - (int)BSGUIDraw::stringHeight()/2,
     		caption);
}

void Checkbox::setState(bool checked)
{
	if (checked != this->checked.get())
	{
		this->checked.set(checked);
		BSGUI_RUNACTION(modified);
	}
}

void Checkbox::onMouseDown(int x, int y, int b)
{
	focus();
	if (b == 1)
	{
		setMouseTrackingControl(this);
		pushed = true;
	}
}

void Checkbox::onMouseUp(int x, int y, int b)
{
	if (!focused())
		return;
	if (b == 1 && pushed)
	{
		setMouseTrackingControl(NULL);
		if (hover)
		{
			checked.set(!checked.get());
			if (modified)
				modified->run(this);
 		}
	}
	pushed = false;
}

void Checkbox::onMouseEnter()
{
	Control::onMouseEnter();
	hover = true;
}

void Checkbox::onMouseExit()
{
	Control::onMouseExit();
	hover = false;
}
