/*
** Bad Sector's OpenGL GUI
*/

#include <dirent.h>
#include <bsgui/bsgui.h>

static void cancelFileBrowserAction(Control *sender)
{
	delete sender->parent;
}

static void closeFileBrowserAction(Control *sender)
{
	FileBrowser	*browser = (FileBrowser*)sender->parent;
	if (browser->filename->getText()[0] == '/')
	{
		char	*newPath;
		if (!strcmp(browser->filename->getText(), "/."))
		{
			browser->reloadFiles();
			return;
		}
		newPath = (char*)MALLOC(strlen(browser->filename->getText()) + 
			strlen(browser->path)+1);
		sprintf(newPath, "%s/%s", browser->path,
			browser->filename->getText() + 1);
		FREE(browser->path);
		browser->path = newPath;
		browser->reloadFiles();
		return;
	}
	BSGUI_RUNACTION_OF(browser, browser->selected);
	delete browser;
}

static void selectFileBrowserAction(Control *sender)
{
	FileBrowser	*browser = (FileBrowser*)sender->parent;
	browser->filename->setText(browser->files->active->text);
}

FileBrowser::FileBrowser(char *caption, char *path)
	: Window(caption)
{
	int	w, h;
	Label	*l;
	Button	*b;
	resize(500, 300);
	center();
	if (path)
		this->path = strdup(path);
	else
		this->path = strdup(".");
	
	getClientSize(w, h);
	
	l = NEW(Label, this, 5, 5, "Files and directories:");
	files = NEW(Listbox, this, 5, l->y2 + 5, w-10, h-40);
	files->modified = new CallbackAction(selectFileBrowserAction);
	l = NEW(Label, this, 5, files->y2+9, "Filename:");
	filename = NEW(Inputbox, this, l->x2 + 5, files->y2 + 5, w-130,
		files->y2 + 30);
	b = NEW(Button, this, w-125, files->y2 + 5, w-70, files->y2 + 30, "Ok");
	b->clicked = NEW(CallbackAction, closeFileBrowserAction);
	b = NEW(Button, this, w-65, files->y2 + 5, w-10, files->y2+30, "Cancel");
	b->clicked = NEW(CallbackAction, cancelFileBrowserAction);
	
	reloadFiles();
	
	makeExclusive();
}

FileBrowser::~FileBrowser()
{
	FREE(path);
}

char *FileBrowser::getFilename()
{
	char	*fname;
	if (filename->getText()[0] == '/')
	{
		fname = (char*)MALLOC(strlen(filename->getText()) +
			strlen(path) + 1);
		sprintf(fname, "%s/%s", path, filename->getText() + 1);
	}
	else
	{
		fname = (char*)MALLOC(strlen(filename->getText()) + 
			strlen(path) + 2);
		sprintf(fname, "%s/%s", path, filename->getText());
	}
	return fname;
}

void FileBrowser::reloadFiles()
{
	struct dirent	*dent;
	DIR		*dir;
	files->removeAllChildren();
	
	dir = opendir(path);
	while ((dent = readdir(dir)))
	{
		DIR	*dir2 = opendir(dent->d_name);
		if (dir2)
		{
			char	*buff = (char*)MALLOC(strlen(dent->d_name)+2);
			closedir(dir2);
			sprintf(buff, "/%s", dent->d_name);
			files->addItem(buff);
			FREE(buff);
		}
		else
			files->addItem(dent->d_name);
	}
	closedir(dir);
	files->layout();
	
	files->selectItem((unsigned int)0);

	if (files->active)
		filename->setText(files->active->text);
	else
		filename->setText("");
}
