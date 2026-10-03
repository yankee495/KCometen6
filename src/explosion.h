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
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "global.h"
#include "cometenmath.h"
#include "decal.h"
#include "particlesystem.h"
#include "comet.h"

class Explosion : public Comet
{
public:
	Explosion(CometenScene* scene, double time, double lifeTime,
	          const Vec3f& position, double size, int numParticle,
	          const Vec4f& startColor, const Vec4f& endColor);
    virtual ~Explosion();

	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
private:
	Decal *decal;
	ParticleSystem *psystem;
	
	double startTime;
	double lifeTime;
	bool done;
}; 

#endif
