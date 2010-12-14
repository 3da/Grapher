/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

ModelView::ModelView(Control *parent, int x1, int y1, int x2, int y2)
	: Renderer(parent)
{
	place(x1, y1, x2, y2);
	renderView = NULL;
}

ModelView::~ModelView()
{
	BSGUI_FREEACTION(renderView);
}

void ModelView::offscreenRender()
{
	BSGUI_RUNACTION(renderView);
}

void ModelView::render()
{
	drawOffscreenRender();
}
