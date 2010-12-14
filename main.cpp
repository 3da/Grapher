extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>

#include "SDL/SDL.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "bsgui/bsgui.h"
#undef main

bool	running = true;
Window	*win;
Screen	*screen1;
Label *xLabel;
Label *yLabel;



float zoom=1;
float xOffset=0;
float yOffset=0;
bool dragScreen=false;

const int width=640;
const int height=480;

lua_State *L;

std::vector<std::string> functions;

int setup_opengl(int width, int height);

void CloseWindowAction(Control *sender)
{
	if (win)
		delete win;
	win=0;
	xLabel=0;
	yLabel=0;
}

void ZoomInAction(Control *sender)
{
	zoom*=1.5;
}

void ZoomOutAction(Control *sender)
{
	zoom/=1.5;
}

void ShowInfoWindow()
{
	if (!win)
	{
		win = new Window("Coordinates");
		win->center();

		xLabel = new Label(win, 5, 0, "X = ");
		yLabel = new Label(win, 5, 15, "Y = ");

		(new Button(win, 10, 85, 180, 110, "Close"))->clicked = new CallbackAction(CloseWindowAction);
		(new Button(win, 10, 45, 80, 70, "ZoomIn"))->clicked = new CallbackAction(ZoomInAction);
		(new Button(win, 100, 45, 180, 70, "ZoomOut"))->clicked = new CallbackAction(ZoomOutAction);
	}
}

void CreateUI()
{
	BSGUIDraw::loadFontData("fontdata.bmp", "fontsize.dat");
	BSGUIDraw::loadCursorImage("cursor.bmp", "cursorAlpha.bmp");



	screen1 = screen;
}

int Register(lua_State *ls)
{
	int argc = lua_gettop(ls);
	if (argc!=1)
	{
		fprintf(stderr,"Register: wrong number of arguments!\n");
		// Вернем 0 в Lua скрипт
		lua_pushnumber(ls, 0);
		// Количество возвращаемых значений
    	return 1;
	}

	if(!lua_isstring(ls, 1))
	{
		fprintf(stderr, "invalid arguments!\n");
		lua_pushnumber(ls, 0);

		// Количество возвращаемых значений
		return 1;
	}

	functions.push_back(lua_tostring(ls, 1));
	lua_pushnumber(ls, 1);
	return 1;
}

int InitVideo()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
  }
  atexit(SDL_Quit); // Clean it up nicely :)

  //To use OpenGL, you need to get some information first,
  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  if(!info)
  {
    /* This should never happen, if it does PANIC! */
    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
    return false;
  }
  //int bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_ShowCursor(false);
    SDL_WM_SetCaption("Grapher 0.1.0", "Grapher");

  if (SDL_SetVideoMode(width, height, 0, SDL_OPENGL) == 0)
  {
    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    return false;
  }

	return setup_opengl(width, height);
}

int setup_opengl(int width, int height)
{
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	return true;
}


void Input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (!handleSDLEvent(&e))
			switch(e.type)
			{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
						break;
					/*case SDLK_LEFT:
						xOffset+=20/zoom;
						break;
					case SDLK_RIGHT:
						xOffset-=20/zoom;
						break;
					case SDLK_UP:
						yOffset-=20/zoom;
						break;
					case SDLK_DOWN:
						yOffset+=20/zoom;
						break;
					case SDLK_a:
						zoom/=0.9;
						break;
					case SDLK_d:
						zoom*=0.9;
						break;*/
					case SDLK_i:
						ShowInfoWindow();
						break;
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_MOUSEMOTION:
				if (win)
				{
					char buf[256];
					float x = (e.motion.x-width/2-xOffset)/zoom;
					float y = -(e.motion.y-height/2+yOffset)/zoom;
					sprintf(buf, "X = %.2f", x);
					xLabel->setText(buf);
					sprintf(buf, "Y = %.2f", y);
					yLabel->setText(buf);
				}
				if (dragScreen)
				{
					xOffset+=e.motion.xrel;
					yOffset-=e.motion.yrel;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				dragScreen=true;
				break;
			case SDL_MOUSEBUTTONUP:
				dragScreen=false;
				break;
			}
	}
}


void Display()
{

	renderOffscreenBSGUIControls();

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(xOffset, yOffset,0);
	glColor3f(1, 1, 1);

	glPushMatrix();
	glTranslatef(0,-yOffset,0);
	glBegin(GL_LINES);
	glVertex2f(0, -height/2);
	glVertex2f(0, height/2);
	glVertex2f(0, height/2);
	glVertex2f(5, (height/2-5));
	glVertex2f(0, height/2);
	glVertex2f(-5, (height/2-5));
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-xOffset,0,0);
	glBegin(GL_LINES);
	glVertex2f(-width/2, 0);
	glVertex2f(width/2, 0);
	glVertex2f(width/2, 0);
	glVertex2f((width/2-5), 5);
	glVertex2f(width/2, 0);
	glVertex2f((width/2-5), -5);
	glEnd();
	glPopMatrix();

	glScalef(zoom, zoom, 1);

	for (int i = 0; i < functions.size(); i++)
	{
		glColor3f(i%2, (i+1)%2, (i+2)%3);
		glBegin(GL_LINE_STRIP);
		for (float x = (-width/2-xOffset)/zoom; x<(width/2-xOffset)/zoom; x+=1.0/zoom)
		{
			lua_getfield(L, LUA_GLOBALSINDEX, functions[i].c_str());
			lua_pushnumber(L, x);
			lua_pcall(L, 1, 1, 0);
			float y = lua_tonumber(L, -1);
			glVertex2f(x, y);
			//printf("%.5f\n", );
		}
		glEnd();
	}



	renderBSGUI();

	glFinish();

	SDL_GL_SwapBuffers();

}


int main(int argc, char **argv)
{
	char buff[256];
	int error;
	L = lua_open();
	luaopen_base(L);
	luaopen_table(L);
	//luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	lua_register(L, "Register", Register);

	int status = luaL_loadfile(L, "config.lua");
	if (status)
	{
		fprintf(stderr, "Error: unable to load file");
		return 1;
	}
	status = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (status)
	{
		fprintf(stderr, "Error: runtime error");
		return 1;
	}





	if (InitVideo() == false)
		return 1;

	initBSGUI();

	CreateUI();

	SDL_EnableKeyRepeat(500,100);
	SDL_EnableUNICODE(true);

    while(true)
    {

    	tickBSGUI();
    	Input();

		glLoadIdentity();
    	Display();

    	SDL_GL_SwapBuffers();
    	SDL_Delay(10);
    }

	shutdownBSGUI();
	SDL_Quit();

	lua_close(L);
	return 0;
}


