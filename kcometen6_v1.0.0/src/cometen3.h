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

#ifndef COMETEN3_H
#define COMETEN3_H

#include "global.h"
#include "cometenscene.h"
#include "comet.h"
#include "cometenmath.h"
#include "bezier.h"

// t'(t) = (t - t0) * time_scale + t1
class Timer {
public:
	Timer() {
		t0 = 0.0;
		t1 = 0.0;
		timeScale = 1.0;
	}
	~Timer() {}
	
	void setTime(double startTime, double realTime) {
		t1 = startTime;
		t0 = realTime;
	}
	
	void setScale(double ts, double realTime) {
		t1 = getTime(realTime);
		t0 = realTime;
		timeScale = ts;
	}
	
	double getTime(double realTime) {
		return (realTime - t0) * timeScale + t1;
	}
	
	double getScale() {
		return timeScale;
	}
	
private:
	double t0;
	double t1;
	double timeScale;	
};

class Camera {
public:
	Camera(const Vec3f& pos, const Vec3f& viewp)	{
		position = pos;
		viewpoint = viewp;
		fade = 0.0;
		blackBars = false;
	}
	
	~Camera() {}
	
	Vec3f position;
	Vec3f viewpoint;
	
	double fade;
	Vec4f fadeColor;
	double blackBars;
	Vec4f blackBarsColor;
};

class Cometen3 {
public:
	Cometen3();
	~Cometen3();
	
	void init(const GLuint* textures,
	          int resX, int resY, double time);
	void quit();
	void process(double time);
	void render();
	void resize(int resX, int resY);
	void updateTimeScale(double time);
	void updateFreeCamSpeed();
	void updateSlowMotionEffects(double time);
	void updateSizeScale();

private:
	void changeViewport(int x, int y, int width, int height);
	void resizeBox();

	void newCameraCurve(double time);
	void processDefaultCameraPos(double time);
	void processNormal(double time);
	void processMatrix(double time);
	void processSlowmotion(double time);

	const GLuint* textures;
	Camera *camera;
	CometenScene* scene;
	Timer timeScale;
	Timer sceneTimer;
	enum {
		S_NORMAL,
		S_MATRIX,
		S_SLOWMOTION,
		S_SPLITSCREEN,
		S_NUM
	};
	int state;
	double stateStart;
	
	Vec3f defaultCameraPosition;
	
	int resX;
	int resY;

	double fovx;
	double fovy;
	
	double nextEffect;
	
	Bezier3* cameraCurve;
	double curveTime;
	double curveCreateTime;

	double matrixAngle;
};

#endif
