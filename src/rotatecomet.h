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
#ifndef ROTATECOMET_H
#define ROTATECOMET_H

#include "global.h"
#include "cometenmath.h"
#include "comet.h"
#include "explosion.h"
#include "glow.h"

class RotateComet : public Comet {
public:
	RotateComet(CometenScene * scene, double time, double lifeTime, 
	            const Vec3f & pos, const Vec3f & vel,
	            const Vec4f & acolor1, const Vec4f & ecolor1,
	            const Vec4f & acolor2, const Vec4f & ecolor2);
	virtual ~RotateComet();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
private:
	virtual void hitPlane(double time, const Plane & plane);
	
	bool	destroyed;
	double 	vel;
	Vec3f 	dir;
	Vec3f	up;
	Vec3f	right; 
	double	radius;
	Vec3f	pos1;
	Vec3f	pos2;
	double	startTime;
	double	lifeTime;
	Vec4f 	acolor1, ecolor1;
	Vec4f 	acolor2, ecolor2;
	
	ParticleSystem *psystem1;
	Decal*	decal1;
	ParticleSystem *psystem2;
	Decal*	decal2;
};

class SpComet : public Comet {
public:
	SpComet(CometenScene * scene, double time, const Vec3f & pos, 
	        const Vec3f & normal, double maxRadius,
	        const Vec4f & acolor, const Vec4f & ecolor);
	virtual ~SpComet();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
private:
	Vec3f 	startPos;
	Vec3f 	dir;
	Vec3f	up;
	Vec3f	right; 
	double	maxRadius;
	double	startTime;
	double	lifeTime;
	Vec4f 	acolor, ecolor;
	bool	destroyed;
	
	ParticleSystem *psystem;
	Decal*	decal;
};

#endif
