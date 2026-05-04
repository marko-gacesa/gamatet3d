
/* brickfield.cpp
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#include <stdlib.h>
#include "brick.h"
#include "brickfield.h"
#include "anim.h"

//------------//
// BrickField //
//------------//

bool BrickField::isEmpty(int x, int y, int z) const
{
	int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return true;
	return field[idx] == NULL;
}

// get i set vrse samo primitivne operacije

Brick* BrickField::get(int x, int y, int z) const
{
	int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return NULL;
	return field[idx];
}

void BrickField::set(int x, int y, int z, Brick* brick)
{
	int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return;
	field[idx] = brick;
}

// clear brise objekat i iz memorije i iz polja

void BrickField::clear(int x, int y, int z)
{
	int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return;

	delete field[idx];
	field[idx] = NULL;
}

void BrickField::clear()
{
	for (int i = 0; i < size; i++)
	{
		delete field[i];
		field[i] = NULL;
	}
}

void BrickField::RotateX(bool ccw)
{
	if (dimY != dimZ) return;
	int d = dimY;

	for (int x = 0; x < dimX; x++)
		for (int z = 0; z < d/2; z++)
			for (int y = z; y < d-1-z; y++)
			{
				int i0 = index(x,     y,     z);
				int i1 = index(x,     z, d-y-1);
				int i2 = index(x, d-y-1, d-z-1);
				int i3 = index(x, d-z-1,     y);
				Brick* t = field[i0];
				if (ccw)
				{
					field[i0] = field[i1];
					field[i1] = field[i2];
					field[i2] = field[i3];
					field[i3] = t;
				}
				else // cw
				{
					field[i0] = field[i3];
					field[i3] = field[i2];
					field[i2] = field[i1];
					field[i1] = t;
				}
			}
}
void BrickField::RotateY(bool ccw)
{
	if (dimX != dimZ) return;
	int d = dimX;

	for (int y = 0; y < dimY; y++)
		for (int x = 0; x < d/2; x++)
			for (int z = x; z < d-1- x; z++)
			{
				int i0 = index(    x, y,     z);
				int i1 = index(d-z-1, y,     x);
				int i2 = index(d-x-1, y, d-z-1);
				int i3 = index(    z, y, d-x-1);
				Brick* t = field[i0];
				if (ccw)
				{
					field[i0] = field[i1];
					field[i1] = field[i2];
					field[i2] = field[i3];
					field[i3] = t;
				}
				else // cw
				{
					field[i0] = field[i3];
					field[i3] = field[i2];
					field[i2] = field[i1];
					field[i1] = t;
				}
			}
}
void BrickField::RotateZ(bool ccw)
{
	if (dimX != dimY) return;
	int d = dimX;

	for (int z = 0; z < dimZ; z++)
		for (int y = 0; y < d/2; y++)
			for (int x = y; x < d-1-y; x++)
			{
				int i0 = index(    x,     y, z);
				int i1 = index(    y, d-x-1, z);
				int i2 = index(d-x-1, d-y-1, z);
				int i3 = index(d-y-1,     x, z);
				Brick* t = field[i0];
				if (ccw)
				{
					field[i0] = field[i1];
					field[i1] = field[i2];
					field[i2] = field[i3];
					field[i3] = t;
				}
				else // cw
				{
					field[i0] = field[i3];
					field[i3] = field[i2];
					field[i2] = field[i1];
					field[i1] = t;
				}
			}
}

void BrickField::draw() const
{
	for (int z = 0; z < dimZ; z++)
		for (int y = 0; y < dimY; y++)
			for (int x = 0; x < dimX; x++)
			{
				Brick* b = field[index(x, y, z)];
				if (b == NULL) continue;

				glPushMatrix();

				// dovedi centar koord sistema tamo gde je centar cigle
				glTranslatef(
					-dimX / 2.0f + 0.5f + x,
					-dimY / 2.0f + 0.5f + y,
					-dimZ / 2.0f + 0.5f + z);

				b->render();

				glPopMatrix();
			}
}

//------------------//
// WalledBrickField //
//------------------//

GLuint WalledBrickField::texture = 0;

void WalledBrickField::compileListWall()
{
	listWall = glGenLists(1);
	if (listWall == 0) return;

	glNewList(listWall, GL_COMPILE);

	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	int segX = dimX;
	int segY = dimY;
	int segZ = dimZ;

	float mx = dimX + 0.01;
	float my = dimY + 0.01;
	float mz = dimZ + 0.01;

	float dx = mx / segX;
	float dy = my / segY;
	float dz = mz / segZ;

	float x0 = -mx / 2.0f;
	float y0 = -my / 2.0f;
	float z0 = -mz / 2.0f;

	float x1 = +mx / 2.0f;
	float y1 = +my / 2.0f;
	float z1 = +mz / 2.0f;

	float x, y, z;

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(0.8f, 0.8f, 0.8f, 1.0f);

	// pod

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);

	int add = 10;
	x = x0 - add * dx;
	for (int xi = -add; xi < segX + add; xi++, x += dx)
	{
		y = y0 - add * dy;
		for (int yi = -add; yi < segY + add; yi++, y += dy)
		{
			float z = z0;
			if (xi < 0 || xi >= segX || yi < 0 || yi >= segY) z = z1;
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y   , z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+dx, y   , z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+dx, y+dy, z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y+dy, z);
		}
	}
	glEnd();

	// zid

	glBegin(GL_QUADS);

	// levi zid

	glNormal3f(1, 0, 0);
	z = z0;
	for (int zi = 0; zi < segZ; zi++, z += dz)
	{
		y = y0;
		for (int yi = 0; yi < segY; yi++, y += dy)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y   , z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y+dy, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y+dy, z+dz);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y   , z+dz);
		}
	}

	// donji zid

	glNormal3f(0, 1, 0);
	z = z0;
	for (int zi = 0; zi < segZ; zi++, z += dz)
	{
		x = x1;
		for (int xi = 0; xi < segX; xi++, x -= dx)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y0, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x-dx, y0, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x-dx, y0, z+dz);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y0, z+dz);
		}
	}

	// desni zid

	glNormal3f(-1, 0, 0);
	z = z0;
	for (int zi = 0; zi < segZ; zi++, z += dz)
	{
		y = y1;
		for (int yi = 0; yi < segY; yi++, y -= dy)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y   , z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y-dy, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y-dy, z+dz);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y   , z+dz);
		}
	}

	// gornji zid

	glNormal3f(0, -1, 0);
	z = z0;
	for (int zi = 0; zi < segZ; zi++, z += dz)
	{
		x = x0;
		for (int xi = 0; xi < segX; xi++, x += dx)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y1, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+dx, y1, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+dx, y1, z+dz);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y1, z+dz);
		}
	}

	glEnd();

	// zovi stari

	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glEndList();

	listWallCompiled = true;
}

void WalledBrickField::draw() const
{
	if (listWallCompiled) glCallList(listWall);
	if (drawField) BrickField::draw();
}

//\\//\\
