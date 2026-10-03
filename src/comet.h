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

#ifndef COMET_H
#define COMET_H

#include "global.h"
#include "vec.h"
#include "cometenscene.h"

class CometenScene;
   
class Comet {
public:	
	Comet(CometenScene * _scene,
	      double _size = 0.0,
	      const Vec3f& _position = Vec3f::ZERO):
	scene(_scene),
	size(_size),
	position(_position) {};
		
	virtual ~Comet() {};
	
	virtual void process(double time, double deltat) = 0;
	virtual void render(const Vec3f& campos) = 0;
	virtual void destroy(double time) = 0;
	virtual bool isDestroyed() = 0;
	virtual bool isDone() = 0;
	
	double getSize() { return size; }
	const Vec3f& getPosition() { return position; }

protected:
	CometenScene* scene;
	double size;
	Vec3f position;
};	

#endif
