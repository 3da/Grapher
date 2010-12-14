/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SLIDERS_H_INCLUDED__
#define __BSGUI_SLIDERS_H_INCLUDED__

struct Slider : public Control
{
	float			max;
	DataManager<float>	value;
	bool			sliding;

	Slider(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~Slider();
	
	virtual void render();
	
	virtual void setValue(float newValue);
	virtual void setMax(float newMax);
	
	virtual void onMouseDown(int x, int y, int b);
	virtual void onMouseMoved(int x, int y);
	virtual void onMouseUp(int x, int y, int b);
};

#endif
