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

#include "vec.h"
#include "global.h"
#include "cometenmath.h"

Vec3f Vec3f::randomUnit() {
	Vec3f newvec(frand(-1.0,1.0), 
		frand(-1.0,1.0), 
		frand(-1.0,1.0));
	
	float l = newvec.length();
	if (l == 0.0) {
		return Vec3f(1.0, 0.0, 0.0);
	}
	else {
		newvec /= l;	
		return newvec;
	}	
}

void Vec3f::reflect(const Vec3f & normal) {
	float a = dot(normal) * 2;
	
	data[0] = data[0] - normal.data[0] * a; 
	data[1] = data[1] - normal.data[1] * a;
	data[2] = data[2] - normal.data[2] * a;
}

void Vec3f::generateOrthonormalBasis(const Vec3f& a, Vec3f&b, Vec3f& c) {
	if (fabs(a[0]) >= fabs(a[1])) {
		b[0] = a[2];
		b[1] = 0;
		b[2] = -a[0];
	}
	else {
		b[0] = 0;
		b[1] = a[2];
		b[2] = -a[1];
	}
	b.normalize();
	c = a.cross(b);
}

Vec3f Vec3f::randomUnitOnPlane(const Vec3f& b, const Vec3f& c) {
	float f = frand(-1.0, 1.0);
	float g = frand(-1.0, 1.0);
	Vec3f a = b * f + c * g;
	float l = a.length();
	
	if (l == 0.0) {
		return b;
	}
	else {
		a /= l;
		return a;
	}
}

const Vec3f Vec3f::ZERO(0.0, 0.0, 0.0);
const Vec3f Vec3f::UNIT_X(1.0, 0.0, 0.0);
const Vec3f Vec3f::UNIT_Y(0.0, 1.0, 0.0);
const Vec3f Vec3f::UNIT_Z(0.0, 0.0, 1.0);


const Vec4f Vec4f::ZERO(0.0, 0.0, 0.0, 0.0);
const Vec4f Vec4f::UNIT_X(1.0, 0.0, 0.0, 0.0);
const Vec4f Vec4f::UNIT_Y(0.0, 1.0, 0.0, 0.0);
const Vec4f Vec4f::UNIT_Z(0.0, 0.0, 1.0, 0.0);
const Vec4f Vec4f::UNIT_W(0.0, 0.0, 0.0, 1.0);
