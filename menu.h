
/* tet3d.h
 *
 * Autor: Marko Gacesa
 * Datum: 13.06.2007.
 */

#ifndef _MENU_H_
#define _MENU_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "color.h"
#include "timer.h"
#include "tet3d.h"

class MenuChar
{
private:
	MenuChar();
public:
	static const float charDim;
	static GLuint textures[37];
	static void loadTextures();
	static void draw(char ch);
	static void draw2D(char ch, float x, float y, float d);
	static void drawString2D(const char* s, float x, float y, float d);
	static void drawString2Dc(const char* s, float y, float d);
};

class MenuItem
{
private:
	MenuItem *prev, *next;
	float phi, ang;
	Timer timer;
protected:
	char *text;
	int code;
	friend class Menu;
public:
	MenuItem(int code, const char *str) : timer() {
		this->code = code;
		prev = next = NULL;
		if (str != NULL) text = strdup(str);
		phi = ang = 0.0f;
		timer.start();
	}
	virtual ~MenuItem() { delete[] text; }

	virtual void update() {}

	virtual bool forceSelect() { return false; }
	virtual void action() {}

	virtual void draw();
};

class MenuItemSet : public MenuItem
{
private:
	int *data;
	int value;
public:
	MenuItemSet(int code, const char *str, int *pdata, int val) : MenuItem(code, str), data(pdata), value(val) {}
	virtual bool forceSelect() { return *data == value; }
	virtual void action() { *data = value; }
};

class MenuItemSetBlock : public MenuItem
{
private:
	Tet3D::BlockSet *data;
	Tet3D::BlockSet value;
public:
	MenuItemSetBlock(int code, const char *str, Tet3D::BlockSet *pdata, Tet3D::BlockSet val) : MenuItem(code, str), data(pdata), value(val) {}
	virtual bool forceSelect() { return *data == value; }
	virtual void action() { *data = value; }
};

class MenuItemAdv : public MenuItem
{
private:
	void (*act)();
	bool (*fSel)();
public:
	MenuItemAdv(int code, const char *str, void (*pAction)(), bool (*pForceSel)()) : MenuItem(code, str), act(pAction), fSel(pForceSel) {}
	virtual bool forceSelect() { if (fSel != NULL) return fSel(); else return false; }
	virtual void action() { if (act != NULL) act(); }
};

class MenuItemX : public MenuItem
{
private:
	int *x;
	char *title;
public:
	MenuItemX(int code, const char *s, int *xx) : MenuItem(code, NULL), x(xx), title(strdup(s))
	{
		text = new char[100];
	}
	virtual ~MenuItemX() { delete[] title; }
	virtual void update() { snprintf(text, 100, "%s %d", title, *x); }
};

class MenuItemXBlock : public MenuItem
{
private:
	Tet3D::BlockSet *b;
	char *title;
public:
	MenuItemXBlock(int code, const char *s, Tet3D::BlockSet *bb) : MenuItem(code, NULL), b(bb), title(strdup(s))
	{
		text = new char[100];
	}
	virtual ~MenuItemXBlock() { delete[] title; }
	virtual void update() { snprintf(text, 100, "%s %c", title, Tet3D::getBlockSetCode(*b)); }
};

class MenuItemWHDB : public MenuItem
{
private:
	int *w, *h, *d;
	Tet3D::BlockSet *b;
	char *title;
public:
	MenuItemWHDB(int code, const char *s, int *width, int *height, int *depth, Tet3D::BlockSet *bset) : MenuItem(code, NULL), w(width), h(height), d(depth), b(bset), title(strdup(s))
	{
		text = new char[100];
	}
	virtual ~MenuItemWHDB() { delete[] title; }
	virtual void update() { snprintf(text, 100, "%s %dx%dx%d%c", title, *w, *h, *d, Tet3D::getBlockSetCode(*b)); }
};

class MenuItemCustom : public MenuItemWHDB
{
public:
	MenuItemCustom(int code, const char *s, int *width, int *height, int *depth, Tet3D::BlockSet *bset) : MenuItemWHDB(code, s, width, height, depth, bset) {}
	virtual bool forceSelect() { return true; }
};

class Menu
{
private:
	MenuItem *first, *last, *curr;
	int count;
	int code;
public:
	Menu(int cancelCode) : first(NULL), last(NULL), curr(NULL), count(0), code(cancelCode) {}
	~Menu();

	static GLuint texture;
	static void loadTexture();

	void add(MenuItem *item);

	void start();
	int activate();
	int cancel() { return code; }

	void up();
	void down();

	void draw();

	static void drawHelp();
};


#endif
