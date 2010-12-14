/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_PROGRESSBARS_H_INCLUDED__
#define __BSGUI_PROGRESSBARS_H_INCLUDED__

struct ProgressBar : public Control
{
	DataManager<float>	max;
	DataManager<float>	value;
	
	ProgressBar(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~ProgressBar();
	
	virtual void setMax(float newMax);
	virtual void setValue(float newValue);
	
	virtual void render();
};

#endif
