/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MODELVIEWS_H_INCLUDED__
#define __BSGUI_MODELVIEWS_H_INCLUDED__

struct ModelView : public Renderer
{
	Action	*renderView;
	
	ModelView(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~ModelView();
	
	virtual void offscreenRender();
	virtual void render();
};

#endif
