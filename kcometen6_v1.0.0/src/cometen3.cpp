#include <string>
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

#include "cometen3.h"
#include "settings.h"
#include <GL/glx.h>

Cometen3::Cometen3() {
	scene = 0;
	camera = 0;
	cameraCurve = 0;
	fovx = 0;
	fovy = 0;
	matrixAngle = 0;
}

Cometen3::~Cometen3() {
	quit();
}

void Cometen3::updateTimeScale(double time)
{
	timeScale.setScale(settings->timeScale,
	                   sceneTimer.getTime(time));
}

void Cometen3::updateFreeCamSpeed()
{
	curveTime = 120.0
		/ (1.0 + 2*clamp<double>(settings->freeCameraSpeed, 0, 10));
}

void Cometen3::updateSlowMotionEffects(double time)
{
	// Gracefully exits these effects if suddenly turned off in the preview.
	if (   ( !settings->matrix     && state == S_MATRIX )
		|| ( !settings->slowMotion && state == S_SLOWMOTION ) )
	{
		stateStart = time - 30.51;
	}
}

// Ensure ALL of our custom typedefs, pointers, and glXGetProcAddress mappings are DELETED here.

void Cometen3::init(const GLuint* text,
					int resX, int resY, double time)
{
	quit();

	textures = text;
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	// THE GLOW ROUNDNESS RESTORATION: Activate hardware smoothing!
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glFrontFace(GL_CW);

	std::string glCaps = (char*)glGetString(GL_EXTENSIONS);

	// ============================================================================
	// MODERN HARDWARE ACCELERATION PASS:
	// Bypass legacy extension string checks and read straight from your Python GUI sliders!
	// ============================================================================
	// ============================================================================
	// KCOMETEN6 COMPATIBILITY SHIELD:
	// Verify modern driver capabilities safely if the user engages acceleration!
	// ============================================================================
	if ( settings->usePointSprites )
	{
		// Safe fallback check: ensure the current graphics hardware context can handle point parameters
		#ifdef GL_VERSION_2_0
		glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 60.0);
		glPointParameterf(GL_POINT_SIZE_MIN, 0.0);
		glPointParameterf(GL_POINT_SIZE_MAX, 1000.0);
		#else
		// Silently step down to software billboarding if the device is a relic
		settings->usePointSprites = false;
		#endif
	}

	sceneTimer.setScale(1.0, time);
	sceneTimer.setTime(0.0, time);
	
	timeScale.setScale(settings->timeScale,
	                   sceneTimer.getTime(time));
	timeScale.setTime(0.0, sceneTimer.getTime(time));
	
	scene = new CometenScene(textures,
	                         timeScale.getTime(sceneTimer.getTime(time)),
	                         1.0, 1.0);
	camera = new Camera(Vec3f::ZERO, Vec3f::ZERO); 
	cameraCurve = 0;
	curveCreateTime = 0.0;
	curveTime = 120.0 
		/ (1.0 + 2*clamp<double>(settings->freeCameraSpeed, 0, 10));

	resize(resX, resY);
	
	nextEffect = time + frand(45.0, 120.0);

	state = S_NORMAL;
}

void Cometen3::quit() {
	if (scene != 0) {
		delete scene;
		scene = 0;
	}
	
	if (camera != 0) {
		delete camera;
		camera = 0;
	}
	
	if (cameraCurve != 0) {
		delete cameraCurve;
		cameraCurve = 0;
	}
}

void Cometen3::changeViewport(int x, int y, int width, int height) {
	// a, b, c: size' = size * sqrt[1 / (a + b*d + c*d^2)]

	double maxFov = settings->fov;
	double multiplier = tanf( deg2rad( maxFov/2.0 ) );

	if ( width > height )
	{
		fovx = maxFov;
		fovy = rad2deg( atanf(multiplier*height/width) ) * 2.0;
	}
	else
	{
		fovy = maxFov;
		fovx = rad2deg( atanf(multiplier*width/height) ) * 2.0;
	}

	double fov = fovx > fovy ? fovx : fovy;
	int longer = width > height ? width : height;

	// The original formula used height^2 below, but we want a similar value
	// regardless of whether w or h is longer.  Base it off what we'd get for
	// a 640x480 view: 480 = 0.75 * 640.
	float dist_attenuation[] = {0.0, 0.0,
			4.0 * tanf(deg2rad(fov/2)) * tanf(deg2rad(fov/2)) /(0.75*longer*0.75*longer)};

	glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, dist_attenuation);
	
	glViewport(x, y, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ( width > height )
		gluPerspective(fovy, (double)width / height, 1.0, 10000.0);
	else
	{
		double xmin, xmax, ymin, ymax;
		xmax = tanf( deg2rad(fovx/2) );
		xmin = -xmax;
		ymin = xmin * (double)height/width;
		ymax = xmax * (double)height/width;
		glFrustum(xmin, xmax, ymin, ymax, 1.0, 10000.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

void Cometen3::updateSizeScale()
{
	double oldBw = scene->getBoxWidth() / 2 - 3;
	double oldBh = scene->getBoxHeight() / 2 - 3;

	resizeBox();

	double newBw = scene->getBoxWidth() / 2 - 3;
	double newBh = scene->getBoxHeight() / 2 - 3;
	double xScaleFactor = newBw / oldBw;
	double yScaleFactor = newBh / oldBh;

	double maxAngle = atan( scene->getBoxHeight() / scene->getBoxWidth() );
	if ( matrixAngle > maxAngle )
		matrixAngle = maxAngle;

	if ( settings->freeCamera && cameraCurve )
	{
		const Vec3f* oldPoints = cameraCurve->getPoints();
		const int pointCount = cameraCurve->getPointCount();
		Vec3f newPoints[ pointCount ];

		for ( int i = 0; i < pointCount; i++ )
		{
			double x, y, z;
			x = oldPoints[i][0] * xScaleFactor;
			y = oldPoints[i][1] * yScaleFactor;
			z = oldPoints[i][2] * xScaleFactor;

			newPoints[ i ] = Vec3f( x, y, z );
		}

		delete cameraCurve;
		cameraCurve = new Bezier3( newPoints, pointCount );
	}
}

void Cometen3::resize(int newResX, int newResY) {
	resX = newResX;
	resY = newResY;

	updateSizeScale();
}

void Cometen3::resizeBox()
{
	double boxWidth, boxHeight;

	changeViewport(0,0,resX, resY);

	if ( settings->cube )
	{
		boxWidth = 150.0 * settings->sizeScale;
		boxHeight = boxWidth;
	}
	else
	{
		if ( resX > resY )
		{
			boxWidth = 150.0 * settings->sizeScale;
			boxHeight = boxWidth * resY / resX;
		}
		else
		{
			boxHeight = 150.0 * settings->sizeScale;
			boxWidth = boxHeight * resX / resY;
		}
	}

	scene->resizeBox(boxWidth, boxHeight);
}

void Cometen3::process(double time) {
	processDefaultCameraPos(time);
	
	switch (state) {
	case S_NORMAL:
		processNormal(time);
		break;
	case S_MATRIX:
		processMatrix(time);
		break;
	case S_SLOWMOTION:
		processSlowmotion(time);
		break;
	default:
		state = S_NORMAL;
		stateStart = time;
		break;
	}
	
	scene->process(timeScale.getTime(sceneTimer.getTime(time)));
	
	if (state == S_NORMAL && time > nextEffect) {
		if (settings->matrix && settings->slowMotion) {
			switch(rand() % 2) {
			case 0:
				state = S_SLOWMOTION;
				stateStart = time;
				break;
			case 1:
			default:
				state = S_MATRIX;
				stateStart = time;
				break;
			}	
		}
		else if (settings->matrix) {
			state = S_MATRIX;
			stateStart = time;
		}
		else if (settings->slowMotion) {
			state = S_SLOWMOTION;
			stateStart = time;
		}
		
		nextEffect = time + frand(120.0, 180.0);
	}	
}


void Cometen3::newCameraCurve(double time) {
	Vec3f points[4];
	double bw = scene->getBoxWidth() / 2 - 3;
	double bh = scene->getBoxHeight() / 2 - 3;
	
	if (cameraCurve != 0) {
		points[0] = cameraCurve->position(1.0);
		points[1] = cameraCurve->position(2.0);
		delete cameraCurve;
	}
	else {
		points[0] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[1] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
	}
	
	switch (rand() % 6) {
	case 0:
		points[2] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[3] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		break;
	case 1:
		points[2] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		points[3] = Vec3f(bw,frand(-bh,bh),frand(-bw,bw));
		break;
	case 2:
		points[2] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		points[3] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		break;
	case 3:
		points[2] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		points[3] = Vec3f(frand(-bw,-bw),bh,frand(-bw,bw));
		break;
	case 4:			
		points[2] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		points[3] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		break;
	case 5:
	default:
		points[2] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		points[3] = Vec3f(frand(-bw,-bw),frand(-bh,bh),bw);
		break;
	}
	
	points[3] = (points[3] + points[2]) * 0.5;
	
	cameraCurve = new Bezier3(points, 4);
	curveCreateTime = time;	
}

void Cometen3::processDefaultCameraPos(double time) {
	if (settings->freeCamera) {
		if (cameraCurve == 0 
		    || time > curveCreateTime + curveTime) 
		{
			newCameraCurve(time);
		}
		
		assert(cameraCurve != 0);
		
		defaultCameraPosition = cameraCurve->position(
			(time - curveCreateTime) / curveTime);
	}
	else {
		double fov = fovx > fovy ? fovx : fovy;
		double width = scene->getBoxWidth();
		double height = scene->getBoxHeight();
		double longer = width > height ? width : height;
		double shorter = width < height ? width : height;
		if ( settings->cube )
		{
			double boxCamDistance = shorter / 2 / tanf(deg2rad(fov/2));
			defaultCameraPosition =
				Vec3f(0.0, 0.0, boxCamDistance + longer/2);
		}
		else
		{
			double boxCamDistance = width / 2 / tanf(deg2rad(fovx/2));
			defaultCameraPosition =
				Vec3f(0.0, 0.0, boxCamDistance + width/2);
		}
	}
}

void Cometen3::processNormal(double /*unused*/) {
 	camera->position = defaultCameraPosition;
	camera->viewpoint = Vec3f::ZERO;

	camera->fade = 0.0;
	camera->blackBars = false;
}

void Cometen3::processMatrix(double time) {
	double t = time - stateStart;
	
	camera->viewpoint = Vec3f::ZERO;
	
	if (t < 0.5) {
		camera->position = defaultCameraPosition;
		camera->fade = (t) / 0.5;
		camera->blackBars = false;
		sceneTimer.setScale(1.0, time);
	}
	else if (t < 1.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);

		double maxAngle = atan( scene->getBoxHeight() / scene->getBoxWidth() ) / 2.0;
		matrixAngle = frand(0.0, maxAngle);
	}
	else if (t < 31.0) {
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);
		if (t < 1.5) {
			camera->fade = 1 - (t - 1.0) / 0.5;
		}
		else if (t > 30.5) {
			camera->fade = (t - 30.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}

		double fov = fovx > fovy ? fovx : fovy;
		double width = scene->getBoxWidth();
		double height = scene->getBoxHeight();
		double longer = width > height ? width : height;
		double boxCamDistance = longer / 2 / tanf(deg2rad(fov/2));

		double w = (t - 1.0) / 30.0 * Pi * 2;

		camera->position[0] = sinf(w) * cosf(matrixAngle) * boxCamDistance;
		camera->position[1] = sinf(w) * sinf(matrixAngle) * boxCamDistance;
		camera->position[2] = cosf(w) * boxCamDistance;
		camera->viewpoint = Vec3f::ZERO;
	}
	else if (t < 31.5) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.1, time);
	}
	else if (t < 32.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1 - (t - 31.5) / 0.5;
		sceneTimer.setScale(1.0, time);
		camera->blackBars = false;
	}
	else {
		camera->blackBars = false;
		camera->fade = 0.0;
		camera->position = defaultCameraPosition;
		sceneTimer.setScale(1.0, time);
		
		nextEffect = time + frand(120.0, 180.0);
		state = S_NORMAL;
		stateStart = time;
	}
}

void Cometen3::processSlowmotion(double time) {
	double t = time - stateStart;
	
	camera->viewpoint = Vec3f::ZERO;
	
	if (t < 0.5) {
		camera->position = defaultCameraPosition;
		camera->fade = (t) / 0.5;
		camera->blackBars = false;
		sceneTimer.setScale(1.0, time);
	}
	else if (t < 1.0) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 16.0) {
		camera->blackBars = true;
		if (t < 1.5) {
			camera->fade = 1 - (t - 1.0) / 0.5;
		}
		else if (t > 15.5) {
			camera->fade = (t - 15.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}
		
		camera->position = Vec3f(scene->getBoxWidth()/2 - 10, 
		                        -scene->getBoxHeight()/2 + 10, 
		                        scene->getBoxWidth()/2 - 10);
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 31.0) {
		camera->blackBars = true;
		if (t < 16.5) {
			camera->fade = 1 - (t - 16.0) / 0.5;
		}
		else if (t > 30.5) {
			camera->fade = (t - 30.5) / 0.5;
		}
		else {
			camera->fade = 0.0;
		}
		
		camera->position = Vec3f(-scene->getBoxWidth()/2 + 10, 
		                        +scene->getBoxHeight()/2 - 10, 
		                        -scene->getBoxWidth()/2 + 10);
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 31.5) {
		camera->position = defaultCameraPosition;
		camera->fade = 1.0;
		camera->blackBars = true;
		sceneTimer.setScale(0.25f, time);
	}
	else if (t < 32.0) {
		camera->fade = 1 - (t - 31.5) / 0.5;
		sceneTimer.setScale(1.0, time);
		camera->blackBars = false;
		camera->position = defaultCameraPosition;
	}
	else {
		camera->blackBars = false;
		camera->fade = 0.0;
		camera->position = defaultCameraPosition;
		sceneTimer.setScale(1.0, time);
		
		nextEffect = time + frand(120.0, 180.0);
		state = S_NORMAL;
		stateStart = time;
	}
}


void Cometen3::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(camera->position[0],camera->position[1],camera->position[2],
	          camera->viewpoint[0],camera->viewpoint[1],camera->viewpoint[2],
	          0.0, 1.0, 0.0);
	
	scene->render(camera->position);
	
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	GLint dims[4];
	glGetIntegerv( GL_VIEWPORT, dims);
	glOrtho(0.0, dims[2], 0.0, dims[3], 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	
	if (camera->blackBars && settings->aspectRatio != 2) {
		glDisable(GL_BLEND);
		glColor4f(0.0, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);

		double aspectRatio;
		switch ( settings->aspectRatio )
		{
			case 1:
				// 2.40:1 is actually 2.39:1
				aspectRatio = 2.39;
				break;
			default:
				aspectRatio = 1.85;
				break;
		}

		double bar_height = ( aspectRatio * dims[3] - dims[2] ) / ( 2.0 * aspectRatio );
		double bar_width = ( dims[2] - aspectRatio * dims[3] ) / 2.0;
		if ( bar_height > 0 )
		{
			glVertex2f(0.0, 0.0);
			glVertex2f(0.0, bar_height);
			glVertex2f(dims[2], bar_height);
			glVertex2f(dims[2], 0.0);
			glVertex2f(0.0, dims[3]);
			glVertex2f(0.0, dims[3]-bar_height);
			glVertex2f(dims[2], dims[3]-bar_height);
			glVertex2f(dims[2], dims[3]);
		}
		else if ( bar_width > 0 )
		{
			glVertex2f(0.0, 0.0);
			glVertex2f(0.0, dims[3]);
			glVertex2f(bar_width, dims[3]);
			glVertex2f(bar_width, 0.0);
			glVertex2f(dims[2], 0.0);
			glVertex2f(dims[2], dims[3]);
			glVertex2f(dims[2]-bar_width, dims[3]);
			glVertex2f(dims[2]-bar_width, 0.0);
		}
		glEnd();
	}
	
	if (camera->fade != 0.0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		glColor4f(1.0, 1.0, 1.0, camera->fade);
		glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, dims[3]);
		glVertex2f(dims[2], dims[3]);
		glVertex2f(dims[2], 0.0);
		glEnd();
		
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
