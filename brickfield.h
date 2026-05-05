
/* brickfield.h
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _BRICKFIELD_H_
#define _BRICKFIELD_H_

#include "brick.h"

//------------//
// BrickField //
//------------//

class BrickField : public AnimObj
{
public:
	const int dimX;
	const int dimY;
	const int dimZ;

private:
	const int size;
	Brick ** const field;

	int index(const int x, const int y, const int z) const { return dimX * dimY * z + dimX * y + x; }

public:
	BrickField(const int dimensionX, const int dimensionY, const int dimensionZ) :
			dimX(dimensionX), dimY(dimensionY), dimZ(dimensionZ), size(dimX * dimY * dimZ), field(new Brick*[size])
	{
		for (int i = 0; i < size; i++) field[i] = nullptr;
	}

	~BrickField() override
	{
		clear();
		delete [] field;
	}

	// vraca true ako su koordinate (x,y,z) validne
	bool isInside(const int x, const int y, const int z) const
	{
		return x >= 0 && x < dimX && y >= 0 && y < dimY && z >= 0 && z < dimZ;
	}

	// vraca true ako na (x,y,z) nema nista
	bool isEmpty(int x, int y, int z) const;

	Brick* get(int x, int y, int z) const;
	void set(int x, int y, int z, Brick* brick) const;

	void clear(int x, int y, int z) const; // brise Brick na (x, y, z) i upisuje tu NULL
	void clear() const;

	int minX() const { return 0; }
	int minY() const { return 0; }
	int minZ() const { return 0; }
	int maxX() const { return dimX - 1; }
	int maxY() const { return dimY - 1; }
	int maxZ() const { return dimZ - 1; }

	void RotateX(bool ccw) const;
	void RotateY(bool ccw) const;
	void RotateZ(bool ccw) const;

	void draw() const override;
};

//------------------//
// WalledBrickField //
//------------------//

class WalledBrickField : public BrickField
{
private:
	GLuint listWall{};
	bool listWallCompiled;
	void compileListWall();
public:
	WalledBrickField(const int dimensionX, const int dimensionY, const int dimensionZ)
		: BrickField(dimensionX, dimensionY, dimensionZ)
	{
		drawField = true;
		listWallCompiled = false;
		compileListWall();
	}
	~WalledBrickField() override
	{
		if (listWallCompiled) glDeleteLists(listWall, 1);
	}

	static GLuint texture;

	bool drawField;

	void draw() const override;
};


#endif
