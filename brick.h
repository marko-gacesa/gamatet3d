
/* brick.h
 *
 * Autor: Marko Gacesa
 * Datum: 05.06.2007.
 */

#ifndef _BRICK_H_
#define _BRICK_H_

#include <GL/glut.h>
#include <GL/gl.h>
#include "random.h"
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

	virtual void draw() const;
};


#endif
