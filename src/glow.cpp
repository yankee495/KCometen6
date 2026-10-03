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
#include "glow.h"
extern GLuint g_textures[];

Glow::Glow(CometenScene* scene, double time, const Vec3f& position, 
           double size, const Vec3f& normal, const Vec4f& _color)
: 
Comet(scene, size/2, position)
{
	done = false;
	texture = g_textures[0];
	color = _color;
	
	lifeTime = 1.5;
	startTime = time;
	
	dir = normal;
	Vec3f::generateOrthonormalBasis(dir, up, right);
	calcSize();
}

Glow::~Glow() {
}

void Glow::calcSize() {
	//Plane* plane;
	int i;
	double distance;
	double e = 5.0;
	Vec3f a,b;
		
	for (i = 0; i != scene->getNumPlanes(); i++) {
		const Plane& plane = scene->getPlane(i);
		
		if (dir.dot(plane.N) >= 0.999) {
			continue;
		}
		
		distance = position.dot(plane.N) - plane.d + e;
		size = clamp<double>(size, 0, distance / abs(plane.N.dot(up)));
		size = clamp<double>(size, 0, distance / abs(plane.N.dot(right)));
	} 
}

void Glow::process(double time, double /*unused*/) {
	double fadeinTime = min<double>(lifeTime/2, 0.125);
	
	if (time < startTime) {
		transparency = 0.0;
	} 
	else if (time < startTime + fadeinTime) {
		// fadein t 0->1
		transparency = (time - startTime) / fadeinTime;	
	}
	else if (time < startTime + lifeTime) {
		// fadeout t 1->0
		transparency = 1 
			- (time - startTime - fadeinTime) / (lifeTime-fadeinTime);
	}
	else if (time < startTime + fadeinTime) {
		// fadein t 0->1
		transparency = (time - startTime) / fadeinTime;
	}
	else {
		transparency = 0;
		done = true;
	}
}

void Glow::render(const Vec3f& /*unused*/) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glPushMatrix();
	
	glTranslatef(position[0], position[1], position[2]);
	glScalef(size, size, size);
	
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
	
	glPopMatrix();	
}

void Glow::destroy(double /*unused*/) {
}

bool Glow::isDestroyed() {
	return true;
}

bool Glow::isDone() {
	return done;
}


