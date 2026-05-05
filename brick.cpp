
/* brick.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#include <GL/glut.h>
#include <GL/gl.h>
#include "brick.h"

GLuint Brick::texture = 0;
GLuint Brick::listBlock = 0;

void Brick::drawBrick()
{
	const float blockSize = 0.475f;

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

	if (color.getAlpha() < 0.99f)
		glDisable(GL_CULL_FACE);

	color.activate();

	if (listBlock == 0)
		drawBrick();
	else
		glCallList(listBlock);

	glEnable(GL_CULL_FACE);
}
