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
#ifndef CURVECOMET_H
#define CURVECOMET_H

#include "global.h"
#include "cometenmath.h"
#include "bezier.h"
#include "comet.h"
#include "explosion.h"

class CurveComet : public Comet {
public:
	CurveComet(CometenScene * scene, double time, double lifeTime, 
	           const Vec3f& start, const Vec4f & acolor, const Vec4f & ecolor);
	
	virtual ~CurveComet();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
protected:
	void createCurve(const Vec3f& start);
	void destroyEffect1(double time, const Vec3f& pos, const Vec3f& vel);
	void destroyEffect2(double time, const Vec3f& pos, const Vec3f& vel);
	void destroyEffect3(double time, const Vec3f& pos, const Vec3f& vel);
	
	bool	destroyed;
	double	startTime;
	double	lifeTime;
	int		numChildren;
	Vec4f 	acolor, ecolor;
	Bezier3* curve;	
	
	ParticleSystem *psystem;
	Decal*	decal;
};

class SmallCurveComet : public Comet {
public:
	SmallCurveComet(CometenScene * scene, double time, double lifeTime, 
	           const Vec3f *points, int numPoints, 
	           const Vec4f & acolor, const Vec4f & ecolor);
	
	virtual ~SmallCurveComet();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
protected:
	bool	destroyed;
	double	startTime;
	double	lifeTime;
	Vec4f 	acolor, ecolor;
	Bezier3* curve;	
	
	ParticleSystem *psystem;
	Decal*	decal;
};


#endif
