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
#ifndef PCOMET_H
#define PCOMET_H

#include "global.h"
#include "cometenmath.h"
#include "comet.h"
#include "explosion.h"
#include "glow.h"

class PComet : public Comet {
public:
	PComet(CometenScene * scene, double time, double lifeTime, double size,
	       const Vec3f& position, const Vec3f& velocity, 
	       double particlePerSec, double particleLifeTime,
	       const Vec4f& acolor, const Vec4f& ecolor);
	virtual ~PComet();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
protected:
	virtual void hitPlane(double time, const Plane & plane);
	
	bool	destroyed;
	Vec3f 	velocity;
	Vec3f 	acceleration;
	double	startTime;
	double	lifeTime;
	Vec4f 	acolor, ecolor;
	
	ParticleSystem *psystem;
	Decal*	decal;
};

class QComet : public PComet {
public:
	QComet(CometenScene * scene, double time, double lifeTime, 
	       const Vec3f & pos, const Vec3f & vel, bool split,
	       const Vec4f & acolor, const Vec4f & ecolor);
	virtual ~QComet();
	
	virtual void destroy(double time);
	
protected:
	bool split;
};

class SmallQComet : public PComet {
public:
	SmallQComet(CometenScene * scene, double time, double lifeTime,
	            const Vec3f & pos, const Vec3f & vel, double size,
	            int split, const Vec4f & acolor, const Vec4f & ecolor);
	virtual ~SmallQComet();
	
	virtual void destroy(double time);
	
protected:
	int split;
};

#endif
