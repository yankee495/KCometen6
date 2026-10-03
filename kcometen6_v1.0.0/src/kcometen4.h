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

#ifndef KCometen4_H__
#define KCometen4_H__

#include "kscreensaver.h"
#include "ui_kcometen4ui.h"

#include "kglcometen4.h"

class KCometen4 : public KScreenSaver
{
    Q_OBJECT
public:
    KCometen4( WId drawable );
    virtual ~KCometen4();

protected:
    void resizeEvent( QResizeEvent *e );

private:
    KGLCometen4 * kglcometen4;
};

class KCometen4Setup : public KDialog
{
    Q_OBJECT
public:
    KCometen4Setup( QWidget *parent = NULL );
    ~KCometen4Setup();

private slots:
    void slotOkPressed();
    void slotHelp();
    void slotDefault();
    void updateSimpleSettings();
    void updateSlowMotionEffects();
    void updateBoxShape();
    void updateSizeScale(double scale);
    void updateFov(double scale);
    void updateTextures();
    void updateTimeScale();
    void updateFreeCamSpeed();

private:
    void initDialog();
    KGLCometen4 *saver;
    Ui::KCometen4UI ui;
    QButtonGroup buttonGroupBackground;
};

#endif
