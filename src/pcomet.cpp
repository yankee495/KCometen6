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
#include "pcomet.h"
extern GLuint g_textures[];

PComet::PComet(CometenScene * scene, double time, double _lifeTime, double size,
               const Vec3f& pos, const Vec3f& vel, 
               double particlePerSec, double particleLifeTime,
               const Vec4f& _acolor, const Vec4f& _ecolor):
Comet(scene, size, pos),
velocity(vel),
startTime(time),
lifeTime(_lifeTime),
acolor(_acolor),
ecolor(_ecolor)
{
	GLuint tex = g_textures[0];
	int numParticle = min<int>((int)(particlePerSec*lifeTime),
	                      	   (int)(particlePerSec*particleLifeTime));
	psystem = new ParticleSystem(time, tex, pos, pos, numParticle, 0, 
	                             max<double>(0.0, particleLifeTime-0.5), 
	                             particleLifeTime,  true, particlePerSec, 
	                             4*size, 8*size, false, 4*size, acolor, ecolor);
	
	decal = new Decal(tex, pos, 10*size, 
	                  true, Vec3f::UNIT_X, acolor);
	destroyed = false;
}

PComet::~PComet() {
	delete psystem;
	delete decal;;
}

void PComet::process(double time, double deltat) {
	int i;
	int hitplane;
	double hittime;
	double t;
	
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
		
		if (TestDynamicSpherePlane(position, size, velocity, 
		                           scene->getPlane(i), t) && 
		    t <= hittime) 
		{
			hitplane = i;
			hittime = t;
		}
	} 
	
	if (!destroyed) {
		position += velocity * hittime;
		
		if (hitplane != -1) {
			hitPlane(time + hittime, scene->getPlane(hitplane));
		}
	}
	
	if (time - startTime > lifeTime) {
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

void PComet::render(const Vec3f& campos) {
	if (!destroyed) {
		decal->render(campos);
	}
	psystem->render(campos);
}

void PComet::hitPlane(double time, const Plane & plane) {	
	double distance;
	const double e = 0.0001;
	Vec3f hitPoint;	
	Comet* glow;
	
	distance = position.dot(plane.N) - plane.d; 	
	
	hitPoint = position - plane.N * (distance - 0.01);
	
	// if comet intersects plane move along normal
	if (distance >= 0 && distance <= size + e) {
		position += (plane.N * (size + e - distance));
	}
	else if (distance < 0 && distance >= -size - e) {
		position += (plane.N * (-size - e + distance));
	}
	
	// bounce comet
	velocity.reflect(plane.N);
	
	double decalSize = 1.0 * fabs(velocity.dot(plane.N)) * size;
	glow = new Glow(scene, time, hitPoint, decalSize, plane.N, acolor);
	scene->addComet(glow);
}

void PComet::destroy(double /*unused*/) {
	destroyed = true;
	psystem->stop();
}

bool PComet::isDestroyed() {
	return destroyed;
}

bool PComet::isDone() {
	return destroyed && psystem->done();
}

QComet::QComet(CometenScene * scene, double time, double lifeTime, 
               const Vec3f & pos, const Vec3f & vel, bool _split,
               const Vec4f & acolor, const Vec4f & ecolor)
:
PComet(scene, time, lifeTime, 1.0, pos, vel, 400, 1.0, acolor, ecolor)
{
	split = _split;
}

QComet::~QComet() {
}

void QComet::destroy(double time) {
	Comet * newc;

	if (destroyed)
		return;

	if (split) {
		for (int i = 0; i < 3; i++) {
			// FIXED: Correctly alignment (scene, time, lifeTime, pos, vel, size, _split, acolor, ecolor)
			newc = new SmallQComet(scene, time, frand(0.2, 0.3),
								   position, Vec3f::randomUnit() * frand(80.0, 120.0),
								   0.65, 3, acolor, ecolor); // 3 = split depth layers allowed
			scene->addComet(newc);
		}

		newc = new Explosion(scene, time, 1.0, position, 25*size,
							 400, acolor, ecolor);
		scene->addComet(newc);
	}
	else {
		int num = rand(20,30);
		for (int i = 0; i < num; i++) {
			// FIXED: Aligned parameters footprint smoothly
			newc = new SmallQComet(scene, time, frand(0.2, 0.75),
								   position, velocity / 2.0 + Vec3f::randomUnit() * frand(40.0, 90.0),
								   frand(0.3, 0.4), 0, acolor, ecolor); // 0 = non-splitting fragments
			scene->addComet(newc);
		}
	}

	newc = new Explosion(scene, time, 1.0, position, 25*size,
						 400, acolor, ecolor);
	scene->addComet(newc);

	psystem->stop();
	destroyed = true;
}

SmallQComet::SmallQComet(CometenScene * scene, double time, double lifeTime, 
                         const Vec3f & pos, const Vec3f & vel, double size,
                         int _split, const Vec4f & acolor, const Vec4f & ecolor)
:
PComet(scene, time, lifeTime, size, pos, vel, 250, 1.0, acolor, ecolor)
{
	split = _split;
}

SmallQComet::~SmallQComet() {
}

void SmallQComet::destroy(double time) {
	Comet * newc;

	if (!destroyed) {
		if (split > 0) {
			for (int i = 0; i < 3; i++) {
				// FIXED: Passes split-1 into slot 7 to tick down recursive explosion iterations safely!
				newc = new SmallQComet(scene, time, frand(0.2, 0.3),
									   position, velocity / 2.0 + Vec3f::randomUnit() * frand(50.0, 60.0),
									   max<double>(0.25, size*0.5), split - 1, acolor, ecolor);
				scene->addComet(newc);
			}
		}
		newc = new Explosion(scene, time, 1.0, position, 25*size,
							 100, acolor, ecolor);
		scene->addComet(newc);
	}

	psystem->stop();
	destroyed = true;
}


