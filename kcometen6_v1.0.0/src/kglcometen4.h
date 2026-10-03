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

#ifndef KGLCOMETEN4_H
#define KGLCOMETEN4_H

#include <qdir.h>
#include <qgl.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdatetime.h>
#include <kstandarddirs.h>
#include "cometen3.h"
#include "global.h"

/**
@author Peter Mueller
*/
class KGLCometen4 : public QGLWidget
{
Q_OBJECT
public:
    KGLCometen4(QWidget *parent = 0);
    virtual ~KGLCometen4();
	
	void start();
	void stop();
	void updateSizeScale();
	void updateTextures();
	void updateTimeScale();
	void updateFreeCamSpeed();
	void updateSlowMotionEffects();

protected:
	void initializeGL();
	void resizeGL( int, int );
	void paintGL();
	void timerEvent( QTimerEvent * );
	
private:
	GLuint loadTexture(const QString& file);
	QString randomFileFromDir(const QString& dir);
		
	Cometen3 * cometen3;
	GLuint* textures;
	int m_timerId;
	
	QTime t;
};

#endif
