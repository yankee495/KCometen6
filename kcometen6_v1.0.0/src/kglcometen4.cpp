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

#include "kglcometen4.h"
extern GLuint g_textures[];
#include "cometenmath.h"
extern GLuint g_textures[];
#include "settings.h"
extern GLuint g_textures[];
#include <QApplication>
extern GLuint g_textures[];
#include <QDesktopWidget>
extern GLuint g_textures[];
#include <QImage>
extern GLuint g_textures[];


KGLCometen4::KGLCometen4(QWidget *parent)
: QGLWidget(parent)
{
	settings = new Settings();
	cometen3 = new Cometen3();
	textures = new GLuint[T_NUM];
}

KGLCometen4::~KGLCometen4() {
	delete cometen3;
	delete[] textures;
	delete settings;
}

void KGLCometen4::start()
{
	m_timerId = startTimer(1000/settings->maxFps);
}

void KGLCometen4::stop()
{
	killTimer( m_timerId );
}

void KGLCometen4::updateTimeScale()
{
	cometen3->updateTimeScale(t.elapsed() / 1000.0);
}

void KGLCometen4::updateFreeCamSpeed()
{
	cometen3->updateFreeCamSpeed();
}

void KGLCometen4::updateSlowMotionEffects()
{
	cometen3->updateSlowMotionEffects(t.elapsed() / 1000.0);
}

void KGLCometen4::updateTextures()
{
	QImage image;
	
	srand(time(NULL));

	if (settings->bgType == 2) {
		QString file = randomFileFromDir(settings->bgDir);
		if (!image.load(file)) {
			image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
		}
	}
	else if (settings->bgType == 1) {
		if (!image.load(settings->bgFile)) {
			image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
		}
	}
	else {
		image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
	}
	
	switch (settings->bgSize) {
	case 0:
		image = image.scaled(128, 128, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 1:
		image = image.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 2:
		image = image.scaled(512, 512, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 3:
		image = image.scaled(1024, 1024, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		break;
	case 4:
	default:
		break;
	}
		
	image = QGLWidget::convertToGLFormat(image);


	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textures[T_BACKGROUND]);
	glBindTexture(GL_TEXTURE_2D, textures[T_BACKGROUND]); 

	if (settings->mipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width(), image.height(),
		                  GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(),
		             0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	
	textures[T_PARTICLE] = loadTexture("particle.png");
	textures[T_LIGHTMAP] = loadTexture("lightmap.png");
}

void KGLCometen4::initializeGL() {
	qglClearColor(QColor(Qt::black));
	updateTextures();

	t.start();
	cometen3->init(textures, 640, 480, t.elapsed() / 1000.0);
}

void KGLCometen4::resizeGL(int res_x, int res_y) {
	cometen3->resize(res_x, res_y);
}

void KGLCometen4::updateSizeScale()
{
	cometen3->updateSizeScale();
}

void KGLCometen4::paintGL() {
	cometen3->render();
}

void KGLCometen4::timerEvent(QTimerEvent * ) {
	cometen3->process(t.elapsed() / 1000.0);
	updateGL();
}

QString KGLCometen4::randomFileFromDir(const QString& dirname) {
	QDir dir(dirname, "*.jpg;*.jpeg;*.png;*.gif",
	         QDir::Unsorted, QDir::Files | QDir::Readable);
	
	if (dir.count() < 1) {
		return QString("");
	}
	
	int num = rand() % dir.count();
	
	return dir.absolutePath() + "/" + dir[num];
}

GLuint KGLCometen4::loadTexture(const QString& file) {
	QImage image;
	GLuint tex = 0;

	if (image.load(KStandardDirs::locate("data", 
	      QString("kcometen4/pixmaps/") + file))) {
		image = QGLWidget::convertToGLFormat(image);
		
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex); 

		if (settings->mipmaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width(), image.height(),
			                  GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
			             0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	return tex;
}

#include "kglcometen4.moc"
extern GLuint g_textures[];
