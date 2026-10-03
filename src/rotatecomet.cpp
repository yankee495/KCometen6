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
#include "rotatecomet.h"
extern GLuint g_textures[];
#include "settings.h" // THE INCLUDE FIX: Allows the file to safely read sizeScale parameters!
extern GLuint g_textures[];

RotateComet::RotateComet(CometenScene * scene, double time, double _lifeTime,
						 const Vec3f & _pos, const Vec3f & _vel,
						 const Vec4f & _acolor1, const Vec4f & _ecolor1,
						 const Vec4f & _acolor2, const Vec4f & _ecolor2)
:
// THE 4K SCALE MATRIX: Inject sizeScale safely to expand the physical object presence!
Comet(scene, settings->sizeScale, _pos),
startTime(time),
lifeTime(_lifeTime),
acolor1(_acolor1),
ecolor1(_ecolor1),
acolor2(_acolor2),
ecolor2(_ecolor2)
{
	GLuint tex = g_textures[0];
	dir = _vel;
	vel = dir.normalize();
	Vec3f::generateOrthonormalBasis(dir, up, right);

	// Multiply constraints smoothly by the scale multiplier factors
	radius = 1.75 * settings->sizeScale;

	psystem1 = new ParticleSystem(time, tex, _pos, _pos, 800, 0, 1.5, 2.0,
								  true, 400, 0.0, 4.0, true, 3.0 * settings->sizeScale,
							   acolor1, ecolor1);

	decal1 = new Decal(tex, _pos, 6.0 * settings->sizeScale, true, Vec3f::UNIT_X, acolor1);

	psystem2 = new ParticleSystem(time, tex, _pos, _pos, 800, 0, 1.5, 2.0,
								  true, 400, 0.0, 4.0, false, 3.0 * settings->sizeScale,
							   acolor2, ecolor2);

	decal2 = new Decal(tex, _pos, 6.0 * settings->sizeScale, true, Vec3f::UNIT_X, acolor2);

	destroyed = false;
}

RotateComet::~RotateComet() {
	delete psystem1;
	delete decal1;
	delete psystem2;
	delete decal2;
}

void RotateComet::process(double time, double deltat) {
	int i;
	int hitplane;
	double hittime;
	double t;
	double r;
	double w;

	if (!destroyed) {
		hitplane = -1;
		hittime = deltat;
		for (i = 0; i != scene->getNumPlanes(); i++) {
			// kill comets outside of the box
			if (position.dot(scene->getPlane(i).N)
				< scene->getPlane(i).d)
			{
				destroy(time);
				break;
			}

			if (TestDynamicSpherePlane(position, radius, dir*vel,
				scene->getPlane(i), t) &&
				t <= hittime)
			{
				hitplane = i;
				hittime = t;
			}
		}

		position += dir * (vel * hittime);
		if (hitplane != -1) {
			hitPlane(time + hittime, scene->getPlane(hitplane));
		}

		if (time - startTime > lifeTime) {
			destroy(time);
		}
	}

	t = (time - startTime) / lifeTime;

	if (t < 0.05) {
		r = 20 * t * radius;
	} else if (t > 0.95) {
		r = 20 * (1-t) * radius;
	}
	else {
		r = radius;
	}

	w = 12.0*time;

	pos1 = sinf(w)*r*up+cosf(w)*r*right + position;
	pos2 = sinf(w+Pi)*r*up+cosf(w+Pi)*r*right + position;

	decal1->setPosition(pos1);
	psystem1->setEmitterPos1(psystem1->getEmitterPos2());
	psystem1->setEmitterPos2(pos1);
	psystem1->process(time);
	decal2->setPosition(pos2);
	psystem2->setEmitterPos1(psystem2->getEmitterPos2());
	psystem2->setEmitterPos2(pos2);
	psystem2->process(time);

	if (destroyed) {
		psystem1->stop();
		psystem2->stop();
	}
}

void RotateComet::render(const Vec3f& campos) {
	if (!destroyed) {
		decal1->render(campos);
		decal2->render(campos);
	}
	psystem1->render(campos);
	psystem2->render(campos);
}

void RotateComet::hitPlane(double time, const Plane & plane) {
	double distance;
	const double e = 0.0001;
	Vec3f hitPoint1;
	Vec3f hitPoint2;
	double decalSize;
	Comet* glow;

	distance = pos1.dot(plane.N) - plane.d;
	hitPoint1 = pos1 - plane.N * (distance - 0.0001);
	distance = pos2.dot(plane.N) - plane.d;
	hitPoint2 = pos2 - plane.N * (distance - 0.0001);
	distance = position.dot(plane.N) - plane.d;

	// if comet intersects plane move along normal
	if (distance >= 0 && distance <= radius + e) {
		position += (plane.N * (radius + e - distance));
	}
	else if (distance < 0 && distance >= -radius - e) {
		position += (plane.N * (-radius - e + distance));
	}

	// 	bounce comet
	dir.reflect(plane.N);
	Vec3f::generateOrthonormalBasis(dir, up, right);

	// Scale your impact decal blast rings up smoothly for 4K visibility!
	decalSize = 50.0 * settings->sizeScale;

	glow = new Glow(scene, time, hitPoint1, decalSize, plane.N, acolor1);
	scene->addComet(glow);

	glow = new Glow(scene, time, hitPoint2, decalSize, plane.N, acolor2);
	scene->addComet(glow);
}

void RotateComet::destroy(double time) {
	Comet * newc;
	Vec3f pos1, pos2;
	int numChildren = rand(4,6);
	double w;
	Vec3f normal;

	for (int i = 0; i < numChildren; i++) {
		double t1 = frand();
		double t2 = frand();
		w = Pi*(double)i/numChildren;
		normal = sinf(w) * up + cosf(w) * right;
		newc = new SpComet(scene, time, position,
						   normal, 18, t1*acolor1+(1-t1)*acolor2,
						   t2*ecolor1+(1-t2)*ecolor2);
		scene->addComet(newc);
	}

	newc = new Explosion(scene, time, 1.0, position, 6 * settings->sizeScale, 400, acolor2, ecolor2);
	scene->addComet(newc);

	psystem1->stop();
	psystem2->stop();
	destroyed = true;
}

bool RotateComet::isDestroyed() {
	return destroyed;
}

bool RotateComet::isDone() {
	return destroyed && psystem1->done() && psystem2->done();
}


/*
 *  SmallCurveComet -> SpComet
 */

SpComet::SpComet(CometenScene * scene, double time, const Vec3f & _pos,
				 const Vec3f & normal, double _maxRadius,
				 const Vec4f & _acolor, const Vec4f & _ecolor)
:
Comet(scene, 0.25, _pos),
startPos(_pos),
maxRadius(_maxRadius),
startTime(time),
acolor(_acolor),
ecolor(_ecolor),
destroyed(false)
{
	GLuint tex = g_textures[0];
	dir = normal;
	Vec3f::generateOrthonormalBasis(dir, up, right);
	up = Vec3f::randomUnitOnPlane(up, right);
	right = dir.cross(up);

	lifeTime = 0.75;
	psystem = new ParticleSystem(time, tex, _pos, _pos, 500, 0, 2.5, 3.0,
								 true, 670, 0.0, 2.5,
							  true, 2.0 * settings->sizeScale, acolor, ecolor);

	decal = new Decal(tex, _pos, 2.5 * settings->sizeScale, true, Vec3f::UNIT_X, acolor);
}

SpComet::~SpComet() {
	delete psystem;
	delete decal;
}

void SpComet::process(double time, double /*unused*/) {
	double t;
	double r;
	double w;
	double bw = scene->getBoxWidth()/2;
	double bh = scene->getBoxHeight()/2;

	t = (time - startTime) / lifeTime;
	if (t <= 0.25) {
		r = 4*t*maxRadius;
	}
	else if (t >= 0.75) {
		r = (1 - t) * 4 * maxRadius;
	}
	else {
		r = maxRadius;
	}

	w = t*4*Pi;
	position = sinf(w)*r*up+cosf(w)*r*right + startPos;
	position[0] = clamp<float>(position[0], -bw, bw);
	position[1] = clamp<float>(position[1], -bh, bh);
	position[2] = clamp<float>(position[2], -bw, bw);

	decal->setPosition(position);
	psystem->setEmitterPos1(psystem->getEmitterPos2());
	psystem->setEmitterPos2(position);
	psystem->process(time);

	if (!destroyed) {
		if (time - startTime > lifeTime) {
			destroy(time);
		}
	}
}

void SpComet::render(const Vec3f& campos) {
	if (!destroyed) {
		decal->render(campos);
	}

	psystem->render(campos);
}

void SpComet::destroy(double time) {
	Comet * newc = 0;

	newc = new Explosion(scene, time, 1.0, position, 6 * settings->sizeScale, 100, acolor, ecolor);
	scene->addComet(newc);

	psystem->stop();
	destroyed = true;
}

bool SpComet::isDestroyed() {
	return destroyed;
}

bool SpComet::isDone() {
	return destroyed && psystem->done();
}
