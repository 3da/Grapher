/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_ACTIONS_H_INCLUDED__
#define __BSGUI_ACTIONS_H_INCLUDED__

struct Action
{
	bool	autoDelete;
	
	Action();
	virtual ~Action();
	
	virtual void run(struct Control *sender){}
};

typedef void    (*CallbackActionFunc)(struct Control *sender);

struct CallbackAction : public Action
{
	CallbackActionFunc      func;
	
	CallbackAction(CallbackActionFunc cbFunc);
	
	virtual void run(struct Control *sender);
};

#define BSGUI_FREEACTION(a) { \
	if ((a) && (a)->autoDelete) \
		DELETE(a); }
#define BSGUI_RUNACTION(a) { \
	if (a) (a)->run(this); }
#define BSGUI_RUNACTION_OF(o,a) { \
	if (a) (a)->run(o); }

#endif
