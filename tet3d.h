
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

#include "timer.h"
#include "random.h"
#include "brickfield.h"
#include "color.h"
#include "anim.h"


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

	static constexpr float animDuration = 0.25f; // [sekundi]
	static constexpr float slideTime = 0.55f; // [sekundi]

	static constexpr int NEXTBLOCKS = 5;
	static constexpr float NEXTSPAN = 0.8f;
	float nextScale = 1.0f;
	float nextDeltaY = 0.20f;
	BrickField* next[NEXTBLOCKS];
	Timer timerNext;
	AnimList nextAnimList;
	float nextA;
	float nextB;

	int blockX;
	int blockY;
	int blockZ;

	BlockSet blockSet;

	const Color colorHUD = Color(0.92f, 0.92f, 0.92f, 1.0f);
	const Color colorHeight = Color(0.55f, 0.5f, 0.4f, 0.7f);
	const Color colorActive = Color(1.0f, 1.0f, 0.6f, 0.7f);
	const Color colorNext = Color(1.0f, 1.0f, 0.6f, 1.0f);
	static constexpr float colorFieldAlpha = 0.6;

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

	void renderNextBlocks() const;
	void renderHUD() const;
	void renderHUDHeight() const;
};


#endif
