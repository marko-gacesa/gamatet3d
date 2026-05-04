
/* brick.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#ifndef _BRICK_H_
#define _BRICK_H_

#include "random.h"
#include "anim.h"
#include "color.h"
#include "gl/glut.h"
#include "gl/gl.h"

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

	virtual void draw() const;
};


#endif

//\\//\\
