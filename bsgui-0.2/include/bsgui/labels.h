/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_LABELS_H_INCLUDED__
#define __BSGUI_LABELS_H_INCLUDED__

struct Label : public Control
{
	char	*text;
	
	Label(Control *parent, int x, int y, char *text);
	virtual ~Label();
	
	virtual void setText(char *newText);
	
	virtual void render();
};

#endif

