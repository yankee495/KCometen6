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

#ifndef COMETEN_MATH_H
#define COMETEN_MATH_H

#include "global.h"
#include "vec.h"
 
const double Pi = 3.141592653589793238512808959406186204433;

// Use the native, high-performance system clamp from modern C++ standard libraries
using std::clamp;


inline double deg2rad(double d) {
	return d * Pi / 180.0;
}

inline double rad2deg(double r) {
	return r * 180.0 / Pi;
}

inline double frand() {
	return (double)(rand() % RAND_MAX) / RAND_MAX;
}

inline double frand(double min, double max) {
	return min + frand() * (max - min);
}

inline int rand(int min, int max) {
	return min + rand() % (max-min+1);
}


class Plane {
public:
	Plane(const Vec3f & _N, double _d) : N(_N), d(_d) {}
	~Plane() {};
	
	Vec3f N;
	double d;
};

bool TestDynamicSpherePlane(const Vec3f & c1, double r1, const Vec3f & W, 
                            const Plane & plane, double & time);


#endif
