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
#include "lightning.h"
extern GLuint g_textures[];

Lightning::Lightning(CometenScene* _scene, double time,
                     const Vec3f & _start, const Vec3f & _end) 
:
Comet(_scene),
startTime(time),
startPoint(_start),
endPoint(_end)
{	
	Vec3f diff = startPoint - endPoint;
	double w = diff.length() / 100;
	
	w = clamp<double>(w, 1.0, 2.0);
	createLightning(startPoint, endPoint, 3, 2, w);
	lifeTime = frand(0.2, 0.3);
	done = false;
	transparency = 0;
	
	texture = g_textures[0];
	
	maxVertices = 12 * max(lines.size(), lines2.size());
	vertices = new LightningVertex[maxVertices];
}

Lightning::~Lightning() {
	delete[] vertices;;
}

void Lightning::process(double time, double /*unused*/) {
	if (time > startTime + lifeTime) {
		done = true;
		transparency = 0.0;
		return;
	}
	
	transparency = sinf((time-startTime) / lifeTime * 2*Pi );
	transparency *= transparency;
	transparency = min<double>(1.0, 2*transparency);
}

int Lightning::buildVertexArray(const Vec3f& campos, double width2, 
                                 const vector<Line>& lines) 
{
	Vec3f dir, up, right;
	LightningVertex * v;
	vector<Line>::const_iterator i;
	
	// line to cam direction
	dir = campos - ((startPoint + endPoint) / 2);
	dir.normalize();
	
	v = vertices;
	
	for (i = lines.begin(); i != lines.end(); i++) {
		const Line & line = *i;
		double width = line.width * width2;
		
		assert(v + 12 <= vertices + maxVertices);
		
		right = line.dir;
		up = dir.cross(right); 
		up.normalize();
		
		v->pos = line.p1 - right * (width/2) - up * (width/2);
		v->uv[0] = 0.0;
		v->uv[1] = 0.0;
		v++;
		
		v->pos = line.p1 - right * (width/2) + up * (width/2);
		v->uv[0] = 0.0;
		v->uv[1] = 1.0;
		v++;
		
		v->pos = line.p1 + up * (width/2);
		v->uv[0] = 0.5;
		v->uv[1] = 1.0;
		v++;
		
		v->pos = line.p1 - up * (width/2);
		v->uv[0] = 0.5;
		v->uv[1] = 0.0;
		v++;
		
		////	
		v->pos = line.p1 - up * (width/2);
		v->uv[0] = 0.49;
		v->uv[1] = 0.0;	
		v++;	
		
		v->pos = line.p1 + up * (width/2);
		v->uv[0] = 0.49;
		v->uv[1] = 1.0;	
		v++;
		
		v->pos = line.p2 + up * (width/2);
		v->uv[0] = 0.51;
		v->uv[1] = 1.0;
		v++;
		
		v->pos = line.p2 - up * (width/2);
		v->uv[0] = 0.51;
		v->uv[1] = 0.0;
		v++;	

		////
		v->pos = line.p2 - up * (width/2);
		v->uv[0] = 0.5;
		v->uv[1] = 0.0;
		v++;	
		
		v->pos = line.p2 + up * (width/2);
		v->uv[0] = 0.5;
		v->uv[1] = 1.0;	
		v++;
		
		v->pos = line.p2 + up * (width/2) + right * (width/2);
		v->uv[0] = 1.0;
		v->uv[1] = 1.0;	
		v++;
		
		v->pos = line.p2 - up * (width/2) + right * (width/2);
		v->uv[0] = 1.0;
		v->uv[1] = 0.0;		
		v++;
	}
	
	return v - vertices;
}

void Lightning::render(const Vec3f & campos) {
	int numVertices;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(LightningVertex), &vertices[0].pos);
	glTexCoordPointer(2, GL_FLOAT, sizeof(LightningVertex), &vertices[0].uv);
	
	numVertices = buildVertexArray(campos, 1.0, lines);
	glColor4f(1.0, 1.0, 1.0, 1.0*transparency);
	glDrawArrays(GL_QUADS, 0, numVertices);
	
	numVertices = buildVertexArray(campos, 4.0, lines);
	glColor4f(0.1, 0.1, 0.4, 0.7*transparency);
	glDrawArrays(GL_QUADS, 0, numVertices);
	
	numVertices = buildVertexArray(campos, 12.0, lines2);
	glColor4f(0.99, 0.99, 0.99, 0.15*transparency);
	glDrawArrays(GL_QUADS, 0, numVertices);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Lightning::createLightning(const Vec3f& start, const Vec3f& end,
                                int detail, int split, double width) 
{
	const int numSteps = 4;
	Vec3f p1, p2, p3;
	Vec3f up, right;
	Line line;
	Vec3f dir = end - start;
	double l = dir.normalize();
	double var = l / (6.5);
	
	Vec3f::generateOrthonormalBasis(dir, up, right);
	
	p2 = start;
	
	for (int i = 1; i <= numSteps; i++) {
		p1 = p2;
		
		p2 = start + i * (l / numSteps) * dir;
		
		if (i < numSteps) {
			p2 += (Vec3f::randomUnitOnPlane(up, right)*frand(var/2, var));
		}
		
		if (detail == 0) {
			line.p1 = p1;
			line.p2 = p2;
			line.dir = p2 - p1;
			line.l = line.dir.normalize();
			line.width = width;
			lines.push_back(line);
		}
		
		if (detail == 2) {
			line.p1 = p1;
			line.p2 = p2;
			line.dir = p2 - p1;
			line.l = line.dir.normalize();
			line.width = width;
			lines2.push_back(line);
		}

		if (detail > 0) {
			createLightning(p1, p2, detail-1, 0, width);
		
			if (i > 1 && split > 0) {
				p3 = p1 + dir * l/numSteps * frand();
				p3 += Vec3f::randomUnitOnPlane(up, right) 
					* frand(l/numSteps / 2, l/numSteps);
				
				createLightning(p1, p3, detail-1, split-1, width/2);
			}
		}
	}
}

void Lightning::destroy(double /*unused*/) {
}

bool Lightning::isDestroyed() {
	return true;
}

bool Lightning::isDone() {
	return done;
}


Blitz::Blitz(CometenScene* scene, double time)
:
Comet(scene)
{
	num = rand(3,6);
	nextBlitz = time;
	interval = 0.8;
	done = false;
}

Blitz::~Blitz() {
}

void Blitz::process(double time, double /*unused*/) {
	if (num < 1) {
		done = true;
	}
	
	if (!done && time >= nextBlitz) {
		list<Comet*>::iterator j;
		Comet * comet = 0;
		Comet* newc;
		
		nextBlitz = time + interval;
		
		for (j = scene->comets.begin(); j != scene->comets.end(); j++) {
			if (!(*j)->isDestroyed() && (*j)->getSize() >= 0.75) {
				comet = *j;
				break;
			}
		}
		
		if (comet) {
			Vec3f start;
			
			num--;
			
			comet->destroy(time);
			
			start[0] = frand(-scene->getBoxHeight()/6, scene->getBoxHeight()/6);
			start[1] = scene->getBoxHeight()/2;
			start[2] = frand(-scene->getBoxHeight()/6, scene->getBoxHeight()/6);
			
			newc = new Lightning(scene, time,
			                     start, 
			                     comet->getPosition());
			scene->addComet(newc);
			
			newc = new Glow(scene, time, 
			                start, 25.0,
			                Vec3f(0,-1,0), Vec4f(1.0, 1.0, 1.0, 1.0));
			scene->addComet(newc);
			newc = new Glow(scene, time, 
			                start, 40.0,
			                Vec3f(0,-1,0), Vec4f(0.0, 0.0, 1.0, 1.0));
			scene->addComet(newc);
		}			
	}
}

void Blitz::render(const Vec3f& /*unused*/) {
}

void Blitz::destroy(double /*unused*/) {
}

bool Blitz::isDestroyed() {
	return true;
}

bool Blitz::isDone() {
	return done;
}


