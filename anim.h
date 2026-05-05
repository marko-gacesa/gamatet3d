
/* anim.h
 *
 * Autor: Marko Gacesa
 * Datum: 09.06.2007.
 * Datum: 05.05.2026.
 */

#ifndef _ANIM_H_
#define _ANIM_H_


#include "timer.h"
#include "random.h"

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
	explicit Anim(const float dur) : next(nullptr), timer(), duration(dur), finished(false), timeLength(1.0f) { timer.start(); }
	virtual ~Anim() { delete next; }

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
	AnimLin(const float dx, const float dy, const float dz, const float duration) : Anim(duration) {
		x = -dx; y = -dy; z = -dz;
	}
	float dx() override { return x * (1.0f - t()); }
	float dy() override { return y * (1.0f - t()); }
	float dz() override { return z * (1.0f - t()); }
};


class AnimQuad : public AnimLin
{
protected:
	float t() const override { return AnimLin::t() * AnimLin::t(); }
public:
	AnimQuad(const float dx, const float dy, const float dz, const float duration) : AnimLin(dx, dy, dz, duration) {}
};


class AnimRotX : public Anim
{
protected:
	bool ccw;
public:
	AnimRotX(const bool ccwRot, const float duration) : Anim(duration), ccw(ccwRot) {}
	float rx() override { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimRotY : public Anim
{
protected:
	bool ccw;
public:
	AnimRotY(const bool ccwRot, const float duration) : Anim(duration), ccw(ccwRot) {}
	float ry() override { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimRotZ : public Anim
{
protected:
	bool ccw;
public:
	AnimRotZ(const bool ccwRot, const float duration) : Anim(duration), ccw(ccwRot) {}
	float rz() override { return (1.0f - t()) * (ccw ? -90.0f : 90.0f); }
};

class AnimShake : public Anim
{
protected:
	Random random;
	float intensity;
public:
	AnimShake(const float intense, const float duration) : Anim(duration), random(), intensity(intense) {}
	float dx() override { return (1.0f - t()) * (static_cast<float>(random.next()) - 0.5f) * intensity; }
	float dy() override { return (1.0f - t()) * (static_cast<float>(random.next()) - 0.5f) * intensity; }
	float dz() override { return (1.0f - t()) * (static_cast<float>(random.next()) - 0.5f) * intensity; }
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
	AnimList() : first(nullptr), dx(0), dy(0), dz(0), rx(0), ry(0), rz(0), sx(0), sy(0), sz(0) {}
	~AnimList() { delete first; }

	float dx, dy, dz, rx, ry, rz, sx, sy, sz;

	bool isAnimating() const { return first != nullptr; }

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
	virtual ~AnimObj() = default;

	AnimList anims;

	void render();

	virtual void draw() const =0;
};


#endif
