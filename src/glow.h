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
#ifndef GLOW_H
#define GLOW_H

#include "global.h"
#include "cometenmath.h"
#include "comet.h"
#include "decal.h"

class Glow : public Comet {
public:
	Glow(CometenScene* scene, double time, const Vec3f& position, 
	     double size, const Vec3f& normal, const Vec4f& color);
	virtual ~Glow();
	
	virtual void process(double time, double deltat);
	virtual void render(const Vec3f& campos);
	virtual void destroy(double time);
	virtual bool isDestroyed();
	virtual bool isDone();
	
protected:
	void calcSize();
	
	GLuint texture;	
	double startTime;
	double lifeTime;
	Vec3f dir, up, right;
	Vec4f color;
	double transparency;
	bool done;
};

#endif
