/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_DATAMAN_H_INCLUDED__
#define __BSGUI_DATAMAN_H_INCLUDED__

template <class Data> class DataManager
{
	Data	data;
	Data	*dataSource;
	struct Control	*control;
	
	public:

	struct Action	*modified;
	struct Action	*requested;

	DataManager()
	{
		control = NULL;
		dataSource = &data;
		modified = requested = NULL;
	}
	~DataManager()
	{
		BSGUI_FREEACTION(modified);
		BSGUI_FREEACTION(requested);
	}
	
	void setControl(Control *control)
	{
		this->control = control;
	}
	
	void setDataSource(Data *place)
	{
		if (place)
			dataSource = (Data*)place;
		else
			dataSource = &data;
	}
	
	void set(Data newData)
	{
		*dataSource = newData;
		BSGUI_RUNACTION_OF(control, modified);
	}
	
	Data get()
	{
		BSGUI_RUNACTION_OF(control, requested);
		return *dataSource;
	}
};

#endif
