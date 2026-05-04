
/* color.h
 *
 * Autor: Marko Gacesa
 * Datum: 10.06.2007.
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include <GL/glut.h>
#include <GL/gl.h>

class Color
{
	GLfloat v[4];
public:
	Color() { for (int i = 0; i < 4; i++) v[i] = 1.0f; }
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { v[0] = r; v[1] = g; v[2] = b; v[3] = a; }
	Color(const Color& c) { set(c); }

	void set(const Color& c) { for (int i = 0; i < 4; i++) v[i] = c.v[i]; }
	void set(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { v[0] = r; v[1] = g; v[2] = b; v[3] = a; }
	Color get() const { return Color(*this); }

	void setRed(GLfloat r) { v[0] = r; }
	GLfloat getRed() const { return v[0]; }

	void setGreen(GLfloat g) { v[1] = g; }
	GLfloat getGreen() const { return v[1]; }

	void setBlue(GLfloat b) { v[2] = b; }
	GLfloat getBlue() const { return v[2]; }

	void setAlpha(GLfloat a) { v[3] = a; }
	GLfloat getAlpha() const { return v[3]; }

	void activate() const { glColor4fv(v); }
};

#endif
