/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

Action::Action()
{
	autoDelete = true;
}

Action::~Action()
{
}


CallbackAction::CallbackAction(CallbackActionFunc cbFunc)
{
	func = cbFunc;
}

void CallbackAction::run(Control *sender)
{
	if (func)
		func(sender);
}
