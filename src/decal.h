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
#ifndef DECAL_H
#define DECAL_H

#include "cometenmath.h"
#include "global.h"
#include "vec.h"

class Decal
{
public:	
	// normal must be normalized
	Decal(GLuint texture, const Vec3f & position,  double size,
	      bool billboard, const Vec3f & normal, const Vec4f & color,
	      double transparency = 1.0);
    ~Decal();
	
	void render(const Vec3f & campos);
	
	void setPosition(const Vec3f& position);
	Vec3f getPosition();
	void setSize(double size);
	double getSize();
	void setColor(const Vec4f& Color);
	Vec4f getColor();
	void setTransparency(double transparency);
	double getTransparency();
	
	
private:
	GLuint texture;
	Vec3f position;
	double size;
	bool billboard;
	Vec3f normal;
	Vec4f color;
	double transparency;
};

inline void Decal::setPosition(const Vec3f& pos) {
	position = pos;
}

inline Vec3f Decal::getPosition() {
	return position;
}

inline void Decal::setSize(double s) {
	size = s;
}

inline double Decal::getSize() {
	return size;
}

inline void Decal::setColor(const Vec4f& c) {
	color = c;
}

inline Vec4f Decal::getColor() {
	return color;
}

inline void Decal::setTransparency(double t){
	transparency = t;
}

inline double Decal::getTransparency() {
	return transparency;
}

#endif
