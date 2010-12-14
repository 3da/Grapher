/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_DRAW_H_INCLUDED__
#define __BSGUI_DRAW_H_INCLUDED__

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB	0x84F5
#endif

namespace BSGUIDraw
{
	enum FrameStyle
	{
		FS_NONE,
		FS_FLAT,
		FS_RAISED,
		FS_LOWERED
	};

	void enableOrtho();
	void disableOrtho();
	
	void color(int r, int g, int b, int a=255);
	
	void drawRect(int x1, int y1, int x2, int y2);
	void fillRect(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawFrame(int x1, int y1, int x2, int y2,
 		FrameStyle s=FS_FLAT);
 		
	bool loadCursorImage(char *colorFile, char *alphaFile);
	void drawCursor(int x, int y);

	bool loadFontData(char *bitmapFile, char *sizeFile);
	void drawString(int x, int y, char *str, int length=-1);
	int stringWidth(char *str, int length=-1);
	int stringHeight();
	
	void setClipArea(int x1, int y1, int x2, int y2);
	
	void resetClipping();
	void openClipArea(int x1, int y1, int x2, int y2);
	void closeClipArea();
}

#endif
