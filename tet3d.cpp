
/* tet3d.cpp
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "brick.h"
#include "brickfield.h"
#include "tet3d.h"
#include "menu.h"

//-------//
// Tet3D //
//-------//

//                                       0     1     2     3     4     5     6     7     8     9     X
float Tet3D::levelPause[LEVELCOUNT] = { 4.0f, 3.4f, 2.8f, 2.3f, 1.8f, 1.4f, 1.0f, 0.7f, 0.4f, 0.2f, 0.1f };

const float Tet3D::NEXTBLOCKY = 0.20f;

// konstruktor

Tet3D::Tet3D() : field(NULL), block(NULL), timer(), random(), timerNext(), nextAnimList()
{
	drawShadows = true;

	playing = false;
	paused = false;

	animDuration = 0.25f; // [sekundi]
	slideTime = 0.55f; // [sekundi]
	blockAlpha = 0.5f; // 0..1

	for (int i = 0; i < NEXTBLOCKS; i++) next[i] = NULL;
	nextA = nextB = 0.0f;
	timerNext.start();

	colorHUD.set(0.55f, 0.5f, 0.4f, 0.7f);

	score_count = 0;
	block_count = 0;
	cube_count  = 0;
	lines_count = 0;
	level       = 0;

	blockSet = FLAT;
}

// destruktor

Tet3D::~Tet3D()
{
	delete field;
	delete block;
	for (int i = 0; i < NEXTBLOCKS; i++) delete next[i];
}

// metode

const int brickColorsNum = 7;
float brickColorsR[brickColorsNum] = { 0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 0.5f };
float brickColorsG[brickColorsNum] = { 0.1f, 0.5f, 0.5f, 0.1f, 0.1f, 0.5f, 0.5f };
float brickColorsB[brickColorsNum] = { 0.5f, 0.1f, 0.5f, 0.1f, 0.5f, 0.1f, 0.5f };

void Tet3D::paintBricks()
{
	for (int z = 0; z < field->dimZ; z++)
		for (int x = 0; x < field->dimX; x++)
			for (int y = 0; y < field->dimY; y++)
			{
				Brick* b = field->get(x, y ,z);
				if (b == NULL) continue;

				int color = z % brickColorsNum;
				b->color.set(brickColorsR[color], brickColorsG[color], brickColorsB[color], 1.0f);
			}
}

void Tet3D::generateField(int dimX, int dimY, int dimZ, BlockSet bset)
{
	if (field != NULL) delete field;
	if (block != NULL) delete block;

	field = NULL;
	block = NULL;

	field = new WalledBrickField(dimX, dimY, dimZ);
	playing = false;
	paused = false;

	blockSet = bset;

	for (int i = 0; i < NEXTBLOCKS; i++)
	{
		if (next[i] != NULL)
			delete next[i];

		next[i] = createBlock();
	}

	timer.stop();
	timer.reset();

	/*
	for (int z = 0; z < 20; z++)
		for (int x = 0; x < field->dimX; x++)
			for (int y = 0; y < field->dimY; y++)
				field->set(x, y, z, new Brick());
	paintBricks();
	//*/
}

void Tet3D::deleteField()
{
	if (field != NULL) delete field;
	if (block != NULL) delete block;

	field = NULL;
	block = NULL;

	for (int i = 0; i < NEXTBLOCKS; i++)
		if (next[i] != NULL)
		{
			delete next[i];
			next[i] = NULL;
		}

	playing = false;
	paused = false;

	timer.stop();
	timer.reset();
}

char Tet3D::getBlockSetCode(BlockSet bset)
{
	switch (bset)
	{
	case FLAT: return 'f';
	case BASIC: return 'b';
	case EXTENDED: return 'e';
	default: return 'x';
	}
}

void Tet3D::nextBlock()
{
	if (block != NULL) delete block;
	block = NULL;

	block = next[0];

	for (int i = 1; i < NEXTBLOCKS; i++) next[i - 1] = next[i];
	next[NEXTBLOCKS - 1] = createBlock();
	nextAnimList.add(new AnimQuad(0, NEXTBLOCKY, 0, animDuration));

	for (int z = 0; z < block->dimZ; z++)
		for (int y = 0; y < block->dimY; y++)
			for (int x = 0; x < block->dimX; x++)
			{
				Brick* q = block->get(x, y, z);
				if (q != NULL) q->color.set(0.9f, 0.9f, 0.6f, blockAlpha);
			}

	//blockX = field->dimX / 2 - block->dimX / 2;
	//blockY = field->dimY / 2 - block->dimY / 2;

	blockX = 0;
	blockY = 0;
	blockZ = field->dimZ - block->dimZ;

	blockX = -2;
	while (!isBlockOk() && blockX < 0) blockX++;
	blockY = -2;
	while (!isBlockOk() && blockY < 0) blockY++;

	if (!isBlockOk())
	{
		meld();

		delete block;
		block = NULL;

		gameOver();
	}
	else
		block_count++;
}

bool Tet3D::isBlockOk() const
{
	if (block == NULL) return true;

	for (int z = 0; z < block->dimZ; z++)
		for (int y = 0; y < block->dimY; y++)
			for (int x = 0; x < block->dimX; x++)
				if (
					!block->isEmpty(x, y, z) &&
					(!field->isInside(blockX+x, blockY+y, blockZ+z) ||
					!field->isEmpty(blockX+x, blockY+y, blockZ+z))
					)
					return false;
	return true;
}

/* igra */

bool Tet3D::zFull(int z) const
{
	if (field == NULL) return false;

	for (int x = 0; x < field->dimX; x++)
		for (int y = 0; y < field->dimY; y++)
			if (field->get(x, y, z) == NULL)
				return false;
	return true;
}

int Tet3D::meld()
{
	int melded = 0;

	if (block == NULL) return 0;

	for (int z = 0; z < block->dimZ; z++)
		for (int y = 0; y < block->dimY; y++)
			for (int x = 0; x < block->dimX; x++)
				if (!block->isEmpty(x, y, z))
				{
					if (field->isInside(blockX+x, blockY+y, blockZ+z))
					{
						if (field->isEmpty(blockX+x, blockY+y, blockZ+z))
						{
							// regularno stanje
							// premesti brick iz block-a u field
							field->set(blockX+x, blockY+y, blockZ+z, block->get(x, y, z));
							block->set(x, y, z, NULL);
							melded++;
						}
						else
							block->clear(x, y, z); // ne moze da se melduje, ima vec nesto tu u polju (sme da se desi samo na kraju)
					}
					else
						block->clear(x, y, z); // nema sa cim da se melduje, van polja je (nikad ne bi smelo da se desi)
				}
	return melded;
}

void Tet3D::drop1()
{
	blockZ--;
	if (!isBlockOk())
	{
		blockZ++;

		int melded = meld();
		int cleared = 0;

		for (int z = 0; z < field->dimZ;)
		{
			if (zFull(z))
			{
				cleared++;

				int x, y, k;

				for (x = 0; x < field->dimX; x++)
					for (y = 0; y < field->dimY; y++)
						field->clear(x, y, z);

				for (k = z + 1; k < field->dimZ; k++)
					for (x = 0; x < field->dimX; x++)
						for (y = 0; y < field->dimY; y++)
							if (!field->isEmpty(x, y, k))
							{
								field->get(x, y, k)->anims.add(new AnimQuad(0.0f, 0.0f, -1.0f, animDuration));
								field->set(x, y, k - 1, field->get(x, y, k));
								field->set(x, y, k, NULL);
							}
			}
			else
			{
				z++;
			}
		} // for

		if (cleared > 1)
			field->anims.add(new AnimShake(0.05f * (cleared-1), 1.0f)); // protresi ekran kad spusti za 2 ili vise

		score_count += (melded + (field->dimX * field->dimY) * cleared * cleared) * (level + 1);
		cube_count  += melded;
		lines_count += cleared;

		if (level <= 9 && cube_count > (field->dimX + field->dimY) * (level + 1) * 15)
			level++;

		paintBricks();

		nextBlock();
	}
	else
	{
		block->anims.add(new AnimQuad(0.0f, 0.0f, -1.0f, animDuration));
	}

	timer.reset();
}

/* pocinjanje/zavrsavanje igre */

void Tet3D::gameStart()
{
	timer.reset();
	timer.start();
	playing = true;
	paused = false;

	nextBlock();
}

void Tet3D::gameOver()
{
	timer.stop();
	playing = false;
	paused = false;
}

void Tet3D::pause()
{
	if (!playing) return;

	if (paused)
	{
		timer.start();
	}
	else
	{
		timer.stop();
	}

	paused = !paused;
}

/* pomeranje block-a */

void Tet3D::drop()
{
	if (!playing || paused || block == NULL) return;

	int oldBlockZ = blockZ;

	while (isBlockOk()) blockZ--;
	blockZ++;

	if (blockZ >= oldBlockZ)
	{
		blockZ = oldBlockZ;
	}
	else
	{
		block->anims.add(new AnimQuad(0.0f, 0.0f, blockZ - oldBlockZ, animDuration));

		score_count += ((oldBlockZ - blockZ) * (level + 1)) >> 1;

		// block slide

		timer.reset();
		timer.addTime(levelPause[level % LEVELCOUNT] - slideTime);
		//timer.addTime(levelPause[level % LEVELCOUNT] * (1.0f - slideTime));
	}
}

void Tet3D::moveX(int dx)
{
	if (!playing || paused || block == NULL) return;
	blockX += dx;
	if (!isBlockOk()) { blockX -= dx; return; }
	block->anims.add(new AnimLin(dx, 0.0f, 0.0f, animDuration));
}

void Tet3D::moveY(int dy)
{
	if (!playing || paused || block == NULL) return;
	blockY += dy;
	if (!isBlockOk()) { blockY -= dy; return; }
	block->anims.add(new AnimLin(0.0f, dy, 0.0f, animDuration));
}

void Tet3D::moveZ(int dz)
{
	if (!playing || paused || block == NULL) return;
	blockZ += dz;
	if (!isBlockOk()) { blockZ -= dz; return; }
	block->anims.add(new AnimLin(0.0f, 0.0f, dz, animDuration));
}

bool Tet3D::rotAdjust()
{
	const int d[3] = {0, -1, 1};
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
				if (d[x] == 0 || d[y] == 0)
				{
					int oldX = blockX;
					int oldY = blockY;
					blockX += d[x];
					blockY += d[y];

					if (isBlockOk())
					{
						if (d[x] != 0 || d[y] != 0) block->anims.add(new AnimLin(d[x], d[y], 0, animDuration));
						return true;
					}

					blockX = oldX;
					blockY = oldY;
				}
	return false;
}

void Tet3D::rotateX(bool ccw)
{
	if (!playing || paused || block == NULL) return;
	block->RotateX(ccw);

	if (rotAdjust())
		block->anims.add(new AnimRotX(ccw, animDuration));
	else
		block->RotateX(!ccw);
}

void Tet3D::rotateY(bool ccw)
{
	if (!playing || paused || block == NULL) return;
	block->RotateY(ccw);

	if (rotAdjust())
		block->anims.add(new AnimRotY(ccw, animDuration));
	else
		block->RotateY(!ccw);
}

void Tet3D::rotateZ(bool ccw)
{
	if (!playing || paused || block == NULL) return;
	block->RotateZ(ccw);

	if (rotAdjust())
		block->anims.add(new AnimRotZ(ccw, animDuration));
	else
		block->RotateZ(!ccw);
}

// height

int Tet3D::getHeight(int x, int y) const
{
	for (int z = field->dimZ - 1; z >= 0; z--)
		if (!field->isEmpty(x, y, z))
			return z + 1;
	return 0;
}

int Tet3D::getHeight() const
{
	for (int z = field->dimZ - 1; z >= 0; z--)
		for (int y = 0; y < field->dimY; y++)
			for (int x = 0; x < field->dimX; x++)
				if (!field->isEmpty(x, y, z))
					return z + 1;
	return 0;
}

// animate

void Tet3D::animate()
{
	if (playing && !paused && timer.getTime() > levelPause[level % LEVELCOUNT]) drop1();

	nextAnimList.update();
	if (timerNext.getTime() > 0.1)
	{
		timerNext.reset();
		nextA += 3;
		nextB += 5;
	}
}

// render

void Tet3D::render()
{
	// crtaj polje

	if (field != NULL)
	{
		field->drawField = !paused;
		field->render();
	}

	// crtaj "senku" bloka

	if (!paused && block != NULL && drawShadows) renderShadow();

	// crtaj blok

	if (!paused && block != NULL)
	{
		glPushMatrix();

		// dovedi centar koord sistema tamo gde centar block-a treba da se nalazi
		glTranslatef(
			-field->dimX / 2.0f + block->dimX / 2.0f + blockX,
			-field->dimY / 2.0f + block->dimY / 2.0f + blockY,
			-field->dimZ / 2.0f + block->dimZ / 2.0f + blockZ);

		block->render();

		glPopMatrix();
	}
}

void Tet3D::renderShadow() const
{
	bool m[5][5]; // x, y

	int x, y, z;

	for (x = 0; x < 5; x++)
		for (y = 0; y < 5; y++)
			m[x][y] = false;

	for (z = 0; z < block->dimZ; z++)
		for (y = 0; y < block->dimY; y++)
			for (x = 0; x < block->dimX; x++)
				m[x][y] = !block->isEmpty(x, y, z) || m[x][y];

	float x0 = -field->dimX / 2.0f;
	float y0 = -field->dimY / 2.0f;
	float z0 = -field->dimZ / 2.0f;

	glDisable(GL_LIGHTING);

	glColor4f(0.0f, 0.0f, 0.0f, 0.15f);
	for (x = 0; x < block->dimX; x++)
		for (y = 0; y < block->dimY; y++)
			if (m[x][y])
			{
				int height = getHeight(blockX + x, blockY + y);
				glBegin(GL_QUADS);
					glVertex3f(x0 + blockX + x - 0.0f, y0 + blockY + y - 0.0f, z0 + height);
					glVertex3f(x0 + blockX + x + 1.0f, y0 + blockY + y - 0.0f, z0 + height);
					glVertex3f(x0 + blockX + x + 1.0f, y0 + blockY + y + 1.0f, z0 + height);
					glVertex3f(x0 + blockX + x - 0.0f, y0 + blockY + y + 1.0f, z0 + height);
				glEnd();
			}

	glEnable(GL_LIGHTING);
}

void frame(float x1, float y1, float x2, float y2, float d)
{
	glBegin(GL_QUADS);
		glVertex2f(x1 - d, y2 + d);
		glVertex2f(x1 - d, y2    );
		glVertex2f(x2 + d, y2    );
		glVertex2f(x2 + d, y2 + d);

		glVertex2f(x1 - d, y1    );
		glVertex2f(x1 - d, y1 - d);
		glVertex2f(x2 + d, y1 - d);
		glVertex2f(x2 + d, y1    );

		glVertex2f(x1 - d, y2);
		glVertex2f(x1 - d, y1);
		glVertex2f(x1    , y1);
		glVertex2f(x1    , y2);

		glVertex2f(x2    , y2);
		glVertex2f(x2    , y1);
		glVertex2f(x2 + d, y1);
		glVertex2f(x2 + d, y2);
	glEnd();
}

void Tet3D::renderHUD() const
{
	//---//

	colorHUD.activate();

	char cbuffer[7];

	static const float dy1 = 0.24f;
	//static const float dy2 = dy1 / 2;
	static const float dy3 = dy1 / 3;
	static const float dy4 = dy1 / 4;
	static const float dy5 = dy1 / 5;
	static const float dy6 = dy1 / 6;
	static const float dy7 = dy1 / 7;
	static const float dy8 = dy1 / 8;
	static const float dy9 = dy1 / 9;

	static const float x_hud = 1.0f - dy9 * 10;

	float y_hud = 1.0f - dy9;

	snprintf(cbuffer, 7, " %c ", level <= 9 ? '0' + (char)level : 'x' );
	MenuChar::drawString2D("level", x_hud, y_hud -= dy5, dy5);
	MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy3, dy3);
	y_hud -= dy4;
	snprintf(cbuffer, 7, "%06d", uint(score_count) % 1000000);
	MenuChar::drawString2D(" score ", x_hud, y_hud -= dy7, dy7);
	MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy6, dy6);
	y_hud -= dy8;
	snprintf(cbuffer, 7, "%06d", block_count);
	MenuChar::drawString2D(" block ", x_hud, y_hud -= dy7, dy7);
	MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy6, dy6);
	y_hud -= dy8;
	snprintf(cbuffer, 7, "%06d", cube_count);
	MenuChar::drawString2D(" cubes ", x_hud, y_hud -= dy7, dy7);
	MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy6, dy6);
	y_hud -= dy8;
	snprintf(cbuffer, 7, "%06d", lines_count);
	MenuChar::drawString2D(" layers ", x_hud, y_hud -= dy8, dy8);
	MenuChar::drawString2D(cbuffer,  x_hud, y_hud -= dy6, dy6);
	y_hud -= dy4;
	snprintf(cbuffer, 7, "%1dx%1dx%2d", field->dimX, field->dimY, field->dimZ);
	MenuChar::drawString2D("  area  ", x_hud, y_hud -= dy8, dy8);
	MenuChar::drawString2D(cbuffer,  x_hud, y_hud -= dy6, dy6);
	y_hud -= dy8;

	//---//

	MenuChar::drawString2D("block set", x_hud, y_hud -= dy9, dy9);
	switch (blockSet)
	{
	default:
	case FLAT:     snprintf(cbuffer, 7, "%6s", " flat "); MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy6, dy6); break;
	case BASIC:    snprintf(cbuffer, 7, "%5s",  "basic"); MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy5, dy5); break;
	case EXTENDED: snprintf(cbuffer, 7, "%6s", "extend"); MenuChar::drawString2D(cbuffer, x_hud, y_hud -= dy6, dy6); break;
	}
	y_hud -= dy4;

	//---//

	MenuChar::drawString2D("next", x_hud, y_hud -= dy4, dy4);

	glPushMatrix();
	glTranslatef(x_hud + dy1 / 2.0f, y_hud - 0.1f + nextAnimList.dy, 0.0f);
	for (int i = 0; i < NEXTBLOCKS && i < 5; i++)
		if (next[i] != NULL)
		{
			glPushMatrix();
			glRotatef(nextB, 0, 1, 0);
			glRotatef(nextA, 1, 0, 1);
			glScalef(0.05f, 0.05f, 0.05f);
			next[i]->draw();
			glPopMatrix();
			glTranslatef(0.0f, -NEXTBLOCKY, 0.0f);
		}
	glPopMatrix();

	//---//

	colorHUD.activate();

	if (!playing)
	{
		MenuChar::drawString2Dc("game",  0.00f, 0.15f);
		MenuChar::drawString2Dc("over", -0.15f, 0.15f);
	}

	if (paused)
		MenuChar::drawString2Dc("paused", -0.075f, 0.15f);

	//---//

	int x, y, z;

	int height = getHeight();

	float x0 = -0.95f;
	float y0 = -0.95f;
	float x1 = -0.80f;
	float y1 =  0.95f;

	float dx = x1 - x0;
	float dy = (y1 - y0) / (float)field->dimZ;

	float ty = y0;

	glBindTexture(GL_TEXTURE_2D, Brick::texture);

	glBegin(GL_QUADS);
	for (z = 0; z < field->dimZ; z++)
	{
		if (z < height)
		{
			int color = z % brickColorsNum;
			glColor4f(brickColorsR[color], brickColorsG[color], brickColorsB[color], colorHUD.getAlpha());
		}
		else
			glColor4f(0.0f, 0.0f, 0.0f, colorHUD.getAlpha());

		glTexCoord2f(0.0f, 0.0f); glVertex2f(x0     , ty     );
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x0 + dx, ty     );
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x0 + dx, ty + dy);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x0     , ty + dy);

		ty += dy;
	}
	glEnd();

	ty = y0;

	colorHUD.activate();
	frame(x0, y0, x1, y1, 0.02f);

	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	for (z = 0; z < field->dimZ; z++, ty += dy)
		frame(x0 + 0.01f, ty + 0.01f, x1 - 0.01f, ty + dy - 0.01f, 0.005f);

	//---//

	if (block != NULL)
	{
		bool m[5];

		for (z = 0; z < 5; z++) m[z] = false;

		for (z = 0; z < block->dimZ; z++)
			for (y = 0; y < block->dimY; y++)
				for (x = 0; x < block->dimX; x++)
					m[z] = !block->isEmpty(x, y, z) || m[z];

		float by1 = y0 + (blockZ + block->dimZ) * dy;
		float by0 = y0 + blockZ * dy;

		for (int z = 0; z < block->dimZ; z++)
			if (!m[z])
				by0 += dy;
			else
				break;

		for (int z = block->dimZ - 1; z >= 0; z--)
			if (!m[z])
				by1 -= dy;
			else
				break;

		glColor4f(1.0f, 1.0f, 1.0f, colorHUD.getAlpha());
		frame(x0, by0, x1, by1, 0.01f);
	}
}

// block creation

BrickField* Tet3D::createBlock()
{
	BrickField* b;

	int number;

	switch (blockSet)
	{
	default:
	case FLAT:
		number = random.next(8);
		break;
	case BASIC:
		number = random.next(7) + 4;
		break;
	case EXTENDED:
		number = random.next(41);
		break;
	}

	switch (number)
	{
		// flat block set

	default:
	case 0:
		b = new BrickField(1, 1, 1);
		b->set(0, 0, 0, new Brick()); // *
		break;

	case 1:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 1, new Brick()); // **
		b->set(1, 0, 1, new Brick());
		break;

	case 2:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // ***
		b->set(1, 1, 1, new Brick());
		b->set(2, 1, 1, new Brick());
		break;

	case 3:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 1, new Brick()); // **
		b->set(1, 0, 1, new Brick()); // **
		b->set(0, 1, 1, new Brick());
		b->set(1, 1, 1, new Brick());
		break;

		// basic block set (7)

	case 4:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 1, new Brick()); // *
		b->set(1, 0, 1, new Brick()); // **
		b->set(0, 1, 1, new Brick());
		break;

	case 5:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		break;

	case 6:
		b = new BrickField(3, 3, 3);
		b->set(0, 0, 1, new Brick()); //  **
		b->set(1, 0, 1, new Brick()); // **
		b->set(1, 1, 1, new Brick());
		b->set(2, 1, 1, new Brick());
		break;

	case 7:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		break;

	case 8:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 0, new Brick()); // *
		b->set(0, 1, 0, new Brick()); // #*
		b->set(1, 0, 0, new Brick());
		b->set(0, 0, 1, new Brick());
		break;

	case 9:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 0, new Brick()); // **
		b->set(0, 1, 0, new Brick()); // #
		b->set(1, 1, 0, new Brick());
		b->set(0, 0, 1, new Brick());
		break;

	case 10:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 0, new Brick()); //  *
		b->set(1, 1, 0, new Brick()); // #*
		b->set(1, 0, 0, new Brick());
		b->set(0, 0, 1, new Brick());
		break;

		// extended block set

	case 11:
		b = new BrickField(4, 4, 4);
		b->set(0, 1, 1, new Brick()); // ****
		b->set(1, 1, 1, new Brick());
		b->set(2, 1, 1, new Brick());
		b->set(3, 1, 1, new Brick());
		break;

	case 12:
		b = new BrickField(5, 5, 5);
		b->set(0, 2, 2, new Brick()); // *****
		b->set(1, 2, 2, new Brick());
		b->set(2, 2, 2, new Brick());
		b->set(3, 2, 2, new Brick());
		b->set(4, 2, 2, new Brick());
		break;

	case 13:
		b = new BrickField(4, 4, 4);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // ****
		b->set(2, 1, 1, new Brick());
		b->set(3, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		break;

	case 14:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // **
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		break;

	case 15:
		b = new BrickField(3, 3, 3);
		b->set(0, 0, 0, new Brick()); // *
		b->set(1, 0, 0, new Brick()); // *
		b->set(2, 0, 0, new Brick()); // ***
		b->set(0, 1, 0, new Brick());
		b->set(0, 2, 0, new Brick());
		break;

	case 16:
		b = new BrickField(4, 4, 4);
		b->set(1, 1, 1, new Brick()); // **
		b->set(2, 1, 1, new Brick()); //  ***
		b->set(3, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		break;

	case 17:
		b = new BrickField(4, 4, 4);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // ****
		b->set(2, 1, 1, new Brick());
		b->set(3, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		break;

	case 18:
		b = new BrickField(3, 3, 3);
		b->set(0, 0, 1, new Brick()); //  *
		b->set(1, 0, 1, new Brick()); //  *
		b->set(2, 0, 1, new Brick()); // ***
		b->set(1, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());

	case 19:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick()); //  *
		b->set(0, 2, 1, new Brick());
		b->set(1, 0, 1, new Brick());
		break;

	case 20:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick()); //   *
		b->set(0, 2, 1, new Brick());
		b->set(2, 0, 1, new Brick());
		break;

	case 21:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // * *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(2, 2, 1, new Brick());
		break;

	case 22:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick()); //  *
		b->set(1, 0, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		break;

	case 23:
		b = new BrickField(3, 3, 3);
		b->set(0, 2, 1, new Brick()); // *
		b->set(0, 1, 1, new Brick()); // **
		b->set(1, 1, 1, new Brick()); //  **
		b->set(1, 0, 1, new Brick());
		b->set(2, 0, 1, new Brick());
		break;

	case 24:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // #**
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(0, 1, 2, new Brick());
		break;

	case 25:
		b = new BrickField(3, 3, 3);
		b->set(1, 0, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // *
		b->set(1, 2, 1, new Brick()); // #*
		b->set(2, 0, 1, new Brick());
		b->set(1, 0, 2, new Brick());
		break;

	case 26:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // #
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(0, 2, 2, new Brick());
		break;

	case 27:
		b = new BrickField(3, 3, 3);
		b->set(1, 0, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // *
		b->set(1, 2, 1, new Brick()); // *#
		b->set(2, 0, 1, new Brick());
		b->set(2, 0, 2, new Brick());
		break;

	case 28:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // *#*
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(1, 1, 2, new Brick());
		break;

	case 29:
		b = new BrickField(3, 3, 3);
		b->set(1, 0, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // #
		b->set(1, 2, 1, new Brick()); // **
		b->set(2, 0, 1, new Brick());
		b->set(1, 1, 2, new Brick());
		break;

	case 30:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // **#
		b->set(2, 1, 1, new Brick());
		b->set(0, 2, 1, new Brick());
		b->set(2, 1, 2, new Brick());
		break;

	case 31:
		b = new BrickField(3, 3, 3);
		b->set(1, 0, 1, new Brick()); // #
		b->set(1, 1, 1, new Brick()); // *
		b->set(1, 2, 1, new Brick()); // **
		b->set(2, 0, 1, new Brick());
		b->set(1, 2, 2, new Brick());
		break;

	case 32:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  #
		b->set(1, 1, 1, new Brick()); // ***
		b->set(2, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		b->set(1, 2, 2, new Brick());
		break;

	case 33:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // #**
		b->set(2, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		b->set(0, 1, 2, new Brick());
		break;

	case 34:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // *#*
		b->set(2, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		b->set(1, 1, 2, new Brick());
		break;

	case 35:
		b = new BrickField(3, 3, 3);
		b->set(0, 1, 1, new Brick()); //  *
		b->set(1, 1, 1, new Brick()); // **#
		b->set(2, 1, 1, new Brick());
		b->set(1, 2, 1, new Brick());
		b->set(2, 1, 2, new Brick());
		break;

	case 36:
		b = new BrickField(3, 3, 3);
		b->set(0, 0, 1, new Brick()); //  **
		b->set(1, 0, 1, new Brick()); // #*
		b->set(1, 1, 1, new Brick());
		b->set(2, 1, 1, new Brick());
		b->set(0, 0, 2, new Brick());
		break;

	case 37:
		b = new BrickField(3, 3, 3);
		b->set(0, 0, 1, new Brick()); //  **
		b->set(1, 0, 1, new Brick()); // *#
		b->set(1, 1, 1, new Brick());
		b->set(2, 1, 1, new Brick());
		b->set(1, 0, 2, new Brick());
		break;

	case 38:
		b = new BrickField(2, 2, 2);
		b->set(0, 0, 0, new Brick()); // **
		b->set(1, 0, 0, new Brick()); // #*
		b->set(0, 1, 0, new Brick());
		b->set(1, 1, 0, new Brick());
		b->set(0, 0, 1, new Brick());
		break;

	case 39:
		b = new BrickField(3, 3, 3);
		b->set(1, 1, 1, new Brick()); // ^#
		b->set(2, 1, 1, new Brick()); //  **
		b->set(1, 2, 1, new Brick());
		b->set(0, 2, 2, new Brick());
		b->set(1, 2, 2, new Brick());
		break;

	case 40:
		b = new BrickField(3, 3, 3);
		b->set(1, 2, 1, new Brick()); // *
		b->set(1, 1, 1, new Brick()); // *#
		b->set(2, 1, 1, new Brick()); //  ^
		b->set(2, 1, 2, new Brick());
		b->set(2, 0, 2, new Brick());
		break;
	}

	for (int z = 0; z < b->dimZ; z++)
		for (int y = 0; y < b->dimY; y++)
			for (int x = 0; x < b->dimX; x++)
			{
				Brick* q = b->get(x, y, z);
				if (q != NULL) q->color.set(colorHUD);
			}

	return b;
}
