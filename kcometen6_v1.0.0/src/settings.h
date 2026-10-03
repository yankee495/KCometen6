#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings {
public:
	// ============================================================================
	// THE CLEAN UNIFIED CONSTRUCTOR INITIALIZATION MATRIX
	// All fields perfectly initialized, finished with a clean empty body loop {}!
	// ============================================================================
	Settings() :
	fov(110.0),
	freeCamera(true),
	freeCameraSpeed(1.5),
	sizeScale(2.5),
	cube(true),
	slowMotion(false),
	matrix(false),
	aspectRatio(0),
	maxFps(60),
	mipmaps(false),
	color(0),
	createInterval(0.40),
	timeScale(1.0),
	particleDensity(100),
	rotateComet(true),
	splitComet(true),
	curveComet(true),
	blitz(true),
	bgType(1),                                    // 1 = Custom Static File Mode
	bgFile("/tmp/kcometen/live_desktop.png"),     // Satisfies background texture hooks
	bgDir(""),
	bgSize(2),
	usePointSprites(false),
	cometCountValue(45) {}                        // THE FIX: Clean initialization list body finish!

	~Settings() {}

	double fov;
	bool freeCamera;
	double freeCameraSpeed;
	double sizeScale;
	bool cube;
	bool slowMotion;
	bool matrix;
	int aspectRatio;
	int maxFps;
	bool mipmaps;
	int color;
	double createInterval;
	double timeScale;
	int particleDensity;
	bool rotateComet;
	bool splitComet;
	bool curveComet;
	bool blitz;

	// Explicit variable layout mirroring settings.cpp exactly:
	int bgType;
	std::string bgFile;
	std::string bgDir;
	int bgSize;
	bool usePointSprites;

	static bool getBlur() { return true; }

	// ============================================================================
	// THE UNLEASHED COSMIC STORM PASS:
	// Clean variable field declaration matching your constructor and main parser!
	// ============================================================================
	int cometCountValue;

	int cometCount() const { return cometCountValue; }
};

extern Settings* settings;

#endif
