
/* anim.h
 *
 * Autor: Marko Gaćeša
 * Datum: 09.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _ANIM_H_
#define _ANIM_H_

#include "timer.h"
#include "random.h"

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
public:
	AnimObj() : anims() {}
	AnimObj(const AnimObj&) = delete; // prevent copy-construction
	virtual ~AnimObj() = default;

	AnimList anims;

	void render();

	virtual void draw() const =0;
};


#endif
