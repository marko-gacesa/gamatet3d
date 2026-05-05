
/* anim.cpp
 *
 * Autor: Marko Gacesa
 * Datum: 09.06.2007.
 */

#include <GL/glut.h>
#include <GL/gl.h>
#include "anim.h"

//------//
// Anim //
//------//

void Anim::update()
{
	if (finished) return;
	timeLength = timer.getTime() / duration;
	if (timeLength > 1.0f) { timeLength = 1.0f; finished = true; }
}

//----------//
// AnimList //
//----------//

void AnimList::update()
{
	Anim* prev = NULL;
	Anim* curr = first;

	clear();

	while (curr != NULL)
	{
		curr->update();

		dx += curr->dx();
		dy += curr->dy();
		dz += curr->dz();
		rx += curr->rx();
		ry += curr->ry();
		rz += curr->rz();
		sx += curr->sx();
		sy += curr->sy();
		sz += curr->sz();

		if (curr->finished)
		{
			if (prev == NULL)
				first = curr->next;
			else
				prev->next = curr->next;

			Anim* t = curr;
			curr = curr->next;
			t->next = NULL;
			delete t;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void AnimList::add(Anim *anim)
{
	anim->next = first;
	first = anim;
}

//---------//
// AnimObj //
//---------//

void AnimObj::render()
{
	anims.update();

	glPushMatrix();

	glTranslatef(anims.dx, anims.dy, anims.dz);

	glRotatef(anims.rz, 0, 0, 1);
	glRotatef(anims.ry, 0, 1, 0);
	glRotatef(anims.rx, 1, 0, 0);

	glScalef(1.0f + anims.sx, 1.0f + anims.sy, 1.0f + anims.sz);

	draw();

	glPopMatrix();
}
