/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_RENDERERS_H_INCLUDED__
#define __BSGUI_RENDERERS_H_INCLUDED__

struct Renderer : public Control
{
	GLuint			texture;
	void			*rendererListEntry;
	bool			enabled;
	
	Renderer(Control *parent);
	virtual ~Renderer();
	
	virtual void place(int x1, int y1, int x2, int y2);
	virtual void offscreenRender();
	virtual void drawOffscreenRender();
};

#endif
