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
#include "curvecomet.h"
extern GLuint g_textures[];
#include "settings.h"  // THE FIX: Allows the file to read settings->sizeScale!
extern GLuint g_textures[];

CurveComet::CurveComet(CometenScene * scene, double time, double _lifeTime,
					   const Vec3f& start, const Vec4f & _acolor,
					   const Vec4f & _ecolor)
:
// Swap 1.0 out for your global scale modifier!
Comet(scene, settings->sizeScale),
startTime(time),
lifeTime(_lifeTime),
acolor(_acolor),
ecolor(_ecolor)
{
	GLuint tex = g_textures[0];

	psystem = new ParticleSystem(time, tex, start, start, 800, 0, 1.5, 2.0,
								 true, 400, 0, 5, false,
							  4 * settings->sizeScale, acolor, ecolor);

	decal = new Decal(tex, start, 12.0 * settings->sizeScale,
					  true, Vec3f::UNIT_X, acolor);

	createCurve(start);
	numChildren = rand(25, 30);
	destroyed = false;
}

CurveComet::~CurveComet() {
	delete psystem;
	delete decal;
	delete curve;
}

void CurveComet::process(double time, double /*unused*/) {
	position = curve->position((time - startTime) / lifeTime);
	
	if (!destroyed) {
		if (time - startTime > lifeTime) 
			destroy(time);
	}
	
	decal->setPosition(position);
	psystem->setEmitterPos1(psystem->getEmitterPos2());
	psystem->setEmitterPos2(position);
	psystem->process(time);
	
	if (destroyed) {
		psystem->stop();
	}
}

void CurveComet::render(const Vec3f& campos) {
	if (!destroyed) {
		decal->render(campos);
	}
	psystem->render(campos);
}

void CurveComet::createCurve(const Vec3f& start) {
	Vec3f points[15];
	double bw = scene->getBoxWidth() / 2;
	double bh = scene->getBoxHeight() / 2;
	int n = 15;
	
	points[0] = start;
	for (int k = 1; k < n; k++) {
		points[k] = Vec3f(frand(-bw,bw),frand(-bh,bh),frand(-bw,bw));
	}
	
	Vec3f diff = points[n-1] - points[n-2];
	double l = diff.normalize();
	
	if (l > 200) {
		points[n-1] = points[n-2] + diff * 200;
	}
	
	curve = new Bezier3(points, n);
}

void CurveComet::destroyEffect1(double time, const Vec3f& pos, const Vec3f& vel) {
	Comet* newc;
	Vec3f points[4];
	Vec3f dir, dir2, dir3;
	Vec3f b, c;
	double bw = scene->getBoxWidth()/2;
	double bh = scene->getBoxHeight()/2;
	
	dir = vel;
	dir.normalize();
	Vec3f::generateOrthonormalBasis(dir, b, c);
	points[0] = pos;
	for (int i = 0; i < numChildren; i++) {
		dir2 = Vec3f::Vec3f::randomUnitOnPlane(b,c);
		dir3 = dir.cross(dir2);
		
		points[1] = points[0] + dir2 * 50;
		points[1][0] = clamp<float>(points[1][0], -bw, bw);
		points[1][1] = clamp<float>(points[1][1], -bh, bh);
		points[1][2] = clamp<float>(points[1][2], -bw, bw);
		points[2] = points[0] + dir3 * 50;
		points[2][0] = clamp<float>(points[2][0], -bw, bw);
		points[2][1] = clamp<float>(points[2][1], -bh, bh);
		points[2][2] = clamp<float>(points[2][2], -bw, bw);
		points[3] = points[0] + dir3 * frand(0,5);
		points[3][0] = clamp<float>(points[3][0], -bw, bw);
		points[3][1] = clamp<float>(points[3][1], -bh, bh);
		points[3][2] = clamp<float>(points[3][2], -bw, bw);
		
		newc = new SmallCurveComet(scene, time, frand(2.75, 3.25),  
		                           points, 4, acolor, ecolor);
		scene->addComet(newc);
	}
}

void CurveComet::destroyEffect2(double time, const Vec3f& pos, const Vec3f& /*unused*/) {
	Comet* newc;
	Vec3f points[4];
	Vec3f dir, dir2, dir3;
	double bw = scene->getBoxWidth()/2;
	double bh = scene->getBoxHeight()/2;
	
	points[0] = pos;
	for (int i = 0; i < numChildren; i++) {
		dir = Vec3f::Vec3f::randomUnit();
		Vec3f::generateOrthonormalBasis(dir, dir2, dir3);
		points[1] = points[0] + dir * 10 
			+ Vec3f::randomUnitOnPlane(dir2, dir3) * 35;
		points[1][0] = clamp<float>(points[1][0], -bw, bw);
		points[1][1] = clamp<float>(points[1][1], -bh, bh);
		points[1][2] = clamp<float>(points[1][2], -bw, bw);
		points[2] = points[0]+ dir * 20 
			+ Vec3f::randomUnitOnPlane(dir2, dir3) * 35;
		points[2][0] = clamp<float>(points[2][0], -bw, bw);
		points[2][1] = clamp<float>(points[2][1], -bh, bh);
		points[2][2] = clamp<float>(points[2][2], -bw, bw);
		points[3] = points[0]+ dir * 30 
			+ Vec3f::randomUnitOnPlane(dir2, dir3) * 35;
		points[3][0] = clamp<float>(points[3][0], -bw, bw);
		points[3][1] = clamp<float>(points[3][1], -bh, bh);
		points[3][2] = clamp<float>(points[3][2], -bw, bw);
		
		newc = new SmallCurveComet(scene, time, frand(1.0, 1.25),  
		                           points, 4, acolor, ecolor);
		scene->addComet(newc);
	}
}

void CurveComet::destroyEffect3(double time, const Vec3f& pos, const Vec3f& /*unused*/) {
	Comet* newc;
	Vec3f points[4];
	Vec3f dir, dir2, dir3;
	double bw = scene->getBoxWidth()/2;
	double bh = scene->getBoxHeight()/2;
	
	points[0] = pos;
	for (int i = 0; i < numChildren; i++) {
		dir = Vec3f::Vec3f::randomUnit();
		Vec3f::generateOrthonormalBasis(dir, dir2, dir3);
		dir2 = Vec3f::randomUnitOnPlane(dir2, dir3);
		
		points[1] = points[0] + dir * 50;
		points[1][0] = clamp<float>(points[1][0], -bw, bw);
		points[1][1] = clamp<float>(points[1][1], -bh, bh);
		points[1][2] = clamp<float>(points[1][2], -bw, bw);
		points[2] = points[1] + dir2 * 50;
		points[2][0] = clamp<float>(points[2][0], -bw, bw);
		points[2][1] = clamp<float>(points[2][1], -bh, bh);
		points[2][2] = clamp<float>(points[2][2], -bw, bw);
		points[3] = points[0] + dir2 * 2.5;
		points[3][0] = clamp<float>(points[3][0], -bw, bw);
		points[3][1] = clamp<float>(points[3][1], -bh, bh);
		points[3][2] = clamp<float>(points[3][2], -bw, bw);

		newc = new SmallCurveComet(scene, time, frand(1.75, 1.99),  
		                           points, 4, acolor, ecolor);
		scene->addComet(newc);
	}
}

void CurveComet::destroy(double time) {
	Vec3f pos;
	Vec3f vel;
	Comet* newc;
	
	if (destroyed)
		return;
	
	pos = curve->position((time - startTime) / lifeTime);
	vel = curve->firstDerivate((time - startTime) / lifeTime);
	switch (rand() % 3) {
	case 0:
		destroyEffect1(time, pos, vel);
		break;
	case 1:
		destroyEffect2(time, pos, vel);
		break;
	case 2:
	default:
		destroyEffect3(time, pos, vel);
		break;
	}
	
	newc = new Explosion(scene, time, 1.0, pos, 25*size, 300, acolor, ecolor);
	scene->addComet(newc);
	
	psystem->stop();
	destroyed = true;
}


bool CurveComet::isDestroyed() {
	return destroyed;
}

bool CurveComet::isDone() {
	return destroyed && psystem->done();
}


/*
 *  SmallCurveComet
 */

SmallCurveComet::SmallCurveComet(CometenScene * scene, double time, 
	double _lifeTime, const Vec3f *points, int numPoints, 
    const Vec4f & _acolor, const Vec4f & _ecolor)
:
Comet(scene, 0.25),
startTime(time),
lifeTime(_lifeTime),
acolor(_acolor),
ecolor(_ecolor)
{
	GLuint tex = g_textures[0];
	
	assert(points && numPoints >= 2);
	
	position = points[0];
	psystem = new ParticleSystem(time, tex, points[0], points[0], 300, 0, 
	                             1.5, 2.0,  true, 150, 0.0, 2.0, false, 
	                             1.75, acolor, ecolor);
	
	decal = new Decal(tex, points[0], 3.0, 
	                  true, Vec3f::UNIT_X, acolor);
	
	curve = new Bezier3(points, numPoints);
	destroyed = false;
}

SmallCurveComet::~SmallCurveComet() {
	delete psystem;
	delete decal;
	delete curve;
}

void SmallCurveComet::process(double time, double /*unused*/) {
	position = curve->position((time - startTime) / lifeTime);
	
	decal->setPosition(position);
	psystem->setEmitterPos1(psystem->getEmitterPos2());
	psystem->setEmitterPos2(position);
	psystem->process(time);
	
	if (!destroyed) {
		if (time - startTime > lifeTime) 
			destroy(time);
	}
}

void SmallCurveComet::render(const Vec3f& campos) {
	if (!destroyed) {
		decal->render(campos);
	}
	psystem->render(campos);
}

void SmallCurveComet::destroy(double time) {
	Vec3f pos;
	Comet* newc;
	
	if (destroyed)
		return;
	
	pos = curve->position((time - startTime) / lifeTime);
	newc = new Explosion(scene, time, 1.0, pos, 25*size, 150, acolor, ecolor);
	scene->addComet(newc);
	
	psystem->stop();
	destroyed = true;
}


bool SmallCurveComet::isDestroyed() {
	return destroyed;
}

bool SmallCurveComet::isDone() {
	return destroyed && psystem->done();
}

