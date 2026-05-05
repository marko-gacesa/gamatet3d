
/* color.h
 *
 * Autor: Marko Gaćeša
 * Datum: 10.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include <GL/gl.h>

class Color
{
	GLfloat v[4];
public:
	Color() : v{1.0f, 1.0f, 1.0f, 1.0f} {}
	Color(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) : v{r, g, b, a} {}
	Color(const Color& c) { set(c); }

	void set(const Color& c) { for (int i = 0; i < 4; i++) v[i] = c.v[i]; }
	void set(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
	{
		v[0] = r; v[1] = g; v[2] = b; v[3] = a;
	}
	Color get() const { return *this; }

	void setRed(const GLfloat r) { v[0] = r; }
	GLfloat getRed() const { return v[0]; }

	void setGreen(const GLfloat g) { v[1] = g; }
	GLfloat getGreen() const { return v[1]; }

	void setBlue(const GLfloat b) { v[2] = b; }
	GLfloat getBlue() const { return v[2]; }

	void setAlpha(const GLfloat a) { v[3] = a; }
	GLfloat getAlpha() const { return v[3]; }

	void activate() const { glColor4fv(v); }
};

#endif
