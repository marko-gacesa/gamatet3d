
/* menu.cpp
 *
 * Autor: Marko Gaćeša
 * Datum: 13.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include "menu.h"

#include "stb/stb_image.h"

float deg2rad = 0.01745329252f;
float rad2deg = 57.2957795131f;

//----------//
// MenuChar //
//----------//

const float MenuChar::charDim = 0.5f;
GLuint MenuChar::textures[37];

void MenuChar::loadTextures()
{
	const auto fname = "texture/char/_.png";
	const auto name = strdup(fname);

	glGenTextures(37, textures);

	for (int i = 0; i < 37; i++)
	{
		if (i >=  1 && i <= 10) name[13] = '0' + i - 1;
		if (i >= 11 && i <= 36) name[13] = 'a' + i - 11;

		int width, height, channels;
		unsigned char* data = stbi_load(name, &width, &height, &channels, 0);

		if (data == nullptr) continue;

		glBindTexture(GL_TEXTURE_2D, textures[i]);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_INTENSITY8, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		stbi_image_free(data);
	}

	free(name);
}

void MenuChar::draw(const char ch)
{
	int tex = 0;

	if (ch >= '0' && ch <= '9')
		tex = ch - '0' + 1;
	else if (ch >= 'a' && ch <= 'z')
		tex = ch - 'a' + 11;
	else if (ch >= 'A' && ch <= 'Z')
		tex = ch - 'A' + 11;

	glBindTexture(GL_TEXTURE_2D, textures[tex]);

	glBegin(GL_QUADS);
		// front
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-charDim, -charDim,  charDim);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( charDim, -charDim,  charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( charDim,  charDim,  charDim);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-charDim,  charDim,  charDim);
		// back
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-charDim, -charDim, -charDim);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-charDim,  charDim, -charDim);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( charDim,  charDim, -charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( charDim, -charDim, -charDim);
		// top
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-charDim,  charDim, -charDim);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-charDim,  charDim,  charDim);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( charDim,  charDim,  charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( charDim,  charDim, -charDim);
		// bottom
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-charDim, -charDim, -charDim);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( charDim, -charDim, -charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( charDim, -charDim,  charDim);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-charDim, -charDim,  charDim);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glBegin(GL_QUADS);
		// right
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( charDim, -charDim, -charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( charDim,  charDim, -charDim);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( charDim,  charDim,  charDim);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( charDim, -charDim,  charDim);
		// left
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-charDim, -charDim, -charDim);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-charDim, -charDim,  charDim);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-charDim,  charDim,  charDim);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-charDim,  charDim, -charDim);
	glEnd();
}

void MenuChar::draw2D(const char ch, const float x, const float y, const float d)
{
	int tex = 0;

	if (ch >= '0' && ch <= '9')
		tex = ch - '0' + 1;
	else if (ch >= 'a' && ch <= 'z')
		tex = ch - 'a' + 11;
	else if (ch >= 'A' && ch <= 'Z')
		tex = ch - 'A' + 11;

	glBindTexture(GL_TEXTURE_2D, textures[tex]);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x    , y + d);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x    , y    );
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + d, y    );
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + d, y + d);
	glEnd();
}

void MenuChar::drawString2D(const char* s, const float x, const float y, const float d)
{
	for (auto xx = x; *s != '\0'; s++, xx += d)
		draw2D(*s, xx, y, d);
}

void MenuChar::drawString2Dc(const char* s, const float y, const float d)
{
	const int l = static_cast<int>(strlen(s));
	float x = -l / 2.0f * d;
	for (; *s != '\0'; s++, x += d)
		draw2D(*s, x, y, d);
}

//----------//
// MenuItem //
//----------//

void MenuItem::draw()
{
	if (text == nullptr) return;

	if (timer.getTime() > 0.1)
	{
		timer.reset();
		ang += 5;
		phi += 1;
	}

	const int length = static_cast<int>(strlen(text));

	const float dx = 2.2f * MenuChar::charDim;
	float x = (1 - length) * dx / 2.0f;

	glPushMatrix();

	glTranslatef(x, 0, 0);

	glRotatef(ang, 1, 0, 0);

	glDisable(GL_BLEND);

	for (int i = 0; i < length; i++, x += dx)
	{
		glPushMatrix();
		glRotatef((i-length/2.0)*10*sin(phi*deg2rad), 1, 0, 0);
		MenuChar::draw(text[i]);
		glPopMatrix();
		glTranslatef(dx, 0, 0);
	}

	glEnable(GL_BLEND);

	glPopMatrix();
}

//------//
// Menu //
//------//

GLuint Menu::texture = 0;

Menu::~Menu()
{
	while (first != nullptr)
	{
		curr = first;
		first = first->next;
		delete curr;
	}
}

void Menu::loadTexture()
{
	glGenTextures(1, &texture);

	int width, height, channels;
	unsigned char* data = stbi_load("texture/back.jpg", &width, &height, &channels, 0);
	if (data == nullptr) return;

	glBindTexture(GL_TEXTURE_2D, texture);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data);
}

void Menu::add(MenuItem *item)
{
	if (item == nullptr) return;

	count++;

	if (first == nullptr)
		first = last = curr = item;
	else
	{
		last->next = item;
		item->prev = last;
		last = item;
	}
}

void Menu::start()
{
	if (first == nullptr) return;

	MenuItem* k;

	for (k = first; k != nullptr; k = k->next)
		k->update();

	for (k = first; k != nullptr; k = k->next)
		if (k->forceSelect())
		{
			curr = k;
			break;
		}
}

int Menu::activate() const
{
	curr->action();
	return curr->code;
}

void Menu::up()
{
	if (curr->prev != nullptr) curr = curr->prev;
}

void Menu::down()
{
	if (curr->next != nullptr) curr = curr->next;
}

void Menu::draw() const
{
	if (first == nullptr) return;

	// back

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0);
	glEnd();

	drawTitle();

	//

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//

	const float dy = -3.2f * MenuChar::charDim;
	const float y = (1 - count) * dy / 2.0f * 0.667f;

	glPushMatrix();

	glTranslatef(0, y, 0);

	for (MenuItem *i = first; i != nullptr; i = i->next)
	{
		if (i == curr)
			glColor3f(0.9f, 0.6f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);

		i->draw();
		glTranslatef(0, dy, 0);
	}

	glPopMatrix();
}

void Menu::drawHelp()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0);
	glEnd();

	drawTitle();

	//

	glColor4f(0.7f, 0.7f, 0.7f, 0.4f);

	float y = 0.3f;
	constexpr float dy = -0.05f;

	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("       keyboard commands       ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("          move block:          ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("      keyboard arrow keys      ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("         rotate block:         ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" q  ccw rotation around x axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" w  ccw rotation around y axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" e  ccw rotation around z axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" a   cw rotation around x axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" s   cw rotation around y axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc(" d   cw rotation around z axis ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("            pause p            ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("            shadow b           ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("         full screen f1        ", y, 0.05f); y += dy;
	MenuChar::drawString2Dc("                               ", y, 0.05f); y += dy;

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Menu::drawTitle()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_SRC_COLOR);
	MenuChar::drawString2Dc("gamatet3d", 0.7f, 0.16f);
	MenuChar::drawString2Dc("by marko gacesa 2007", 0.6f, 0.07f);
	MenuChar::drawString2Dc("a clone of blockout from 1989", 0.55f, 0.03f);
	MenuChar::drawString2Dc("refurbished and open sourced in 2026", -0.97f, 0.03f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
