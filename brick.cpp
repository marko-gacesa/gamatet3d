
/* brick.h
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "brick.h"

GLuint Brick::texture = 0;
GLuint Brick::listBlock = 0;

void Brick::drawBrick()
{
	constexpr float blockSize = 0.475f;

	glBegin(GL_QUADS);
		// front
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-blockSize, -blockSize,  blockSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( blockSize, -blockSize,  blockSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( blockSize,  blockSize,  blockSize);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-blockSize,  blockSize,  blockSize);
		// back
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-blockSize, -blockSize, -blockSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-blockSize,  blockSize, -blockSize);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( blockSize,  blockSize, -blockSize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( blockSize, -blockSize, -blockSize);
		// top
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-blockSize,  blockSize, -blockSize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-blockSize,  blockSize,  blockSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( blockSize,  blockSize,  blockSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( blockSize,  blockSize, -blockSize);
		// bottom
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-blockSize, -blockSize, -blockSize);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( blockSize, -blockSize, -blockSize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( blockSize, -blockSize,  blockSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-blockSize, -blockSize,  blockSize);
		// right
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( blockSize, -blockSize, -blockSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( blockSize,  blockSize, -blockSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( blockSize,  blockSize,  blockSize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( blockSize, -blockSize,  blockSize);
		// left
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-blockSize, -blockSize, -blockSize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-blockSize, -blockSize,  blockSize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-blockSize,  blockSize,  blockSize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-blockSize,  blockSize, -blockSize);
	glEnd();
}

void Brick::compileList()
{
	listBlock = glGenLists(1);
	if (listBlock == 0) return;

	glNewList(listBlock, GL_COMPILE);
	drawBrick();
	glEndList();
}

void Brick::draw() const
{
	glBindTexture(GL_TEXTURE_2D, texture);

	color.activate();

	if (listBlock == 0)
		compileList();
	else
		glCallList(listBlock);
}
