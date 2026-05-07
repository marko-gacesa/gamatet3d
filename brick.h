
/* brick.h
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _BRICK_H_
#define _BRICK_H_

#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "anim.h"
#include "color.h"

class Brick : public AnimObj
{
private:
	static GLuint listBlock;
	static void drawBrick();
public:
	Brick() : color() {}

	static GLuint texture;

	static void compileList();

	Color color;

	void draw() const override;
};


#endif
