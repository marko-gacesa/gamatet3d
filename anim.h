
/* anim.h
 *
 * Autor: Marko Gacesa
 * Datum: 09.06.2007.
 */

#ifndef _ANIM_H_
#define _ANIM_H_


#include <math.h>
#include "timer.h"
#include "random.h"

#define NULL 0

/*
inline float deg2rad(float a) { return a * 0.01745329252f; }
inline float rad2deg(float a) { return a * 57.2957795131f; }

#define PI 3.1415926535897932384626433832795f
#define PIPI 6.283185307179586476925286766559f
#define PI2 1.5707963267948966192313216916398f
#define PI4 0.78539816339744830961566084581988f
#define SQRT2 1.4142135623730950488016887242097f
#define SQRT1_2 0.70710678118654752440084436210485f
*/

//------//
// Anim //
//------//

class Anim
{
private:
	Anim* next;
	Timer timer;

	float duration;
	bool finished;
	float timeLength;

protected:
	virtual float t() const { return timeLength; }

public:
	Anim(float dur) : next(NULL), timer(), duration(dur), finished(false), timeLength(1.0f) { timer.start(); }
	~Anim() { delete next; }

	void update();
	bool isFinished() const { return finished; }

	virtual float dx() { return 0.0f; }
	virtual float dy() { return 0.0f; }
	virtual float dz() { return 0.0f; }

	virtual float rx() { return 0.0f; }
	virtual float ry() { return 0.0f; }
	virtual float rz() { return 0.0f; }

	virtual float sx() { return 0.0f; }
	virtual float sy() { return 0.0f; }
	virtual float sz() { return 0.0f; }

	friend class AnimList;
};


class AnimLin : public Anim
{
protected:
	float x, y, z;
public:
	AnimLin(float dx, float dy, float dz, float duration) : Anim(duration) { x = -dx; y = -dy; z = -dz; }
	virtual float dx() { return x * (1.0f - t()); }
	virtual float dy() { return y * (1.0f - t()); }
	virtual float dz() { return z * (1.0f - t()); }
};


class AnimQuad : public AnimLin
{
public:
	AnimQuad(float dx, float dy, float dz, float duration) : AnimLin(dx, dy, dz, duration) {}
	virtual float t() const { return AnimLin::t() * AnimLin::t(); }
};


class AnimRotX : public Anim
{
protected:
	bool ccw;
public:
	AnimRotX(bool ccwRot, float duration) : Anim(duration), ccw(ccwRot) {}
	virtual float rx() { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimRotY : public Anim
{
protected:
	bool ccw;
public:
	AnimRotY(bool ccwRot, float duration) : Anim(duration), ccw(ccwRot) {}
	virtual float ry() { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimRotZ : public Anim
{
protected:
	bool ccw;
public:
	AnimRotZ(bool ccwRot, float duration) : Anim(duration), ccw(ccwRot) {}
	virtual float rz() { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimShake : public Anim
{
protected:
	Random random;
	float intensity;
public:
	AnimShake(float intense, float duration) : Anim(duration), random(), intensity(intense) {}
	virtual float dx() { return (1.0f - t()) * ((float)random.next() - 0.5f) * intensity; }
	virtual float dy() { return (1.0f - t()) * ((float)random.next() - 0.5f) * intensity; }
	virtual float dz() { return (1.0f - t()) * ((float)random.next() - 0.5f) * intensity; }
};

//----------//
// AnimList //
//----------//

class AnimList
{
private:
	Anim* first;
	void clear() { dx = dy = dz = rx = ry = rz = sx = sy = sz = 0.0f; }

public:
	AnimList() : first(NULL) { clear(); }
	~AnimList() { delete first; }

	float dx, dy, dz, rx, ry, rz, sx, sy, sz;

	bool isAnimating() { return first != NULL; }

	void update();

	void add(Anim* anim);
};

//---------//
// AnimObj //
//---------//

class AnimObj
{
private:
	AnimObj(const AnimObj&); // prevent copy-construction
public:
	AnimObj() : anims() {}

	AnimList anims;

	void render();

	virtual void draw() const =0;
};


#endif

//\\//\\
