
/* tet3d.h
 *
 * Autor: Marko Gaćeša
 * Datum: 13.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _MENU_H_
#define _MENU_H_


#include <cstdio>
#include <cstring>
#include <GL/gl.h>
#include "timer.h"
#include "tet3d.h"

class MenuChar
{
public:
	MenuChar() = delete;
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
	MenuItem(const int code, const char *str) : timer() {
		this->code = code;
		prev = next = nullptr;
		if (str != nullptr) text = strdup(str);
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
	MenuItemSet(const int code, const char *str, int *pdata, const int val)
		: MenuItem(code, str), data(pdata), value(val) {}
	bool forceSelect() override { return *data == value; }
	void action() override { *data = value; }
};

class MenuItemSetBlock : public MenuItem
{
private:
	Tet3D::BlockSet *data;
	Tet3D::BlockSet value;
public:
	MenuItemSetBlock(const int code, const char *str, Tet3D::BlockSet *pdata, const Tet3D::BlockSet val)
		: MenuItem(code, str), data(pdata), value(val) {}
	bool forceSelect() override { return *data == value; }
	void action() override { *data = value; }
};

class MenuItemAdv : public MenuItem
{
private:
	void (*act)();
	bool (*fSel)();
public:
	MenuItemAdv(const int code, const char *str, void (*pAction)(), bool (*pForceSel)())
		: MenuItem(code, str), act(pAction), fSel(pForceSel) {}
	bool forceSelect() override { if (fSel != nullptr) return fSel(); return false; }
	void action() override { if (act != nullptr) act(); }
};

class MenuItemX : public MenuItem
{
private:
	int *x;
	char *title;
public:
	MenuItemX(const int code, const char *s, int *xx)
		: MenuItem(code, nullptr), x(xx), title(strdup(s)) { text = new char[100]; }
	~MenuItemX() override { delete[] title; }
	void update() override { snprintf(text, 100, "%s %d", title, *x); }
};

class MenuItemXBlock : public MenuItem
{
private:
	Tet3D::BlockSet *b;
	char *title;
public:
	MenuItemXBlock(const int code, const char *s, Tet3D::BlockSet *bb)
		: MenuItem(code, nullptr), b(bb), title(strdup(s)) { text = new char[100]; }
	~MenuItemXBlock() override { delete[] title; }
	void update() override { snprintf(text, 100, "%s %c", title, Tet3D::getBlockSetCode(*b)); }
};

class MenuItemWHDB : public MenuItem
{
private:
	int *w, *h, *d;
	Tet3D::BlockSet *b;
	char *title;
public:
	MenuItemWHDB(const int code, const char *s, int *width, int *height, int *depth, Tet3D::BlockSet *bset)
		: MenuItem(code, nullptr), w(width), h(height), d(depth), b(bset), title(strdup(s))
	{
		text = new char[100];
	}
	~MenuItemWHDB() override { delete[] title; }
	void update() override { snprintf(text, 100, "%s %dx%dx%d%c", title, *w, *h, *d, Tet3D::getBlockSetCode(*b)); }
};

class MenuItemCustom : public MenuItemWHDB
{
public:
	MenuItemCustom(const int code, const char *s, int *width, int *height, int *depth, Tet3D::BlockSet *bset)
		: MenuItemWHDB(code, s, width, height, depth, bset) {}
	bool forceSelect() override { return true; }
};

class Menu
{
private:
	MenuItem *first, *last, *curr;
	int count;
	int code;
public:
	Menu(const int cancelCode) : first(nullptr), last(nullptr), curr(nullptr), count(0), code(cancelCode) {}
	~Menu();

	static GLuint texture;
	static void loadTexture();

	void add(MenuItem *item);

	void start();
	int activate() const;
	int cancel() const { return code; }

	void up();
	void down();

	void draw() const;

	static void drawHelp();
};


#endif
