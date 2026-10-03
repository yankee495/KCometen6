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
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "global.h"
#include "cometenmath.h"

class ParticleSystem {
private:
	
	class Particle {
	public:
		Vec3f position;
		Vec3f velocity;
		Vec4f color; 
		double startTime;
		double lifeTime;
		double invLifeTime;
		bool active;
	};
	
	// Particles are created on the line between emitterPos1 and emitterPos2
	Vec3f emitterPos1;
	Vec3f emitterPos2;
	double minVelocity, maxVelocity;
	bool slowdown;
	double particleSize;
	double minLifeTime, maxLifeTime;
	bool createNewParticle;
	double newParticlePerSec;
	double numNewParticle;
	double lastUpdateTime;
	
	Vec4f startColor;
	Vec4f endColor;
	
	Particle* particleArr;
	int* indexArr;
	int maxParticle;
	int numActiveParticle;
	GLuint texture;
	static GLfloat *textureArr;

	Vec4f* colorArr;
	Vec3f* vertArr;
	Vec3f campos;
	Vec3f quadLowerLeft, quadLowerRight;

	void renderPoint();
	void renderQuad();
	
public:
	ParticleSystem(double time, GLuint texture,
	               const Vec3f& emitterPos1,
	               const Vec3f& emitterPos2,
	               int maxParticle,
	               int numCreateAtStart,
	               double minLifeTime, 
	               double maxLifeTime, 
				   bool createNewParticle, 
	               double newParticlePerSec,
	               double minVelocity,
	               double maxVelocity,
	               bool slowdown,
	               double particleSize, 
	               const Vec4f& startColor, 
	               const Vec4f& endColor);
	
	~ParticleSystem();
	
	static void setupTextureCoordArray();
	void process(double time);
	void render(const Vec3f & campos);
	void stop();
	bool done();
	
	void setEmitterPos1(const Vec3f& pos);
	Vec3f getEmitterPos1() const;
	void setEmitterPos2(const Vec3f& pos);
	Vec3f getEmitterPos2() const;
	
	void setStartColor(const Vec4f& c);
	Vec4f getStartColor() const;
	void setEndColor(const Vec4f& c);
	Vec4f getEndColor() const;
	
};

inline void ParticleSystem::setEmitterPos1(const Vec3f& p) {
	emitterPos1 = p;
}

inline Vec3f ParticleSystem::getEmitterPos1() const {
	return emitterPos1;
}

inline void ParticleSystem::setEmitterPos2(const Vec3f& p) {
	emitterPos2 = p;
}

inline Vec3f ParticleSystem::getEmitterPos2() const {
	return emitterPos2;
}

inline void ParticleSystem::setStartColor(const Vec4f& c) {
	startColor = c;
}

inline Vec4f ParticleSystem::getStartColor() const {
	return startColor;
}

inline void ParticleSystem::setEndColor(const Vec4f& c) {
	endColor = c;
}

inline Vec4f ParticleSystem::getEndColor() const { 
	return endColor;
}


#endif
