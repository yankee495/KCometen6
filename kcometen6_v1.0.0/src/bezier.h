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

#ifndef BEZIER_H
#define BEZIER_H

#include "global.h"
#include "vec.h"

class Vec3f;

class Bezier3 {
public:
	Bezier3(const Vec3f* ctrlPoints, int num);
	~Bezier3();
	
	Vec3f position(double t) const;
	Vec3f firstDerivate(double t) const;
	Vec3f secondDerivate(double t) const;
	const Vec3f* getPoints() const { return ctrlPoints; }
	int getPointCount() { return numControlPoints; }
	
private:
	Vec3f *ctrlPoints;
	Vec3f *firstDerCtrlPoints;
	Vec3f *secondDerCtrlPoints;
	int numControlPoints;
	int degree; 
};


	

#endif
