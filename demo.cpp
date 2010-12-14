/*
** Bad Sector's OpenGL GUI **DEMO**
*/

#include <bsgui/bsgui.h>
#include <math.h>
//#undef main

bool	running = true;
float	rotation = 0.0f, size = 1.0f, backgnd = 0.2f, progress = 0.0f;
int	steps = 5;
//Slider	*stepCount, *sizeSlider, *backSlider;
Window	*win;
Screen	*screen1/*, *screen2*/;
//Label	*fps;

/*void stepsModifiedAction(Control *sender)
{
	ProgressBar	*pbar2 = (ProgressBar*)screen->findChild("pbar2");
	steps = (int)stepCount->value.get() + 3;
	pbar2->setValue(steps - 3);
}

void terminateAppAction(Control *sender)
{
	running = false;
}

void closeWindowAction(Control *sender)
{
	delete sender->parent;
}

void newWindowAction(Control *sender)
{
	Window	*w = new Window("A simple window");
	w->place(10, 10, 300, 100);
	w->move(rand()%600+30, rand()%500+30);

	Button	*button = new Button(w, 10, 20, 280, 55, "Close me");
	button->clicked = new CallbackAction(closeWindowAction);
}

void addWideItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)screen->findChild("listbox");
	lbox->addItem("A very wide item to test horizontal scrolling");
}

void removeItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)screen->findChild("listbox");
	lbox->removeItem(lbox->active);
}

void addTheAboveTextAction(Control *sender)
{
	Listbox *lbox = (Listbox*)screen->findChild("listbox");
	lbox->addItem(((Inputbox*)screen->findChild("inputbox"))->getText());
	((Inputbox*)screen->findChild("inputbox"))->setText("");
	screen->findChild("inputbox")->focus();
}

void createDynamicSubmenuAction(Control *sender)
{
	MenuItem        *sub = (MenuItem*)sender;
	Listbox         *lbox = (Listbox*)screen1->findChild("listbox");
	sub->clear();
	for (ListboxItem *item = (ListboxItem*)lbox->firstChild;item;
		item = (ListboxItem*)item->next)
		sub->addItem(item->text, NULL);
}

void activateScreen1(Control *sender)
{
	screen1->activate();
}

void activateScreen2(Control *sender)
{
	screen2->activate();
}
*/
/*void renderObject()
{
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);
	float	a = 0.0f, step;
	step = (360.0f/(float)steps)*3.14f/180.0f;
	for (int i=0;i<steps;i++,a+=step)
	{
		glColor3f(i/(float)steps, i/((float)steps+1.0f),
  			i/((float)steps+2.0f));
		glVertex3f(0.0f, size, 0.0f);
		glVertex3f(sinf(a)*size, 0.0f, cosf(a)*size);
		glVertex3f(sinf(a+step)*size, 0.0f, cosf(a+step)*size);
	}
	a = 0.0f;
	for (int i=0;i<steps;i++,a+=step)
	{
		glColor3f(i/(float)steps, i/((float)steps+1.0f),
  			i/((float)steps+2.0f));
		glVertex3f(0.0f, -size, 0.0f);
		glVertex3f(sinf(a)*size, 0.0f, cosf(a)*size);
		glVertex3f(sinf(a+step)*size, 0.0f, cosf(a+step)*size);
	}
	glEnd();
	glEnable(GL_CULL_FACE);
}

void renderObjectAction(Control *sender)
{
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 0.0f, 1.0f, 0.0f);
	renderObject();
}

void fileSelectedAction(Control *sender)
{
	char	*fname = ((FileBrowser*)sender)->getFilename();
	printf("File selected: %s\n", fname);
	free(fname);
}

void selectFileAction(Control *sender)
{
	FileBrowser	*browser = new FileBrowser("Select File");
	browser->selected = new CallbackAction(fileSelectedAction);
}

void aboutBoxAction(Control *sender)
{
	messageBox("BSGUI (Bad Sector's OpenGL GUI) version 0.2 Demo", "About");
}

void imageWinResizedAction(Control *sender)
{
	Image	*img = (Image*)sender->firstChild;
	int	w, h;
	sender->getClientSize(w, h);
	img->minWidth = 10;
	img->minHeight = 10;
	img->place(img->x1, img->y1, w-img->x1 - 1, h-img->y1 - 1);
}

void enableFiltering(Control *sender)
{
	Image	*img = (Image*)screen->findChild("cenda");
	img->bitmap->setFiltering(true);
}

void disableFiltering(Control *sender)
{
	Image	*img = (Image*)screen->findChild("cenda");
	img->bitmap->setFiltering(false);
}

void cendaClicked(Control *sender)
{
	messageBox("Hello, i am Cenda :-)");
}*/

void render()
{
	renderOffscreenBSGUIControls();

	glClearColor((146.0f/255.0f)*backgnd, (139.0f/255.0f)*backgnd,
		(125.0f/255.0f)*backgnd, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation/0.6f, 0.0f, 1.0f, 0.0f);

	//renderObject();

 	renderBSGUI();

  	SDL_GL_SwapBuffers();

	progress += 0.008;
}

void initOpenGL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
	SDL_WM_SetCaption("Bad Sector's OpenGL GUI Demo", "demo");
	SDL_ShowCursor(false);

	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
		SDL_DEFAULT_REPEAT_INTERVAL);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, 800.0f/600.0f, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
}

void createUI()
{
	BSGUIDraw::loadFontData("fontdata.bmp", "fontsize.dat");
	BSGUIDraw::loadCursorImage("cursor.bmp", "cursorAlpha.bmp");

	win = new Window("A fluffy window");
	win->center();

 	new Label(win, 5, 0, "A Label");

 	/*(new Button(win, 10, 25, 180, 50, "Create window",
		new Bitmap(INTERNALBMP_NEW)))->clicked =
		new CallbackAction(newWindowAction);*/
 	new Button(win, 10, 55, 180, 80, "Another button");
 	new Button(win, 10, 85, 180, 110, "Yet another button");

	/*Window	*win2 = new Window("Another fluffy window");
	win2->center();
	win2->move(win2->x1 + 200, win2->y1 - 70);
	win2->resize(win2->x2-win2->x1+1, win2->y2-win2->y1+270);

	(new Checkbox(win2, 10, 0, 180, 20, "Checker"))->setState(true);
	new Checkbox(win2, 10, 25, 180, 45, "Checker 2");
	new Checkbox(win2, 10, 50, 180, 70, "Checker 3");

	Listbox *lbox = new Listbox(win2, 10, 80, 175, 200);
	lbox->addItem("A string item");
	for (int i=0;i<20;i++)
	{
		char    buff[100];
		sprintf(buff, "Item %i of 20", i+1);
		lbox->addItem(buff);
	}
	lbox->setName("listbox");
	(new Button(win2, 10, 210, 175, 235, "Add wide item"))->clicked =
	        new CallbackAction(addWideItemAction);
	(new Button(win2, 10, 240, 175, 265, "Remove active item"))->clicked =
	        new CallbackAction(removeItemAction);
	(new Inputbox(win2, 10, 270, 175, 295))->setName("inputbox");
	(new Button(win2, 10, 300, 175, 325, "Add the above text"))->clicked =
	        new CallbackAction(addTheAboveTextAction);

	ProgressBar	*pbar = new ProgressBar(win2, 10, 330, 175, 355);
	pbar->value.setDataSource(&progress);
	ProgressBar	*pbar2 = new ProgressBar(win2, 10, 360, 175, 385);
	pbar2->setName("pbar2");
	pbar2->setMax(61);

	Window	*win3 = new Window("A window");
	win3->center();
	win3->move(win3->x1 - 200, win3->y1 - 70);
	win3->resize(190, 400);

 	new Label(win3, 5, 25, "Object resolution:");
	stepCount = new Slider(win3, 10, 50, 180, 65);
	stepCount->setMax(61);
	stepCount->setValue(3);
	stepCount->modified = new CallbackAction(stepsModifiedAction);

 	new Label(win3, 5, 80, "Object size:");
	sizeSlider = new Slider(win3, 10, 105, 180, 120);
	sizeSlider->setMax(5);
	sizeSlider->value.setDataSource(&size);

 	new Label(win3, 5, 175, "Background intensity:");
	backSlider = new Slider(win3, 10, 200, 180, 215);
	backSlider->setMax(1.0f);
	backSlider->value.setDataSource(&backgnd);

 	Scrollbox	*sbox = new Scrollbox(win3, 10, 240, 175, 350);
 	new Button(sbox, 10, 10, 200, 35, "Clipped button");
 	new Button(sbox, 10, 40, 200, 65, "Another clipped button");
 	new Checkbox(sbox, 10, 80, 200, 100, "Clipped checkbox");
 	new Label(sbox, 10, 120, "Clipped label");


	Window	*win4 = new Window("ModelView control");
	win4->center();
	win4->move(win4->x1, win4->y1 + 160);

	ModelView	*mview = new ModelView(win4, 10, 10, 175, 110);
	mview->renderView = new CallbackAction(renderObjectAction);

	Window	*win5 = new Window("Image control");
	win5->center();
	win5->move(win5->x1, win5->y1 - 160);
	Image	*cenda = new Image(win5, 0, 0, "cenda.bmp");
	cenda->center();
	cenda->setName("cenda");
	cenda->clicked = new CallbackAction(cendaClicked);
	PopupMenu	*cendaMenu = new PopupMenu;
	cendaMenu->addItem("No filtering", new CallbackAction(disableFiltering));
	cendaMenu->addItem("Bilinear filtering",
		new CallbackAction(enableFiltering));
	cenda->popupMenu = cendaMenu;
	win5->resizeable = true;
	win5->resized = new CallbackAction(imageWinResizedAction);
	win5->icon = new Bitmap(INTERNALBMP_WINICON);
	win5->menu = new PopupMenu;
	win5->menu->addItem("No filtering",
		new CallbackAction(disableFiltering));
	win5->menu->addItem("Bilinear filtering",
		new CallbackAction(enableFiltering));

	(new Button(screen, 750, 5, 795, 30, "Quit"))->clicked =
 		new CallbackAction(terminateAppAction);*/


	/*PopupMenu       *mainMenu = new PopupMenu;
	mainMenu->addItem("Select file...",
		new CallbackAction(selectFileAction),
		new Bitmap(INTERNALBMP_OPEN));
	MenuItem	*sub = mainMenu->addItem("Submenu test", NULL);
	sub->addItem("Sub menu item 1", NULL);
	sub->addItem("Sub menu item 2", NULL);
	sub->addItem("Sub menu item 3", NULL);
	sub->addItem("-", NULL);
	sub->addItem("Sub sub menu", NULL)->addItem("Hello!", NULL);
	sub->addItem("Dynamic submenu", NULL)->popup =
	        new CallbackAction(createDynamicSubmenuAction);
	mainMenu->addItem("About...", new CallbackAction(aboutBoxAction));
	mainMenu->addItem("-", NULL);
	mainMenu->addItem("Quit", new CallbackAction(terminateAppAction),
		new Bitmap(INTERNALBMP_QUIT));*/

	screen1 = screen;
	//screen2 = new Screen();
	//Window	*nwin = new Window(screen2, "A window in screen 2");
	//nwin->center();

	//screen1->popupMenu /*= screen2->popupMenu*/ = mainMenu;


 	/*(new Button(screen, 5, 5, 105, 30, "Screen 1"))->clicked =
 		new CallbackAction(activateScreen1);
 	(new Button(screen, 110, 5, 210, 30, "Screen 2"))->clicked =
 		new CallbackAction(activateScreen2);
 	(new Button(screen2, 5, 5, 105, 30, "Screen 1"))->clicked =
 		new CallbackAction(activateScreen1);
 	(new Button(screen2, 110, 5, 210, 30, "Screen 2"))->clicked =
 		new CallbackAction(activateScreen2);*/

 	//fps = new Label(screen1, 5, 40, "?? fps");
}

void runMe()
{
	SDL_Event	event;
	//uint		last = SDL_GetTicks();
	//float		frames = 0, tenths = 0;

	while (running)
	{
		/*frames++;
		if (SDL_GetTicks() - last > 10)
		{
			char	buff[128];
			rotation += 1;
			last = SDL_GetTicks();
			tenths++;
			float seconds = tenths/100.0f;
			sprintf(buff, "%i fps for %i seconds and %i frames",
				(int)(frames/seconds), (int)seconds,
				(int)frames);
			fps->setText(buff);
		}*/
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

int main(int argc, char **argv)
{
	initOpenGL();
	initBSGUI();

	createUI();

	runMe();

	shutdownBSGUI();
	SDL_Quit();

	return 0;
}
