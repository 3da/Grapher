/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

ProgressBar::ProgressBar(Control *parent, int x1, int y1, int x2, int y2)
	: Control(parent)
{
	max.setControl(this);
	value.setControl(this);
	max.set(100);
	value.set(0);
	place(x1, y1, x2, y2);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::setMax(float newMax)
{
	if (newMax < 0.0f)
		newMax = 0.0f;
	max.set(newMax);
	if (value.get() > newMax)
		value.set(newMax);
}

void ProgressBar::setValue(float newValue)
{
	if (newValue < 0)
		newValue = 0;
	if (newValue > max.get())
		newValue = max.get();
	value.set(newValue);
}

void ProgressBar::render()
{
	int	x1, y1, x2, y2, w;
	float	value = this->value.get();
	getBounds(x1, y1, x2, y2);
	
	BSGUIDraw::drawRect(x1, y1, x2, y2);
	
	if (value < 0.0f)
		value = 0.0f;
	if (value > max.get())
		value = max.get();
	
	if (max.get() == 0.0f || value == 0.0f)
		return;

	w = (int)((float)(x2-x1-2)*value/max.get());
	
	BSGUIDraw::fillRect(x1 + 1, y1 + 2, x1 + w, y2 - 1);
}
