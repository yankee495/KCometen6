/***************************************************************************
 *   Copyright (C) 2005 by Peter Müller                                    *
 *   pmueller@cs.tu-berlin.de                                              *
 *   Copyright (C) 2008 by John Stamp <jstamp@users.sourceforge.net>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/
#include "decal.h"

Decal::Decal(GLuint _texture, const Vec3f & _position,  double _size,
             bool _billboard, const Vec3f & _normal, const Vec4f & _color,
             double _transparency)
:
texture(_texture),
position(_position),
size(_size),
billboard(_billboard),
normal(_normal),
color(_color),
transparency(_transparency)
{
}


Decal::~Decal()
{
}



void Decal::render(const Vec3f & campos) {
	Vec3f dir, up, right;

	if (billboard) {
		dir = campos - position;
		dir.normalize();
	} else {
		dir = normal;
	}

	Vec3f::generateOrthonormalBasis(dir, up, right);

	// THE BILLBOARD SQUARE ALIGNMENT MATRIX:
	// Explicitly normalize the axis vectors before multiplying by size
	// to prevent the quad cards from twisting into warped diamonds at high scales!
	right.normalize();
	up.normalize();

	right *= 0.5 * size;
	up *= 0.5 * size;

	glPushMatrix();

	glTranslatef(position[0], position[1], position[2]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// THE TOTAL RECLAIMATION FIX: Enable Alpha Testing to vaporize the square depth cards!
	// This tells the GPU to completely discard any pixel with an alpha value lower than 0.1
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);

	extern GLuint g_textures[];
	glBindTexture(GL_TEXTURE_2D, g_textures[0]);

	glColor4f(color[0], color[1], color[2], color[3] * transparency);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 1.0);	
	glVertex3f((right[0] + up[0]), (right[1] + up[1]), (right[2] + up[2]));
	glTexCoord2f(1.0, 1.0);
	glVertex3f((-right[0] + up[0]), (-right[1] + up[1]), (-right[2] + up[2]));
	glTexCoord2f(1.0, 0.0);
	glVertex3f((-right[0] - up[0]), (-right[1] - up[1]), (-right[2] - up[2]));
	glTexCoord2f(0.0, 0.0);
	glVertex3f((right[0] - up[0]), (right[1] - up[1]), (right[2] - up[2]));
	
	glEnd();

	// Clear states cleanly so they don't leak into the next rendering pass
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}

