
/* brickfield.cpp
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "brick.h"
#include "brickfield.h"

//------------//
// BrickField //
//------------//

bool BrickField::isEmpty(const int x, const int y, const int z) const
{
	const int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return true;
	return field[idx] == nullptr;
}

// get i set vrse samo primitivne operacije

Brick* BrickField::get(const int x, const int y, const int z) const
{
	const int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return nullptr;
	return field[idx];
}

void BrickField::set(const int x, const int y, const int z, Brick* brick) const
{
	const int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return;
	field[idx] = brick;
}

// clear brise objekat i iz memorije i iz polja

void BrickField::clear(const int x, const int y, const int z) const
{
	const int idx = index(x, y, z);
	if (idx < 0 || idx >= size) return;

	delete field[idx];
	field[idx] = nullptr;
}

void BrickField::clear() const
{
	for (int i = 0; i < size; i++)
	{
		delete field[i];
		field[i] = nullptr;
	}
}

void BrickField::RotateX(const bool ccw) const {
	if (dimY != dimZ) return;
	const int d = dimY;

	for (int x = 0; x < dimX; x++)
		for (int z = 0; z < d/2; z++)
			for (int y = z; y < d-1-z; y++)
			{
				const int i0 = index(x,       y,       z);
				const int i1 = index(x,     z, d-y-1);
				const int i2 = index(x, d-y-1, d-z-1);
				const int i3 = index(x, d-z-1,     y);
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
void BrickField::RotateY(const bool ccw) const
{
	if (dimX != dimZ) return;
	const int d = dimX;

	for (int y = 0; y < dimY; y++)
		for (int x = 0; x < d/2; x++)
			for (int z = x; z < d-1- x; z++)
			{
				const int i0 = index(      x, y,       z);
				const int i1 = index(d-z-1, y,     x);
				const int i2 = index(d-x-1, y, d-z-1);
				const int i3 = index(    z, y, d-x-1);
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
void BrickField::RotateZ(const bool ccw) const
{
	if (dimX != dimY) return;
	const int d = dimX;

	for (int z = 0; z < dimZ; z++)
		for (int y = 0; y < d/2; y++)
			for (int x = y; x < d-1-y; x++)
			{
				const int i0 = index(      x,       y, z);
				const int i1 = index(    y, d-x-1, z);
				const int i2 = index(d-x-1, d-y-1, z);
				const int i3 = index(d-y-1,     x, z);
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
				if (b == nullptr) continue;

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

	const int segX = dimX;
	const int segY = dimY;
	const int segZ = dimZ;

	const float mx = dimX + 0.01f;
	const float my = dimY + 0.01f;
	const float mz = dimZ + 0.01f;

	const float dx = mx / segX;
	const float dy = my / segY;
	const float dz = mz / segZ;

	const float x0 = -mx / 2.0f;
	const float y0 = -my / 2.0f;
	const float z0 = -mz / 2.0f;

	const float x1 = +mx / 2.0f;
	const float y1 = +my / 2.0f;
	const float z1 = +mz / 2.0f;

	float x, y, z;

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(0.8f, 0.8f, 0.8f, 1.0f);

	// pod

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);

	constexpr int add = 10;
	x = x0 - add * dx;
	for (int xi = -add; xi < segX + add; xi++, x += dx)
	{
		y = y0 - add * dy;
		for (int yi = -add; yi < segY + add; yi++, y += dy)
		{
			float z = z0;
			if (xi < 0 || xi >= segX || yi < 0 || yi >= segY) z = z1;
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y   , z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+dx, y   , z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+dx, y+dy, z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y+dy, z);
		}
	}

	// levi zid

	glNormal3f(1, 0, 0);
	z = z0;
	for (int zi = 0; zi < segZ; zi++, z += dz)
	{
		y = y0;
		for (int yi = 0; yi < segY; yi++, y += dy)
		{
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y   , z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y+dy, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y+dy, z+dz);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y   , z+dz);
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
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y0, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x-dx, y0, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x-dx, y0, z+dz);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y0, z+dz);
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
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y   , z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y-dy, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y-dy, z+dz);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y   , z+dz);
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
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x   , y1, z   );
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+dx, y1, z   );
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+dx, y1, z+dz);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x   , y1, z+dz);
		}
	}

	glEnd();

	// zovi stari

	glEndList();

	listWallCompiled = true;
}

void WalledBrickField::draw() const
{
	if (listWallCompiled) glCallList(listWall);
	if (drawField) BrickField::draw();
}
