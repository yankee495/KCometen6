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

#ifndef VEC_H
#define VEC_H

#include "global.h"

class Vec3f {
public:
	Vec3f();
	Vec3f(float x, float y, float z);
	Vec3f(const Vec3f & b);
	~Vec3f();
	
	float operator[](int i) const;
	float& operator[](int i);
	operator const float*();
	operator float*();
	
	bool operator==(const Vec3f& b) const;
	bool operator!=(const Vec3f& b) const;
	
	void operator+=(const Vec3f& b);
	void operator-=(const Vec3f& b);
	void operator*=(float b);
	void operator/=(float b);
	
	Vec3f operator+(const Vec3f& b) const;
	Vec3f operator-(const Vec3f& b) const;
	Vec3f operator-() const;
	Vec3f operator*(float b) const;
	Vec3f operator/(float b) const;
	
	float dot(const Vec3f &b) const;
	float length() const;
	float squareLength() const;
	float normalize();
	
	Vec3f cross(const Vec3f& b) const;
	// random unit length vector
	static Vec3f randomUnit();
	// reflect vector on plane with normal
	// normal must be normalized
	void reflect(const Vec3f& normal);

	// a must be normalized, b,c out
	static void generateOrthonormalBasis(const Vec3f& a, Vec3f& b, Vec3f& c);
	// b,c must be orthonormal vectors on the plane
	static Vec3f randomUnitOnPlane(const Vec3f& b, const Vec3f& c);
	
	static const Vec3f ZERO;
	static const Vec3f UNIT_X;
	static const Vec3f UNIT_Y;
	static const Vec3f UNIT_Z;
private:
	float data[3];	
};

inline Vec3f operator*(float a, const Vec3f& b) {
	return b * a;
}

class Vec4f {
public:
	Vec4f();
	Vec4f(float x, float y, float z, float w);
	Vec4f(const Vec4f & b);
	~Vec4f();
	
	float operator[](int i) const;
	float& operator[](int i);
	operator const float*();
	operator float*();
	
	bool operator==(const Vec4f & b) const;
	bool operator!=(const Vec4f & b) const;
	
	void operator+=(const Vec4f & b);
	void operator-=(const Vec4f & b);
	void operator*=(float b);
	void operator/=(float b);
	
	Vec4f operator+(const Vec4f & b) const;
	Vec4f operator-(const Vec4f & b) const;
	Vec4f operator-() const;
	Vec4f operator*(float b) const;
	Vec4f operator/(float b) const;
	
	float dot(const Vec4f &b) const;
	float length() const;
	float squareLength() const;
	float normalize();
	
	static const Vec4f ZERO;
	static const Vec4f UNIT_X;
	static const Vec4f UNIT_Y;
	static const Vec4f UNIT_Z;
	static const Vec4f UNIT_W;
private:
	float data[4];	
};

/*
 * Vec3f
 */ 

inline Vec3f::Vec3f() {}

inline Vec3f::Vec3f(float x, float y, float z) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

inline Vec3f::Vec3f(const Vec3f& b) {
	data[0] = b.data[0];
	data[1] = b.data[1];
	data[2] = b.data[2];
}

inline Vec3f::~Vec3f() {}

inline float Vec3f::operator[](int i) const {
	assert(i >= 0 && i < 3);
	return data[i];
}

inline float& Vec3f::operator[](int i) {
	assert(i >= 0 && i < 3);
	return data[i];
}

inline Vec3f::operator const float*() {
	return data;
}

inline Vec3f::operator float*() {
	return data;
}

inline bool Vec3f::operator==(const Vec3f & b) const {
	return (data[0] == b.data[0]) && (data[1] == b.data[1]) 
		&& (data[2] == b.data[2]);
}

inline bool Vec3f::operator!=(const Vec3f & b) const {
	return (data[0] != b.data[0]) || (data[1] != b.data[1]) 
		|| (data[2] != b.data[2]);
}

inline void Vec3f::operator+=(const Vec3f & b) {
	data[0] += b.data[0];
	data[1] += b.data[1];
	data[2] += b.data[2];
}

inline void Vec3f::operator-=(const Vec3f & b) {
	data[0] -= b.data[0];
	data[1] -= b.data[1];
	data[2] -= b.data[2];
}

inline void Vec3f::operator*=(float b) {
	data[0] *= b;
	data[1] *= b;
	data[2] *= b;
}

inline void Vec3f::operator/=(float b) {
	float inv = 1 / b;
	data[0] *= inv;
	data[1] *= inv;
	data[2] *= inv;
}

inline Vec3f Vec3f::operator+(const Vec3f& b) const {
	return Vec3f(data[0] + b.data[0], data[1] + b.data[1], data[2] + b.data[2]);
}

inline Vec3f Vec3f::operator-(const Vec3f& b) const {
	return Vec3f(data[0] - b.data[0], data[1] - b.data[1], data[2] - b.data[2]);
}

inline Vec3f Vec3f::operator-() const {
	return Vec3f(-data[0], -data[1], -data[2]);
}

inline Vec3f Vec3f::operator*(float b) const {
	return Vec3f(data[0]*b, data[1]*b, data[2]*b);
}

inline Vec3f Vec3f::operator/(float b) const {
	float inv = 1 / b;
	return Vec3f(data[0]*inv, data[1]*inv, data[2]*inv);
}

inline float Vec3f::dot(const Vec3f &b) const {
	return data[0]*b.data[0] + data[1]*b.data[1] + data[2]*b.data[2];
}

inline Vec3f Vec3f::cross(const Vec3f &b) const {
	return Vec3f(data[1]*b.data[2] - data[2]*b.data[1], 
	            data[2]*b.data[0] - data[0]*b.data[2], 
	            data[0]*b.data[1] - data[1]*b.data[0]);
}

inline float Vec3f::length() const {
	return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
}

inline float Vec3f::squareLength() const {
	return data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
}

inline float Vec3f::normalize() {
	float l = length();
	if (l > 0.0) {
		float inv = 1 / l;
		data[0] *= inv;
		data[1] *= inv;
		data[2] *= inv;
	}
	else {
		data[0] = 1.0;
		data[1] = 0.0;
		data[2] = 0.0;
	}
	return l;
}

/*
 * Vec4f
 */ 

inline Vec4f::Vec4f() {}

inline Vec4f::Vec4f(float x, float y, float z, float w) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = w;
}

inline Vec4f::Vec4f(const Vec4f& b) {
	data[0] = b.data[0];
	data[1] = b.data[1];
	data[2] = b.data[2];
	data[3] = b.data[3];
}

inline Vec4f::~Vec4f() {}

inline float Vec4f::operator[](int i) const {
	assert(i >= 0 && i < 4);
	return data[i];
}

inline float& Vec4f::operator[](int i) {
	assert(i >= 0 && i < 4);
	return data[i];
}

inline bool Vec4f::operator==(const Vec4f & b) const {
	return (data[0] == b.data[0]) && (data[1] == b.data[1]) && 
		(data[2] == b.data[2]) && (data[3] == b.data[3]);
}

inline bool Vec4f::operator!=(const Vec4f & b) const {
	return (data[0] != b.data[0]) || (data[1] != b.data[1]) || 
		(data[2] != b.data[2]) || (data[3] != b.data[3]);
}

inline void Vec4f::operator+=(const Vec4f & b) {
	data[0] += b.data[0];
	data[1] += b.data[1];
	data[2] += b.data[2];
	data[3] += b.data[3];
}

inline void Vec4f::operator-=(const Vec4f & b) {
	data[0] -= b.data[0];
	data[1] -= b.data[1];
	data[2] -= b.data[2];
	data[3] -= b.data[3];
}

inline void Vec4f::operator*=(float b) {
	data[0] *= b;
	data[1] *= b;
	data[2] *= b;
	data[3] *= b;
}

inline void Vec4f::operator/=(float b) {
	float inv = 1 / b;
	data[0] *= inv;
	data[1] *= inv;
	data[2] *= inv;
	data[3] *= inv;
}

inline Vec4f Vec4f::operator+(const Vec4f & b) const {
	return Vec4f(data[0] + b.data[0], data[1] + b.data[1], 
	            data[2] + b.data[2], data[3] + b.data[3]);
}

inline Vec4f Vec4f::operator-(const Vec4f & b) const {
	return Vec4f(data[0] - b.data[0], data[1] - b.data[1], 
	            data[2] - b.data[2], data[3] - b.data[3]);
}

inline Vec4f Vec4f::operator-() const {
	return Vec4f(-data[0], -data[1], -data[2], -data[3]);
}

inline Vec4f Vec4f::operator*(float b) const {
	return Vec4f(data[0]*b, data[1]*b, data[2]*b, data[3]*b);
}

inline Vec4f Vec4f::operator/(float b) const {
	float inv = 1 / b;
	return Vec4f(data[0]*inv, data[1]*inv, data[2]*inv, data[3]*inv);
}

inline float Vec4f::dot(const Vec4f &b) const {
	return data[0]*b.data[0] + data[1]*b.data[1] 
		+ data[2]*b.data[2] + data[3]*b.data[3];
}

inline float Vec4f::length() const {
	return sqrtf(data[0]*data[0] + data[1]*data[1]
	             + data[2]*data[2] + data[3]*data[3]);
}

inline float Vec4f::squareLength() const {
	return data[0]*data[0] + data[1]*data[1]
		+ data[2]*data[2] + data[3]*data[3];
}

inline float Vec4f::normalize() {
	float l = length();
	if (l > 0.0) {
		float inv = 1 / l;
		data[0] *= inv;
		data[1] *= inv;
		data[2] *= inv;
		data[3] *= inv;
	}
	else {
		data[0] = 1.0;
		data[1] = 0.0;
		data[2] = 0.0;
		data[3] = 0.0;
	}
	return l;
}

inline Vec4f operator*(float a, const Vec4f& b) {
	return b * a;
}

#endif
