/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

static bool	running;
Action		*terminateBSGUIApplicationAction = NULL;

static void terminateBSGUI(Control *sender)
{
	running = false;
}

static void initOpenGL(char *caption, int width, int height, bool fullscreen)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(width, height, 32, SDL_OPENGL|
		(fullscreen?SDL_FULLSCREEN:0));
	SDL_WM_SetCaption(caption, caption);
	SDL_ShowCursor(false);
	
	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
		SDL_DEFAULT_REPEAT_INTERVAL);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, (float)width/(float)height, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
}

static void render()
{
	renderOffscreenBSGUIControls();

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

 	renderBSGUI();
 	
  	SDL_GL_SwapBuffers();
}

bool initBSGUIApplication(char *caption, int width, int height, bool fullscreen)
{
	initOpenGL(caption, width, height, fullscreen);
	if (!initBSGUI())
	{
		SDL_Quit();
		return false;
	}
	BSGUIDraw::loadFontData("data/fontdata.bmp", "data/fontsize.dat");
	BSGUIDraw::loadCursorImage("data/cursor.bmp", "data/cursorAlpha.bmp");
	terminateBSGUIApplicationAction = new CallbackAction(terminateBSGUI);
	terminateBSGUIApplicationAction->autoDelete = false;
	return true;
}

void runBSGUIApplication(void (*idleFunc)())
{
	SDL_Event	event;
	
	running = true;
	while (running)
	{
		if (idleFunc)
			idleFunc();
		tickBSGUI();
		while (SDL_PollEvent(&event))
		{
			if (!handleSDLEvent(&event))
				switch (event.type)
				{
					case SDL_QUIT:
						running = false;
					break;
				}
		}
		render();
	}
}

void terminateBSGUIApplication(void)
{
	running = false;
}

void shutdownBSGUIApplication()
{
	delete terminateBSGUIApplicationAction;
	shutdownBSGUI();
	SDL_Quit();
}
