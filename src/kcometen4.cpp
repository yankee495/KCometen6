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

#include <kaboutapplicationdialog.h>
#include "kcometen4.h"
#include "global.h"
#include "settings.h"
#include <QDesktopWidget>

static KAboutData* s_aboutData = 0;
Settings* settings = 0;

class KCometen4Interface : public KScreenSaverInterface
{
public:
	virtual KAboutData* aboutData() {
		return s_aboutData;
	}

	/** function to create screen saver object */
	virtual KScreenSaver* create( WId id )
	{
	  return new KCometen4( id );
	}

	/** function to create setup dialog for screen saver */
	virtual QDialog* setup()
	{
	  return new KCometen4Setup();
	}
};


int main( int argc, char*argv[] )
{
	s_aboutData = new KAboutData( "kcometen4.kss", 0,
								ki18n("KCometen4"), VERSION,
								ki18n("An OpenGL screensaver with exploding comets."),
								KAboutData::License_GPL,
								ki18n("Copyright (C) 2005 Peter Müller<br>Copyright (C) 2008 John Stamp"),
								KLocalizedString(),
								"http://www.mehercule.net/staticpages/index.php/kcometen4",
								"jstamp@users.sourceforge.net" );
	s_aboutData->addAuthor( ki18n("Peter Müller"),
							ki18n("Original author of <a href=\"http://www.kde-apps.org/content/show.php/KCometen3?content=30313\">KCometen3</a>."),
							"pmueller@cs.tu-berlin.de");
	s_aboutData->addAuthor( ki18n("John Stamp"),
							ki18n("Ported to KDE4 as KCometen4."),
							"jstamp@users.sourceforge.net");

	KCometen4Interface kss;
	return kScreenSaverMain( argc, argv, kss );
}

//-----------------------------------------------------------------------------
//! dialog to setup screen saver parameters
KCometen4Setup::KCometen4Setup( QWidget *parent ) : KDialog( parent )
{
	setCaption( i18n( "Setup KCometen4 Screen Saver" ) );
	setButtons( Ok|Cancel|Help|Default );
	setDefaultButton( Ok );
	setModal( true );
	showButtonSeparator(true);
	setButtonText( Help, i18n( "A&bout" ) );

	QWidget *main = new QWidget( this );
	setMainWidget( main );
	ui.setupUi( main );

	ui.kURLRequesterRandomDirectory->setMode(KFile::Directory);
	buttonGroupBackground.addButton( ui.radioButton3 );
	buttonGroupBackground.addButton( ui.radioButton4 );
	buttonGroupBackground.addButton( ui.radioButton5 );
	buttonGroupBackground.setId( ui.radioButton3, 0 );
	buttonGroupBackground.setId( ui.radioButton4, 1 );
	buttonGroupBackground.setId( ui.radioButton5, 2 );

	// Try to duplicate screen's aspect ratio
	int width = 220;
	int height = 170;
	QRect deskSize = QApplication::desktop()->screenGeometry();
	if ( deskSize.width() > 0 )
		height = (double)width*deskSize.height()/deskSize.width();
	ui.preview->setFixedSize( width, height );

	QPalette p = palette();
	p.setColor( backgroundRole(), Qt::black );
	ui.preview->setPalette( p );

	ui.preview->show();    // otherwise saver does not get correct size
	saver = new KGLCometen4( ui.preview );
	saver->resize( ui.preview->size() );
	saver->start();

	initDialog();  // Do this after we have a saver

	connect( this, SIGNAL( okClicked() ), SLOT( slotOkPressed() ) );
	connect( this, SIGNAL( helpClicked() ), SLOT( slotHelp() ) );
	connect( this, SIGNAL( defaultClicked() ), SLOT( slotDefault() ) );

	connect( ui.checkFreeCamera, SIGNAL( toggled(bool)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.checkIsCube, SIGNAL( toggled(bool)), this, SLOT( updateBoxShape() ) );
	connect( ui.kIntNumInputFps, SIGNAL( valueChanged(int)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.comboBoxColor, SIGNAL( currentIndexChanged(int)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.kDoubleNumInputInterval, SIGNAL( valueChanged(double)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.kIntNumInputDensity, SIGNAL( valueChanged(int)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.checkRotateComet, SIGNAL( toggled(bool)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.checkSplitComet, SIGNAL( toggled(bool)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.checkCurveComet, SIGNAL( toggled(bool)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.checkBlitz, SIGNAL( toggled(bool)), this, SLOT( updateSimpleSettings() ) );

	connect( ui.kDoubleNumInputCamSpeed, SIGNAL( valueChanged(double)), this, SLOT( updateFreeCamSpeed() ) );
	connect( ui.checkSlowmotion, SIGNAL( toggled(bool)), this, SLOT( updateSlowMotionEffects() ) );
	connect( ui.checkRotCam, SIGNAL( toggled(bool)), this, SLOT( updateSlowMotionEffects() ) );
	connect( ui.comboBoxAspectRatio, SIGNAL( currentIndexChanged(int)), this, SLOT( updateSimpleSettings() ) );
	connect( ui.kDoubleNumInputTimeScale, SIGNAL( valueChanged(double)), this, SLOT( updateTimeScale() ) );

	connect( ui.kDoubleNumInputFov, SIGNAL( valueChanged(double)), this, SLOT( updateFov(double) ) );
	connect( ui.kDoubleNumInputSizeScale, SIGNAL( valueChanged(double)), this, SLOT( updateSizeScale(double) ) );

	connect( ui.checkMipmaps, SIGNAL( toggled(bool)), this, SLOT( updateTextures() ) );
	connect( &buttonGroupBackground, SIGNAL( buttonClicked(int)), this, SLOT( updateTextures() ) );
	connect( ui.kURLRequesterFile, SIGNAL( returnPressed()), this, SLOT( updateTextures() ) );
	connect( ui.kURLRequesterFile, SIGNAL( urlSelected(KUrl)), this, SLOT( updateTextures() ) );
	connect( ui.kURLRequesterRandomDirectory, SIGNAL( returnPressed()), this, SLOT( updateTextures() ) );
	connect( ui.kURLRequesterRandomDirectory, SIGNAL( urlSelected(KUrl)), this, SLOT( updateTextures() ) );
	connect( ui.comboBoxBgSize, SIGNAL( currentIndexChanged(int)), this, SLOT( updateTextures() ) );
}

KCometen4Setup::~KCometen4Setup()
{
	delete saver;
}

void KCometen4Setup::slotDefault()
{
	settings->defaults();

	// block signals before we update widgets
	ui.checkFreeCamera->blockSignals(true);
	ui.checkIsCube->blockSignals(true);
	ui.kIntNumInputFps->blockSignals(true);
	ui.comboBoxColor->blockSignals(true);
	ui.kDoubleNumInputInterval->blockSignals(true);
	ui.kIntNumInputDensity->blockSignals(true);
	ui.checkRotateComet->blockSignals(true);
	ui.checkSplitComet->blockSignals(true);
	ui.checkCurveComet->blockSignals(true);
	ui.checkBlitz->blockSignals(true);
	ui.kDoubleNumInputCamSpeed->blockSignals(true);
	ui.checkSlowmotion->blockSignals(true);
	ui.checkRotCam->blockSignals(true);
	ui.comboBoxAspectRatio->blockSignals(true);
	ui.kDoubleNumInputTimeScale->blockSignals(true);
	ui.kDoubleNumInputFov->blockSignals(true);
	ui.kDoubleNumInputSizeScale->blockSignals(true);
	ui.checkMipmaps->blockSignals(true);
	ui.kURLRequesterFile->blockSignals(true);
	ui.kURLRequesterRandomDirectory->blockSignals(true);
	ui.comboBoxBgSize->blockSignals(true);

	initDialog();

	// ui.checkFreeCamera is checked by default
	// enable these by hand since we blocked signals
	ui.textLabel1_2->setEnabled(true);
	ui.kDoubleNumInputCamSpeed->setEnabled(true);

	// unblock
	ui.checkFreeCamera->blockSignals(false);
	ui.checkIsCube->blockSignals(false);
	ui.kIntNumInputFps->blockSignals(false);
	ui.comboBoxColor->blockSignals(false);
	ui.kDoubleNumInputInterval->blockSignals(false);
	ui.kIntNumInputDensity->blockSignals(false);
	ui.checkRotateComet->blockSignals(false);
	ui.checkSplitComet->blockSignals(false);
	ui.checkCurveComet->blockSignals(false);
	ui.checkBlitz->blockSignals(false);
	ui.kDoubleNumInputCamSpeed->blockSignals(false);
	ui.checkSlowmotion->blockSignals(false);
	ui.checkRotCam->blockSignals(false);
	ui.comboBoxAspectRatio->blockSignals(false);
	ui.kDoubleNumInputTimeScale->blockSignals(false);
	ui.kDoubleNumInputFov->blockSignals(false);
	ui.kDoubleNumInputSizeScale->blockSignals(false);
	ui.checkMipmaps->blockSignals(false);
	ui.kURLRequesterFile->blockSignals(false);
	ui.kURLRequesterRandomDirectory->blockSignals(false);
	ui.comboBoxBgSize->blockSignals(false);

	// saver now needs to process a few things
	saver->updateTimeScale();
	saver->updateFreeCamSpeed();
	saver->updateSlowMotionEffects();
	saver->updateTextures();
	saver->updateSizeScale();
}

// The preview will have already read things into settings
void KCometen4Setup::initDialog()
{
	// Scene
	ui.kDoubleNumInputFov->setValue(settings->fov);
	ui.checkFreeCamera->setChecked(settings->freeCamera);
	ui.kDoubleNumInputCamSpeed->setValue(settings->freeCameraSpeed);
	ui.kDoubleNumInputSizeScale->setValue(settings->sizeScale);
	ui.checkIsCube->setChecked(settings->cube);
	ui.checkSlowmotion->setChecked(settings->slowMotion);
	ui.checkRotCam->setChecked(settings->matrix);
	ui.comboBoxAspectRatio->setCurrentIndex(settings->aspectRatio);
	ui.kIntNumInputFps->setValue(settings->maxFps);
	ui.checkMipmaps->setChecked(settings->mipmaps);
	// Comets
	ui.comboBoxColor->setCurrentIndex(settings->color);
	ui.kDoubleNumInputInterval->setValue(settings->createInterval);
	ui.kDoubleNumInputTimeScale->setValue(settings->timeScale);
	ui.kIntNumInputDensity->setValue(settings->particleDensity);
	ui.checkRotateComet->setChecked(settings->rotateComet);
	ui.checkSplitComet->setChecked(settings->splitComet);
	ui.checkCurveComet->setChecked(settings->curveComet);
	ui.checkBlitz->setChecked(settings->blitz);
	// Background
	int activeButton = settings->bgType;
	switch (activeButton)
	{
	case 0:
		ui.radioButton3->setChecked(true);
		break;
	case 1:
		ui.radioButton4->setChecked(true);
		break;
	case 2:
		ui.radioButton5->setChecked(true);
		break;
	}
	ui.kURLRequesterFile->setUrl(settings->bgFile);
	ui.kURLRequesterRandomDirectory->setUrl(settings->bgDir);
	ui.comboBoxBgSize->setCurrentIndex(settings->bgSize);
}


// Ok pressed - save settings and exit
void KCometen4Setup::slotOkPressed()
{
	settings->save();
	accept();
}

void KCometen4Setup::updateSimpleSettings()
{
	settings->aspectRatio = ui.comboBoxAspectRatio->currentIndex();
	settings->color = ui.comboBoxColor->currentIndex();
	settings->createInterval = ui.kDoubleNumInputInterval->value();
	settings->maxFps = ui.kIntNumInputFps->value();
	settings->freeCamera = ui.checkFreeCamera->isChecked();
	settings->curveComet = ui.checkCurveComet->isChecked();
	settings->rotateComet = ui.checkRotateComet->isChecked();
	settings->splitComet = ui.checkSplitComet->isChecked();
	settings->blitz = ui.checkBlitz->isChecked();
	settings->particleDensity = ui.kIntNumInputDensity->value();
}

void KCometen4Setup::updateTimeScale()
{
	settings->timeScale = ui.kDoubleNumInputTimeScale->value();
	saver->updateTimeScale();
}

void KCometen4Setup::updateFreeCamSpeed()
{
	settings->freeCameraSpeed = ui.kDoubleNumInputCamSpeed->value();
	saver->updateFreeCamSpeed();
}

void KCometen4Setup::updateSlowMotionEffects()
{
	settings->slowMotion = ui.checkSlowmotion->isChecked();
	settings->matrix = ui.checkRotCam->isChecked();
	saver->updateSlowMotionEffects();
}

void KCometen4Setup::updateTextures()
{
	settings->bgType = buttonGroupBackground.checkedId();
	settings->bgFile = ui.kURLRequesterFile->url().path();
	settings->bgDir = ui.kURLRequesterRandomDirectory->url().path();
	settings->bgSize = ui.comboBoxBgSize->currentIndex();
	settings->mipmaps = ui.checkMipmaps->isChecked();
	saver->updateTextures();
}

void KCometen4Setup::updateSizeScale( double scale )
{
	settings->sizeScale = scale;
	saver->updateSizeScale();
}

void KCometen4Setup::updateFov( double fov )
{
	settings->fov = fov;
	saver->updateSizeScale();
}

void KCometen4Setup::updateBoxShape()
{
	settings->cube = ui.checkIsCube->isChecked();
	saver->updateSizeScale();
}

void KCometen4Setup::slotHelp()
{
	KAboutApplicationDialog mAbout(s_aboutData, this);
	mAbout.exec();
}


//-----------------------------------------------------------------------------


KCometen4::KCometen4( WId id ) : KScreenSaver( id )
{
	setAttribute( Qt::WA_NoSystemBackground );

	QPalette p = palette();
	p.setColor( backgroundRole(), Qt::black );
	setPalette( p );

	kglcometen4 = new KGLCometen4();

	embed(kglcometen4);

	kglcometen4->start();

	// Show last to avoid occasional flicker on startup
	kglcometen4->show();
}

void KCometen4::resizeEvent( QResizeEvent * )
{
	if ( kglcometen4 )
	{
		kglcometen4->stop();
		kglcometen4->resize( width(), height() );
		kglcometen4->start();
	}
}

KCometen4::~KCometen4()
{
	delete kglcometen4;
}

#include "kcometen4.moc"
