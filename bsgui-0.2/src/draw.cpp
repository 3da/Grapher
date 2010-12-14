/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

struct CharInfo
{
	float	u1;
	float	v1;
	float	u2;
	float	v2;
	float	w;
};

struct ClipArea
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	ClipArea	*next;
};

static int	orthoEnableCount = 0;
static GLuint	fontTexture = 0, fontHeight;
static GLuint	cursorTexture = 0;
static CharInfo fontChar[256];
static ClipArea	*carea = NULL;

void BSGUIDraw::enableOrtho()
{
	orthoEnableCount++;
	if (orthoEnableCount != 1)
		return;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

  	glLoadIdentity();
	glOrtho(0, screen->width, screen->height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void BSGUIDraw::disableOrtho()
{
	orthoEnableCount--;
	if (orthoEnableCount != 0)
		return;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
 	glMatrixMode(GL_MODELVIEW);
 	glPopMatrix();
}

void BSGUIDraw::color(int r, int g, int b, int a)
{
	glColor4f((float)r/255.0f, (float)g/255.0f, (float)b/255.0f,
		(float)a/255.0f);
}

void BSGUIDraw::drawRect(int x1, int y1, int x2, int y2)
{
	y1++;
	y2++;
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glVertex2f(x1, y1);
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}

void BSGUIDraw::fillRect(int x1, int y1, int x2, int y2)
{
	x1++;
	x2++;
	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}


void BSGUIDraw::drawLine(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void BSGUIDraw::drawFrame(int x1, int y1, int x2, int y2, FrameStyle s)
{
	BSGUIDraw::color(100, 114, 115);
	if (!s)
	{
		fillRect(x1, y1, x2, y2);
		return;
	}
	fillRect(x1, y1, x2, y2);
	switch (s)
	{
		case FS_FLAT:
			BSGUIDraw::color(170, 184, 185);
			drawRect(x1, y1, x2, y2);
			break;
		case FS_RAISED:
			BSGUIDraw::color(210, 224, 225);
			drawRect(x1, y1, x2, y2);
			break;
		case FS_LOWERED:
			BSGUIDraw::color(70, 84, 85);
			drawRect(x1, y1, x2, y2);
			break;
		default:
			break;
	}
}

bool BSGUIDraw::loadFontData(char *bitmapFile, char *sizeFile)
{
	SDL_Surface	*bmp;
	FILE		*f = fopen(sizeFile, "rb");
	int		sizeSize;
	char		*size;
	unsigned char	*fnt;
	
	if (!f)
	{
		fprintf(stderr, "Cannot open %s\n", sizeFile);
		return false;
	}
	if (!fontTexture)
	{
		glGenTextures(1, &fontTexture);
	}
	bmp = SDL_LoadBMP(bitmapFile);
	if (!bmp)
	{
		fprintf(stderr, "Cannot open %s\n", bitmapFile);
		fclose(f);
		return false;
	}
	
	fnt = (unsigned char*)MALLOC(bmp->w*bmp->h*4);
	for (int i=0;i<bmp->w*bmp->h;i++)
	{
		fnt[i*4] = ((unsigned char*)bmp->pixels)[i*3]&0xFF;
		fnt[i*4+1] = ((unsigned char*)bmp->pixels)[i*3]&0xFF;
		fnt[i*4+2] = ((unsigned char*)bmp->pixels)[i*3]&0xFF;
		fnt[i*4+3] = ((unsigned char*)bmp->pixels)[i*3]&0xFF;
	}

	fseek(f, 0, SEEK_END);
	sizeSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	size = (char*)MALLOC(sizeSize+1);
	fread(size, 1, sizeSize, f);
	size[sizeSize] = 0;
	fclose(f);

	memset((void*)fontChar, 0, sizeof(fontChar));
	float 	fheight = atof(strtok(size, " "));
	float 	ftsize = atof(strtok(NULL, " "));
	fontHeight = (GLuint)fheight;
	for (int i=33;i<128;i++)
	{
		float 	x = atoi(strtok(NULL, " "));
		float 	y = atoi(strtok(NULL, " "));
		float 	w = atoi(strtok(NULL, " "));
		fontChar[i].u1 = x/(ftsize);
		fontChar[i].v1 = y/(ftsize);
		fontChar[i].u2 = (x + w)/(ftsize);
		fontChar[i].v2 = (y + fheight)/(ftsize);
		fontChar[i].w = w;
	}
	
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp->w, bmp->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, fnt);

	SDL_FreeSurface(bmp);
  	FREE(size);
  	FREE(fnt);
	return true;
}

bool BSGUIDraw::loadCursorImage(char *colorFile, char *alphaFile)
{
	SDL_Surface	*colorBmp, *alphaBmp;
	unsigned char	*buffer;
	
	colorBmp = SDL_LoadBMP(colorFile);
	if (!colorBmp)
		return false;
	alphaBmp = SDL_LoadBMP(alphaFile);
	if (!alphaBmp)
	{
		SDL_FreeSurface(colorBmp);
		return false;
	}
	
	buffer = (unsigned char*)MALLOC(32*32*4);
	for (int i=0;i<32*32;i++)
	{
		buffer[i*4] = ((unsigned char*)colorBmp->pixels)[i*3];
		buffer[i*4 + 1] = ((unsigned char*)colorBmp->pixels)[i*3 + 1];
		buffer[i*4 + 2] = ((unsigned char*)colorBmp->pixels)[i*3 + 2];
		buffer[i*4 + 3] = ((unsigned char*)alphaBmp->pixels)[i*3];
	}
	
	SDL_FreeSurface(colorBmp);
	SDL_FreeSurface(alphaBmp);

	if (!cursorTexture)
	{
		glGenTextures(1, &cursorTexture);
	}
	glBindTexture(GL_TEXTURE_2D, cursorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		buffer);
	
	FREE(buffer);
	
	return true;
}

void BSGUIDraw::drawCursor(int x, int y)
{
	bool 	blendEnabled = glIsEnabled(GL_BLEND);
	int	blendSrc, blendDst;
	if (!blendEnabled)
		glEnable(GL_BLEND);
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, cursorTexture);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);	glVertex2f(x, y);
	glTexCoord2f(1.0f, 0.0f);	glVertex2f(x + 31.0f, y);
	glTexCoord2f(1.0f, 1.0f);	glVertex2f(x + 31.0f, y + 31.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex2f(x, y + 31.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBlendFunc(blendSrc, blendDst);
	if (!blendEnabled)
		glDisable(GL_BLEND);
}

void BSGUIDraw::drawString(int x, int y, char *str, int length)
{
	float	cx = x;
	bool 	blendEnabled = glIsEnabled(GL_BLEND);
	int	blendSrc, blendDst;
	if (length == -1)
		length = strlen(str);

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	if (!blendEnabled)
		glEnable(GL_BLEND);
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
//	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	for (int i=0;i<length;i++,str++)
	{
		if (*str == ' ')
		{
			cx += 5;
			continue;
		}
		glTexCoord2f(fontChar[(int)*str].u1, fontChar[(int)*str].v1);
		glVertex2f(cx, y);
		glTexCoord2f(fontChar[(int)*str].u2, fontChar[(int)*str].v1);
		glVertex2f((cx+fontChar[(int)*str].w),
  			y);
		glTexCoord2f(fontChar[(int)*str].u2, fontChar[(int)*str].v2);
		glVertex2f((cx+fontChar[(int)*str].w),
  			(y+fontHeight));
		glTexCoord2f(fontChar[(int)*str].u1, fontChar[(int)*str].v2);
		glVertex2f(cx, (y+fontHeight));
		
		cx += fontChar[(int)*str].w + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBlendFunc(blendSrc, blendDst);
	if (!blendEnabled)
		glDisable(GL_BLEND);
}

int BSGUIDraw::stringWidth(char *str, int length)
{
	float	cx = 0;
	if (length == -1)
		length = strlen(str);

	for (int i=0;i<length;i++,str++)
	{
		if (*str == ' ')
		{
			cx += 5;
			continue;
		}
		cx += fontChar[(int)*str].w + 1;
	}
	return (int)cx;
}

int BSGUIDraw::stringHeight()
{
	return fontHeight;
}

void BSGUIDraw::setClipArea(int x1, int y1, int x2, int y2)
{
	glScissor(x1, screen->height - y2 - 2, x2 - x1+1, y2 - y1+2);
}

void BSGUIDraw::resetClipping()
{
	ClipArea	*ca;
	glScissor(0, 0, screen->width, screen->height);
	while (carea)
	{
		ca = carea->next;
		DELETE(carea);
		carea = ca;
	}
}

void BSGUIDraw::openClipArea(int x1, int y1, int x2, int y2)
{
	ClipArea	*ca = NEW(ClipArea);
	if (carea)
	{
		if (x1 < carea->x1)
			x1 = carea->x1;
		if (y1 < carea->y1)
			y1 = carea->y1;
		if (x2 > carea->x2)
			x2 = carea->x2;
		if (y2 > carea->y2)
			y2 = carea->y2;
	}
	ca->x1 = x1;
	ca->y1 = y1;
	ca->x2 = x2;
	ca->y2 = y2;
	ca->next = carea;
	carea = ca;
	setClipArea(x1, y1, x2, y2);
}

void BSGUIDraw::closeClipArea()
{
	ClipArea	*ca;
	if (!carea)
	{
		glScissor(0, 0, screen->width, screen->height);
		return;
	}
	ca = carea->next;
	DELETE(carea);
	carea = ca;
	if (carea)
		setClipArea(ca->x1, ca->y1, ca->x2, ca->y2);
	else
		glScissor(0, 0, screen->width, screen->height);
}
