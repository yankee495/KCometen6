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

#include "bezier.h"

const int maxBinom = 15;
const double binomial[maxBinom+1][maxBinom+1] = {
/* n = 0 */ {1, },
/* n = 1 */ {1, 1, },
/* n = 2 */ {1, 2, 1, },
/* n = 3 */ {1, 3, 3, 1, },
/* n = 4 */ {1, 4, 6, 4, 1, },
/* n = 5 */ {1, 5, 10, 10, 5, 1, },
/* n = 6 */ {1, 6, 15, 20, 15, 6, 1, },
/* n = 7 */ {1, 7, 21, 35, 35, 21, 7, 1, },
/* n = 8 */ {1, 8, 28, 56, 70, 56, 28, 8, 1, },
/* n = 9 */ {1, 9, 36, 84, 126, 126, 84, 36, 9, 1, },
/* n = 10 */ {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1, },
/* n = 11 */ {1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11, 1, },
/* n = 12 */ {1, 12, 66, 220, 495, 792, 924, 792, 495, 220, 66, 12, 1, },
/* n = 13 */ {1, 13, 78, 286, 715, 1287, 1716, 1716, 1287, 715, 286, 78, 
		13, 1, },
/* n = 14 */ {1, 14, 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001,
		364, 91, 14, 1, },
/* n = 15 */ {1, 15, 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 
		1365, 455, 105, 15, 1, },
};


Bezier3::Bezier3(const Vec3f* points, int num) {
	int   i;
	
	assert(num >= 2 && num <= maxBinom+1);
	
	numControlPoints = num;
	degree = numControlPoints-1;
	
	ctrlPoints = new Vec3f[numControlPoints];
	for (i = 0; i < numControlPoints; i++) {
		if (i < num) 
			ctrlPoints[i] = points[i];
		else
			ctrlPoints[i] = Vec3f::ZERO;
	}
	
	firstDerCtrlPoints = new Vec3f[numControlPoints-1];
	for (i = 0; i < numControlPoints-1; i++) {
		firstDerCtrlPoints[i] = ctrlPoints[i+1] - ctrlPoints[i];
	}
	
	if (degree >= 2) {
		secondDerCtrlPoints = new Vec3f[numControlPoints-2];
		for (i = 0; i < numControlPoints-2; i++) {
			secondDerCtrlPoints[i] = firstDerCtrlPoints[i+1] 
				- firstDerCtrlPoints[i];
		}
	}
	else {
		secondDerCtrlPoints = 0;
	}
}

Bezier3::~Bezier3() {
	delete[] ctrlPoints;
	delete[] firstDerCtrlPoints;
	delete[] secondDerCtrlPoints;
}

Vec3f Bezier3::position(double t) const {
	if (t < 0.0)
		return t*firstDerCtrlPoints[0] + ctrlPoints[0];
	if (t > 1.0)
		return (t-1)*firstDerCtrlPoints[degree-1] + ctrlPoints[degree];
	
	double oneMinusT = 1-t;
	double powt = t;
	int i;
	Vec3f result = ctrlPoints[0] * oneMinusT;
	
	for (i = 1; i < degree; i++) {
		result = (result + binomial[degree][i]*powt*ctrlPoints[i])*oneMinusT;	
		powt *= t;
	}
	
	result += ctrlPoints[degree] * powt;
	
	return result;
}

Vec3f Bezier3::firstDerivate(double t) const {
	if (t < 0.0)
		return firstDerCtrlPoints[0];
	if (t > 1.0)
		return firstDerCtrlPoints[degree-1];
	
	double oneMinusT = 1-t;
	double powt = t;
	int i;
	Vec3f result = firstDerCtrlPoints[0] * oneMinusT;
	
	for (i = 1; i < degree-1; i++) {
		result = (result + binomial[degree-1][i]*powt*firstDerCtrlPoints[i])
			* oneMinusT;	
		powt *= t;
	}
	
	result += firstDerCtrlPoints[degree-1]*powt;
	return result;
}

Vec3f Bezier3::secondDerivate(double t) const {
	if (t < 0.0 || t > 1.0 || degree < 2)
		return Vec3f::ZERO;
	
	assert(secondDerCtrlPoints != 0);
	
	double oneMinusT = 1-t;
	double powt = t;
	int i;
	Vec3f result = secondDerCtrlPoints[0] * oneMinusT;
	
	for (i = 1; i < degree-2; i++) {
		result = (result + binomial[degree-2][i]*powt*secondDerCtrlPoints[i])
			* oneMinusT;	
		powt *= t;
	}
	
	result += secondDerCtrlPoints[degree-2]*powt;
	return result;
}






