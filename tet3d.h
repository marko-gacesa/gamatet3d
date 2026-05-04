
/* tet3d.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
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
	static const int LEVELCOUNT = 11;
	static float levelPause[LEVELCOUNT];

	WalledBrickField* field;
	BrickField* block;

	Timer timer;
	Random random;

	float animDuration;
	float slideTime;
	float blockAlpha;

	static const int NEXTBLOCKS = 10;
	static const float NEXTBLOCKY;
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

	bool playing;
	bool paused;

	int score_count;
	int block_count;
	int cube_count;
	int lines_count;
	int level;

	void paintBricks();

	bool isBlockOk() const;
	void nextBlock();

	bool zFull(int z) const;
	int meld();
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

	void setLevel(int l) { if (!playing && l >= 0 && l <= 9) level = l; }

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

	void render();

	void renderHUD() const;
};


#endif
