/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

#include "bitmaps/new.c"
#include "bitmaps/open.c"
#include "bitmaps/save.c"
#include "bitmaps/saveas.c"
#include "bitmaps/quit.c"
#include "bitmaps/win.c"

#define GL_BGRA 0x80E1

// Bitmap

Bitmap::Bitmap(char *fname)
{
	SDL_Surface	*colorBmp = NULL;
	unsigned char	*source;
	unsigned char	*buffer;

	#define ADDBMP(d,s) \
		if (fname == INTERNALBMP_##d) \
		{ \
			source = (unsigned char*)bmp##s.pixel_data; \
			width = bmp##s.width; \
			height = bmp##s.height; \
		} \
		else

	ADDBMP(NEW, New)
	ADDBMP(OPEN, Open)
	ADDBMP(SAVE, Save)
	ADDBMP(SAVEAS, SaveAs)
	ADDBMP(QUIT, Quit)
	ADDBMP(WINICON, Win)
	{
		colorBmp = SDL_LoadBMP(fname);
		width = colorBmp->w;
		height = colorBmp->h;
		source = (unsigned char*)colorBmp->pixels;
	}

	#undef ADDBMP

	target = GL_TEXTURE_RECTANGLE_ARB;
	for (GLuint i=1,s=1;i<12;i++,s*=2)
	{
		if (width == s && height == s)
		{
			target = GL_TEXTURE_2D;
			break;
		}
	}

	buffer = (unsigned char*)malloc(width*height*4);
	unsigned int	c = 0;
	for (unsigned int y=0;y<height;y++)
	{
		for (unsigned x=0;x<width;x++,c++)
		{
			if (abs(source[c*3]-source[c*3 + 2]) < 10 &&
					source[c*3 + 1] < 20)
			{
				buffer[c*4] = buffer[c*4 + 1] = buffer[c*4 + 2]
					= 0;
				buffer[c*4 + 3] = 255 - source[c*3];
			}
			else
			{
				if (colorBmp)
				{
					buffer[c*4] = source[c*3];
					buffer[c*4 + 1] = source[c*3 + 1];
					buffer[c*4 + 2] = source[c*3 + 2];
				}
				else
				{
					buffer[c*4] = source[c*3 + 2];
					buffer[c*4 + 1] = source[c*3 + 1];
					buffer[c*4 + 2] = source[c*3];
				}
				buffer[c*4 + 3] = 0xFF;
			}
		}
	}

	glGenTextures(1, &name);
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(target, name);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(target, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
		buffer);
	if (colorBmp)
		SDL_FreeSurface(colorBmp);
	glBindTexture(target, 0);
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
	free(buffer);
	filtering = true;
}

Bitmap::~Bitmap()
{
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(target, name);
	glTexImage2D(target, 0, 0, 0, 0, 0, 0, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(target, 0);
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

void Bitmap::bind()
{
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(target, name);
}

void Bitmap::unbind()
{
	glBindTexture(target, 0);
	if (target == GL_TEXTURE_RECTANGLE_ARB)
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

void Bitmap::draw(int x, int y)
{
	draw(x, y, x + width, y + height);
}

void Bitmap::draw(int x1, int y1, int x2, int y2)
{
	bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	if (target == GL_TEXTURE_2D)
	{
		glTexCoord2i(0, 0);		glVertex2f(x1, y1);
		glTexCoord2i(1, 0);		glVertex2f(x2, y1);
		glTexCoord2i(1, 1);		glVertex2f(x2, y2);
		glTexCoord2i(0, 1);		glVertex2f(x1, y2);
	}
	else
	{
		glTexCoord2i(0, 0);		glVertex2f(x1, y1);
		glTexCoord2i(width, 0);		glVertex2f(x2, y1);
		glTexCoord2i(width, height);	glVertex2f(x2, y2);
		glTexCoord2i(0, height);	glVertex2f(x1, y2);
	}
	glEnd();
	glDisable(GL_BLEND);
	unbind();
}

void Bitmap::setFiltering(bool enabled)
{
	filtering = enabled;
	bind();
	if (filtering)
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	unbind();
}

// Image

Image::Image(Control *parent, int x, int y, char *fname)
	: Control(parent)
{
	int	x1 = x, y1 = y, x2, y2;
	if (fname)
	{
		bitmap = NEW(Bitmap, fname);
		x2 = x1 + bitmap->width - 1;
		y2 = y1 + bitmap->height - 1;
	}
	else
	{
		bitmap = NULL;
		x2 = x1 + 9;
		y2 = y1 + 9;
	}
	place(x1, y1, x2, y2);
	autoDeleteBitmap = true;
}

Image::~Image()
{
	if (bitmap && autoDeleteBitmap)
		DELETE(bitmap);
}

void Image::setBitmap(Bitmap *newBitmap, bool resize, bool autoDelete)
{
	if (bitmap && autoDeleteBitmap)
		DELETE(bitmap);
	bitmap = newBitmap;
	if (resize)
		this->resize(bitmap->width, bitmap->height);
	autoDeleteBitmap = autoDelete;
}

void Image::setBitmap(char *fname, bool resize)
{
	setBitmap(NEW(Bitmap, fname), resize, true);
}

void Image::render()
{
	int	x1, y1, x2, y2;
	getBounds(x1, y1, x2, y2);

	bitmap->draw(x1, y1, x2 + 1, y2 + 1);

	Control::render();
}
