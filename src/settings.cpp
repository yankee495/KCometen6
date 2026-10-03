/***************************************************************************
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

#include "settings.h"
#include "cometenmath.h"
#include <KConfigGroup>
#include <KGlobal>
#include <KSharedConfig>

Settings::Settings()
{
	load();
	usePointSprites = true;
}

void Settings::load()
{
	KConfigGroup config( KGlobal::config(), "Settings" );

	// Scene
	fov = clamp<double>(config.readEntry("fov", 76.0), 10.0, 120.0);
	freeCamera = config.readEntry( "freecamera", true );
	freeCameraSpeed = clamp<double>(config.readEntry("freecamspeed", 1.5), 0.0, 10.0);
	sizeScale = clamp<double>(config.readEntry("sizescale", 2.5), 0.5, 5.0);
	cube = config.readEntry( "cube", false );
	slowMotion = config.readEntry( "slowmotion", true );
	matrix = config.readEntry( "matrix", true );
	aspectRatio = config.readEntry( "aspectratio", 0 );
	maxFps = clamp<int>(10, 999, config.readEntry("maxfps", 60));
	mipmaps = config.readEntry("mipmaps", false);
	// Comets
	color = config.readEntry("color", 0);
	createInterval = clamp<double>(config.readEntry("interval", 1.25), 0.1, 10.0);
	timeScale = clamp<double>(config.readEntry("timescale", 1.0), 0.1, 5.0);
	particleDensity = clamp<int>(config.readEntry( "particledensity", 100), 0, 100);
	rotateComet = config.readEntry( "rotatecomet", true );
	splitComet = config.readEntry( "splitcomet", true );
	curveComet = config.readEntry( "curvecomet", true );
	blitz = config.readEntry( "lightning", true );
	// Background
	bgType = config.readEntry("bgtype", 0);
	bgFile = config.readEntry("bgfile","");
	bgDir = config.readEntry("bgdir","");
	bgSize = config.readEntry("bgsize", 2);
}

void Settings::defaults()
{
	// Scene
	fov = 76.0;
	freeCamera = true;
	freeCameraSpeed = 1.5;
	sizeScale = 2.5;
	cube = false;
	slowMotion = true;
	matrix = true;
	aspectRatio = 0;
	maxFps = 60;
	mipmaps = false;
	// Comets
	color = 0;
	createInterval = 1.25;
	timeScale = 1.0;
	particleDensity = 100;
	rotateComet = true;
	splitComet = true;
	curveComet = true;
	blitz = true;
	// Background
	bgType = 0;
	bgFile = QString();
	bgDir = QString();
	bgSize = 2;
}

void Settings::save()
{
	KConfigGroup config( KGlobal::config(), "Settings" );

	// Scene
	config.writeEntry("fov", fov);
	config.writeEntry("freecamera", freeCamera);
	config.writeEntry("freecamspeed", freeCameraSpeed);
	config.writeEntry("sizescale", sizeScale);
	config.writeEntry("cube", cube);
	config.writeEntry("slowmotion", slowMotion);
	config.writeEntry("matrix", matrix);
	config.writeEntry("aspectratio", aspectRatio);
	config.writeEntry("maxfps", maxFps);
	config.writeEntry("mipmaps", mipmaps);
	// Comets
	config.writeEntry("color", color);
	config.writeEntry("interval", createInterval);
	config.writeEntry("timescale", timeScale);
	config.writeEntry("particledensity", particleDensity);
	config.writeEntry("rotatecomet", rotateComet);
	config.writeEntry("splitcomet", splitComet);
	config.writeEntry("curvecomet", curveComet);
	config.writeEntry("lightning", blitz);
	// Background
	config.writeEntry("bgtype", bgType);
	config.writeEntry("bgfile", bgFile);
	config.writeEntry("bgdir", bgDir);
	config.writeEntry("bgsize", bgSize);

	config.sync();
}
