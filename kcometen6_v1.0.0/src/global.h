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

#ifndef GLOBAL_H
#define GLOBAL_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

#define VERSION "1.0.7"
//#define T_PARTICLE "0"
using namespace std;

enum {
	T_PARTICLE,
	T_LIGHTMAP,
	T_BACKGROUND,
	T_NUM,
};

enum {
	C_RED,
	C_BLUE,
	C_GREEN,
	C_BUNT,
	C_EXTREMBUNT
};

#endif
