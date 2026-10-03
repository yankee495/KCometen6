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
#ifndef LIGHTNING_H
#define LIGHTNING_H

#include "global.h"
#include "cometenmath.h"
#include "comet.h"
#include "glow.h"

class Lightning : public Comet
{
private:
	class Line {
	public:
		Vec3f p1, p2;
		Vec3f dir; // normalize dir, (p2 - p1)
		double l;  // p1 + l*dir = p2
		double width; 
	};
	
	class LightningVertex {
	public:
		Vec3f pos;
		float uv[2];
	};
	
public:
	Lightning(CometenScene* scene, double time,
	          const Vec3f& start, const Vec3f& endPoint);
	~Lightning();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
private:
	double lifeTime;
	double startTime;
	double transparency;
	bool done;
	
	void createLightning(const Vec3f& start, const Vec3f& end, 
	                     int detail, int split, double width);
	int buildVertexArray(const Vec3f& campos, double width2,
	                      const vector<Line>& lines);
	
	Vec3f startPoint;
	Vec3f endPoint;
	
	GLuint texture;
	vector<Line> lines;
	vector<Line> lines2;
	LightningVertex* vertices;
	int maxVertices;
};


class Blitz : public Comet {
public:
	Blitz(CometenScene* scene, double time);
	~Blitz();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
private:
	double startTime;
	double nextBlitz;
	double interval;
	int num;
	bool done;
};



#endif
