/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

using namespace BSGUIDraw;

static Control	*lastChildUnderMouse = NULL;
static Control	*trackControl = NULL;
static Control	*keyboardFocusControl = NULL;

bool initBSGUI()
{
	screen = NEW(Screen);
	screen->activate();
 	
	return true;
}

void shutdownBSGUI()
{
	DELETE(screen);
}

void renderBSGUI()
{
	bool	scissorEnabled = glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE;
	bool	depthEnabled = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	bool	cullingEnabled = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	bool	textureEnabled = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE;
	bool	lightingEnabled = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	if (!scissorEnabled)
		glEnable(GL_SCISSOR_TEST);
	if (depthEnabled)
		glDisable(GL_DEPTH_TEST);
	if (cullingEnabled)
		glDisable(GL_CULL_FACE);
	if (!textureEnabled)
		glEnable(GL_TEXTURE_2D);
	if (lightingEnabled)
		glDisable(GL_LIGHTING);
	glLoadIdentity();
	enableOrtho();

	screen->render();
	BSGUIDraw::resetClipping();
	
	int	x, y;
	SDL_GetMouseState(&x, &y);
	screen->mouseX = x;
	screen->mouseY = y;
	BSGUIDraw::drawCursor(x - 1, y - 1);

	disableOrtho();
	if (lightingEnabled)
		glEnable(GL_LIGHTING);
	if (!textureEnabled)
		glDisable(GL_TEXTURE_2D);
	if (cullingEnabled)
		glEnable(GL_CULL_FACE);
	if (depthEnabled)
		glEnable(GL_DEPTH_TEST);
	if (!scissorEnabled)
		glDisable(GL_SCISSOR_TEST);
}

void tickBSGUI()
{
	screen->tick();
}

bool handleSDLEvent(SDL_Event *ev)
{
	Control	*child;
	
	if (!screen)
		return false;
	
	switch (ev->type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (trackControl)
			{
				trackControl->onMouseDown(ev->button.x,
    					ev->button.y, ev->button.button);
				break;
			}
			if (screen->exclusiveChild)
			{
				int	x1, y1, x2, y2;
				screen->exclusiveChild->getBounds(x1, y1, x2,
					y2);
				if (!(ev->button.x >= x1 && ev->button.x <= x2
					&& ev->button.y >= y1 &&
					ev->button.y <= y2))
					break;
				child = screen->exclusiveChild->childAt(
					ev->button.x, ev->button.y);
			}
			else
				child = screen->childAt(ev->button.x,
					ev->button.y);
   			child->onMouseDown(ev->button.x, ev->button.y,
      				ev->button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			if (trackControl)
			{
				trackControl->onMouseUp(ev->button.x,
    					ev->button.y, ev->button.button);
				break;
			}
			if (screen->exclusiveChild)
				child = screen->exclusiveChild->childAt(
					ev->button.x, ev->button.y);
			else
				child = screen->childAt(ev->button.x,
					ev->button.y);
   			child->onMouseUp(ev->button.x, ev->button.y,
      				ev->button.button);
			break;
		case SDL_MOUSEMOTION:
			if (trackControl)
			{
				trackControl->onMouseMoved(ev->button.x,
    					ev->button.y);
			}
			if (screen->exclusiveChild)
				child = screen->exclusiveChild->childAt(
					ev->motion.x, ev->motion.y);
			else
				child = screen->childAt(ev->motion.x,
					ev->motion.y);
			if (lastChildUnderMouse != child)
			{
				if (lastChildUnderMouse)
					lastChildUnderMouse->onMouseExit();
				lastChildUnderMouse = child;
				child->onMouseEnter();
			}
   			child->onMouseMoved(ev->motion.x, ev->motion.y);
			break;
		case SDL_KEYDOWN:
			if (!(keyboardFocusControl &&
				keyboardFocusControl->focused()))
				break;
			keyboardFocusControl->onKeyDown(ev->key.keysym.sym,
				ev->key.keysym.unicode&0x7F);
			break;
		case SDL_KEYUP:
			if (!(keyboardFocusControl &&
				keyboardFocusControl->focused()))
				break;
			keyboardFocusControl->onKeyUp(ev->key.keysym.sym,
				ev->key.keysym.unicode&0x7F);
			break;
	}

	return false;
}

void setMouseTrackingControl(Control *control)
{
	trackControl = control;
}

void invalidateControl(Control *control)
{
        if (lastChildUnderMouse == control)
                lastChildUnderMouse = NULL;
        if (keyboardFocusControl == control)
        	keyboardFocusControl = NULL;
}

void setKeyboardFocusControl(Control *control)
{
	keyboardFocusControl = control;
}

bool hasKeyboardFocus(Control *control)
{
	return (keyboardFocusControl == control) && (control->focused());
}
