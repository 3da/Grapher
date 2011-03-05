#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

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

#include "version.h"

class Application
{
public:
	static int Run();


protected:
	static int InitVideo();
	static int SetupOpengl(int width, int height);
	static void Display();
	static void ZoomInAction(BSGUI::Control *sender);
	static void ZoomOutAction(BSGUI::Control *sender);
	static void CloseWindowAction(BSGUI::Control *sender);
	static void ShowInfoWindow();
	static void CreateUI();
	static int Register(lua_State *ls);
	static void Input();



	static bool running;
	static BSGUI::Window *win;
	static BSGUI::Screen *screen1;
	static BSGUI::Label *xLabel;
	static BSGUI::Label *yLabel;
	static BSGUI::Theme theme;
	static BSGUI::Font *font;

	static float zoom;
	static float xOffset;
	static float yOffset;
	static bool dragScreen;

	static const int width;
	static const int height;

	static lua_State *L;

	static std::vector<std::string> functions;




};

#endif // APPLICATION_H_INCLUDED
