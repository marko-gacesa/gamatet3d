
/* tet3d.h
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _TET3D_H_
#define _TET3D_H_

#include <utility>
#include "timer.h"
#include "random.h"
#include "brickfield.h"
#include "color.h"
#include "anim.h"

typedef struct {
	float x, y;
} xy;

class Tet3D
{
public:
	enum BlockSet { FLAT, BASIC, EXTENDED };

private:
	static constexpr int LEVELCOUNT = 11;
	static float levelPause[LEVELCOUNT];

	WalledBrickField* field;
	BrickField* block;

	Timer timer;
	Random random;

	float animDuration;
	float slideTime;
	float blockAlpha;

	static constexpr int NEXTBLOCKS = 10;
	static constexpr float NEXTBLOCKY = 0.20f;
	BrickField* next[NEXTBLOCKS];
	Timer timerNext;
	AnimList nextAnimList;
	float nextA;
	float nextB;

	int blockX;
	int blockY;
	int blockZ;

	BlockSet blockSet;

	Color colorHUD;
	Color colorNext;

	bool playing;
	bool paused;

	int score_count;
	int block_count;
	int cube_count;
	int lines_count;
	int level;

	void paintBricks() const;

	bool isBlockOk() const;
	void nextBlock();

	bool zFull(int z) const;
	int meld() const;
	void drop1();
	bool rotAdjust();

	int getHeight(int x, int y) const;
	int getHeight() const;

	void renderShadow() const;

	BrickField* createBlock();

public:
	Tet3D();
	~Tet3D();

	// svojstva

	bool drawShadows;

	int width()  const { return field->dimX; }
	int height() const { return field->dimY; }
	int depth()  const { return field->dimZ; }

	bool isPaused() const { return paused; }
	bool isPlaying() const { return playing; }

	// kreiranje polja za igru

	void generateField(int dimX, int dimY, int dimZ, BlockSet bset);
	void deleteField();

	BlockSet getBlockSet() const { return blockSet; }
	static char getBlockSetCode(BlockSet bset);

	// start/stop

	void setLevel(const int l) { if (!playing && l >= 0 && l <= 9) level = l; }

	void gameStart();
	void gameOver();

	void pause();

	// manipulacija trenutnim blokom

	void drop();
	void moveX(int dx);
	void moveY(int dy);
	void moveZ(int dz);
	void rotateX(bool ccw);
	void rotateY(bool ccw);
	void rotateZ(bool ccw);

	// animiranje

	void animate();

	// crtanje

	void render() const;

	xy renderHUD() const;
	void renderHUDNext(float posX, float posY) const;
	void renderHUDHeight() const;
};


#endif
