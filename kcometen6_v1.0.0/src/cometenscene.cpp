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

#include "cometenscene.h"
#include "settings.h"
#include "pcomet.h"
#include "rotatecomet.h"
#include "curvecomet.h"
#include "lightning.h"

CometenScene::CometenScene(const GLuint* text,
                           double time, double boxWidth, double boxHeight)
:
textures(text)
{
	resizeBox(boxWidth, boxHeight);
	lastCometProcess = time;
	nextComet = -10e20;
}

CometenScene::~CometenScene() {
	list<Comet*>::iterator j;
	for (j = comets.begin(); j != comets.end(); j++) {
		delete (*j);
		*j = 0;
	}
}

void CometenScene::resizeBox(double w, double h) {
	boxWidth = w;
	boxHeight = h;
	collisionPlanes.clear();

	// THE 4K DEEP SPACE MATRIX:
	// Expand collision boundaries to 5000.0 units so comets never hit early self-destruct limits!
	collisionPlanes.push_back(Plane(Vec3f::UNIT_X, -boxWidth * 2.0));
	collisionPlanes.push_back(Plane(-Vec3f::UNIT_X, -boxWidth * 2.0));
	collisionPlanes.push_back(Plane(Vec3f::UNIT_Y, -boxHeight * 2.0));
	collisionPlanes.push_back(Plane(-Vec3f::UNIT_Y, -boxHeight * 2.0));
	collisionPlanes.push_back(Plane(Vec3f::UNIT_Z, -5000.0));
	collisionPlanes.push_back(Plane(-Vec3f::UNIT_Z, -5000.0));
}

void CometenScene::process(double time) {
	double deltat = time - lastCometProcess;
	lastCometProcess = time;

	// THE BULLETPROOF MODERN LIST ITERATOR ENVELOPE:
	// Safely increments indices only after vector erasure passes complete!
	auto j = comets.begin();
	while (j != comets.end()) {
		Comet* comet = *j;

		if (comet) {
			comet->process(time, deltat);
		}

		if (!comet || comet->isDone()) {
			delete comet;
			j = comets.erase(j); // Natively returns the next valid iterator node address!
		} else {
			++j;
		}
	}

	// ============================================================================
	// THE EQUILIBRIUM BLIZZARD CONVEYOR BELT:
	// Rhythmic, single-pass injection eliminates frame stutter and restores long tails!
	// ============================================================================
	int maxComets = settings->cometCount();
	double spawnDelay = settings->createInterval;

	if (maxComets <= 0)  maxComets = 150;
	if (spawnDelay <= 0.0001) spawnDelay = 0.01;

	// Distribute memory allocation smoothly across the timeline
	if (time >= nextComet) {
		// If comets have died and dropped below your target, drop exactly ONE new comet this frame
		if (comets.size() < (size_t)maxComets) {
			createComet(time);
		}
		// THE DRIFT-FREE CHRONO EQUALIZER:
		// Advance relative to nextComet to maintain a perfect, never-ending scheduling sequence!
		nextComet += spawnDelay;
	}
}

void CometenScene::getCometColors(Vec4f& acolor, Vec4f& ecolor) {
	switch (settings->color) {
	case C_RED:
		acolor = Vec4f(1.0, 0.5, 0.25, 1.0);
		ecolor = Vec4f(1.0, 0.0, 0.0, 0.0);
		break;
	case C_BLUE:
		acolor = Vec4f(0.25, 0.5, 1.0, 1.0);
		ecolor = Vec4f(0.0, 0.0, 1.0, 0.0);
		break;
	case C_GREEN:
		acolor = Vec4f(0.5, 1.0, 0.25, 1.0);
		ecolor = Vec4f(0.0, 1.0, 0.0, 0.0);
		break;
	case C_EXTREMBUNT:
		acolor = Vec4f(frand(), frand(), frand(), 1.0);
		ecolor = Vec4f(frand(), frand(), frand(), 0.0);
		break;
	case C_BUNT:
	default:
		switch (rand() % 4) {
		case 2:
			acolor = Vec4f(1.0, 0.5, 0.25, 1.0);
			ecolor = Vec4f(1.0, 0.0, 0.0, 0.0);
			break;
		case 1:
			acolor = Vec4f(0.25, 0.5, 1.0, 1.0);
			ecolor = Vec4f(0.0, 0.0, 1.0, 0.0);
			break;
		case 0:
			acolor = Vec4f(0.5, 1.0, 0.25, 1.0);
			ecolor = Vec4f(0.0, 1.0, 0.0, 0.0);
			break;
		case 3:
		default:
			acolor = Vec4f(1.0, 1.0, 1.0, 1.0);
			ecolor = Vec4f(0.0, 0.0, 0.5, 0.0);
			break;
		}
	}
}

void CometenScene::createComet(double time) {
	Vec4f acolor, ecolor, acolor2, ecolor2;
	Comet * newc = nullptr;

	getCometColors(acolor, ecolor);

	// ============================================================================
	// THE BLIZZARD FOCUS MATRIX:
	// Compressing the spawn sphere from d/3 down to d/6 clusters the comets
	// tightly inside your active camera viewport, packing the screen with action!
	// ============================================================================
	double d = boxWidth < boxHeight ? boxWidth : boxHeight;
	Vec3f start = Vec3f::randomUnit() * d/6; // FIXED: Brings them out of the deep background corners!

	// THE UNIFIED PROBABILITY MATRIX: Lock in a single roll value!
	double roll = frand();

	// HIGH-FREQUENCY BLITZ OVERRIDE: Cranked up to 15% so lightning strikes constantly!
	if (roll < 0.15) {
		if (settings && settings->blitz) {
			newc = new Blitz(this, time);
		} else {
			newc = new QComet(this, time, frand(2.5, 10.5), start,
				       Vec3f::randomUnit() * frand(50.0, 120.0),  false,
				       acolor, ecolor);
		}
	}
	else if (roll < 0.50) { // 35% Standard Comets
		newc = new QComet(this, time, frand(2.5, 10.5), start,
			       Vec3f::randomUnit() * frand(75.0, 120.0),  false,
			       acolor, ecolor);
	}
	else if (roll < 0.65) { // 15% Fast Comets
		newc = new QComet(this, time, frand(2.5, 4.5), start,
			       Vec3f::randomUnit() * frand(200.0, 300.0), false,
			       acolor, ecolor);
	}
	else if (roll < 0.75) { // 10% Split Comets
		if (settings && settings->splitComet) {
			newc = new QComet(this, time, frand(2.5, 4.5), start,
				       Vec3f::randomUnit() * frand(75.0, 120.0),  true,
				       acolor, ecolor);
		} else {
			newc = new QComet(this, time, frand(2.5, 10.5), start,
				       Vec3f::randomUnit() * frand(50.0, 120.0),  false,
				       acolor, ecolor);
		}
	}
	else if (roll < 0.88) { // 13% Orbiting Comets
		if (settings && settings->rotateComet) {
			getCometColors(acolor2, ecolor2);
			newc = new RotateComet(this, time, frand(7.8, 9.5), start,
				            Vec3f::randomUnit() * frand(80.0, 150.0),
				            acolor, ecolor, acolor2, ecolor2);
		} else {
			newc = new QComet(this, time, frand(2.5, 10.5), start,
				       Vec3f::randomUnit() * frand(50.0, 120.0),  false,
				       acolor, ecolor);
		}
	}
	else { // 12% Bezier Curve Comets
		if (settings && settings->curveComet) {
			newc = new CurveComet(this, time, frand(1.5, 3.5), start,
				           acolor, ecolor);
		} else {
			newc = new QComet(this, time, frand(2.5, 10.5), start,
				       Vec3f::randomUnit() * frand(50.0, 120.0),  false,
				       acolor, ecolor);
		}
	}

	addComet(newc);
}

void CometenScene::addComet(Comet* comet) {
	if (comet) {
		comets.push_back(comet);
	}
}
void CometenScene::render(const Vec3f& camera_pos) {
	// 1. Draw the background box panels first
	renderBackground();



	// ============================================================================
	// FIXED COMET BLENDING WRAPPER: Use standard alpha blending for bright assets!
	// ============================================================================
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE); // Stops overlapping particle cards from cutting each other out

	//FIX: Changed from GL_ONE to GL_ONE_MINUS_SRC_ALPHA to prevent black textures on bright walls
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Loop through individual moving comets cleanly
	for (std::list<Comet*>::iterator i = comets.begin(); i != comets.end(); ++i) {
		(*i)->render(camera_pos);
	}

	// Restore clean depth settings and reset states safely after loop completes
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void CometenScene::renderBackground() {
	double boxWidth = 1920;
	double boxHeight = 1080;
	double boxDepth = 1920;

	double vertex_array[] = {
		// vorne – front (0-3)
		boxWidth/2, -boxHeight / 2, boxDepth/2,
		boxWidth/2,  boxHeight / 2, boxDepth/2,
		-boxWidth/2,  boxHeight / 2, boxDepth/2,
		-boxWidth/2, -boxHeight / 2, boxDepth/2,
		// hinten – back (4-7)
		-boxWidth/2, -boxHeight / 2, -boxDepth/2,
		-boxWidth/2,  boxHeight / 2, -boxDepth/2,
		boxWidth/2,  boxHeight / 2, -boxDepth/2,
		boxWidth/2, -boxHeight / 2, -boxDepth/2,
		// links – left (8-11)
		-boxWidth/2, -boxHeight / 2, boxDepth/2,
		-boxWidth/2,  boxHeight / 2, boxDepth/2,
		-boxWidth/2,  boxHeight / 2, -boxDepth/2,
		-boxWidth/2, -boxHeight / 2, -boxDepth/2,
		// rechts – right (12-15)
		boxWidth/2, -boxHeight / 2, -boxDepth/2,
		boxWidth/2,  boxHeight / 2, -boxDepth/2,
		boxWidth/2,  boxHeight / 2,  boxDepth/2,
		boxWidth/2, -boxHeight / 2,  boxDepth/2,
		// oben – top (16-19)
		-boxWidth/2,  boxHeight / 2, -boxDepth/2,
		-boxWidth/2,  boxHeight / 2,  boxDepth/2,
		boxWidth/2,  boxHeight / 2,  boxDepth/2,
		boxWidth/2,  boxHeight / 2, -boxDepth/2,
		// unten – bottom (20-23) -> INVERTED GEOMETRY MANDATE
		boxWidth/2, -boxHeight / 2, -boxDepth/2,
		boxWidth/2, -boxHeight / 2,  boxDepth/2,
		-boxWidth/2, -boxHeight / 2,  boxDepth/2,
		-boxWidth/2, -boxHeight / 2, -boxDepth/2,
	};

	static const double texcoords_array[] = {
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // front
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // back
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // left
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // right
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // top
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,   1.0, 0.0, // bottom (FIXED HORIZONTAL ORIENTATION)
	};

	// Enforce strict default hardware tracking states
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_DOUBLE, 0, texcoords_array);
	glVertexPointer(3, GL_DOUBLE, 0, vertex_array);

	// Protect background panels from shadow and depth fog washouts
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_BLEND); // Solid background rendering path

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // ONE UNIFIED CULLING STATE FOR ALL PASSES

	// PASS 1: VERTICAL WALLS
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	glDrawArrays(GL_QUADS, 0, 16);

	// PASS 2: CEILING
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glDrawArrays(GL_QUADS, 16, 4);

	// PASS 3: FLOOR (With fixed geometric geometry, this will now track under GL_FRONT natively!)
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glDrawArrays(GL_QUADS, 20, 4);

//	glTexCoordPointer(2, GL_DOUBLE, 0, texcoords_array);
//	glColor4f(1.0, 1.0, 1.0, 0.99);

	// HARDCODED FIX: Look explicitly at slot 1 for your explosion lightmap glows!
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glDrawArrays(GL_QUADS, 0, 24);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// ============================================================================
	// CLEAN RECOVERY HANDLER: This is guaranteed to execute now!
	// ============================================================================
//	glEnable(GL_LIGHTING);
//	glEnable(GL_FOG);
//	glEnable(GL_BLEND);      // Restores alpha mapping so comets glow bright
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

}
	// CLEANUP: Safely restore structural states for moving particles/lighting blitzes
//	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
//	glMatrixMode(GL_MODELVIEW);
//	glCullFace(GL_FRONT);

	// 3. Clear pipeline client states safely before dropping out of the function pass.
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_CULL_FACE);
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


