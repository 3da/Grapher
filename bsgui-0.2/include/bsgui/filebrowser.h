/*
** Slashy2D Editor
*/

#ifndef __BSGUI_FILEBROWSER_H_INCLUDED__
#define __BSGUI_FILEBROWSER_H_INCLUDED__

struct FileBrowser : public Window
{
	Listbox		*files;
	Inputbox	*filename;
	char		*path;
	
	FileBrowser(char *caption, char *path=NULL);
	virtual ~FileBrowser();
	
	virtual char *getFilename();
	virtual void reloadFiles();
};

#endif
