#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <sys/file.h>
#include <sys/time.h>
#include <unistd.h>
#include <GL/glut.h>
#include "cometenscene.h"
#include "settings.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <thread>

enum TextureSlots {
    TEX_PARTICLE    = 0,  // particle.png
    TEX_LIGHTMAP    = 1,  // lightmap.png
    TEX_WALLS       = 2,  // Widescreen Walls (custom or default)
    TEX_CEILING_CAP = 3,  // Ceiling Cap (custom or default_ceiling_1920x1080.png)
    TEX_FLOOR_CAP   = 4,  // Floor Cap (custom or default_floor_1920x1080.png)
    TEX_COUNT       = 5   // Handy for size definitions
};


// Global Workspace Core Definitions
std::string g_bg_path = "";
std::string g_custom_ceiling_path = "";
std::string g_custom_floor_path = "";

// Expanded to 5 texture slots to decouple floor vs ceiling plates!
//GLuint g_textures[5];
GLuint g_textures[5] = {5};  // Ensure array sizing is explicit
CometenScene* g_scene = nullptr;
Settings* settings = nullptr;

GLfloat g_rot_x = 0.0f;
GLfloat g_rot_y = 0.0f;

// High Precision Timing Engine Tracker
double get_elapsed_time() {
    static struct timeval start_time;
    static bool initialized = false;
    if (!initialized) {
        gettimeofday(&start_time, nullptr);
        initialized = true;
    }
    struct timeval current_time;
    gettimeofday(&current_time, nullptr);
    return (current_time.tv_sec - start_time.tv_sec) +
    (current_time.tv_usec - start_time.tv_usec) / 1000000.0;
}

// ============================================================================
// THE KCOMETEN6 FAIL-SAFE TEXTURE PATH VERIFIER:
// Returns true if a file path actually exists and is readable on this machine!
// ============================================================================
bool file_exists_and_readable(const std::string& path) {
    if (path.empty()) return false;
    std::ifstream f(path.c_str());
    return f.good();
}

bool load_texture(const std::string& path, GLuint* texture_id) {
    if (path.empty()) return false;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    // Force 4 channels (RGBA) to support transparency smoothly
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "[ERROR] stbi_load failed to read path asset: " << path << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, *texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // UNIVERSAL SIZE COMPATIBILITY FIX: Use gluBuild2DMipmaps so any 4K JPG/PNG scales perfectly!
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    std::cout << "[SUCCESS] Hardware processed and mapped image asset: " << path << std::endl;
    return true;
}

// Clean Graceful Termination Handler Pass For Sleep
//void handle_system_signals(int signal) {
    // SECURITY FLUSH: Instantly clear out your single-run protection locks
//    unlink("/tmp/kcometen.lock");

    // SAFE SYSTEM EXIT: Direct kernel exit completely bypasses the dead OpenGL pipeline context,
    // ensuring the application terminates instantly without ever tripping a GPU driver kernel panic!
//    _exit(0);
//}

void display_callback() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // IMMERSIVE UP-CLOSE VIEW: Move camera depth to 600.0 to step deep inside room!
    gluLookAt(0.0, 0.0, 600.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glRotatef(g_rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rot_y, 0.0f, 1.0f, 0.0f);

    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // ============================================================================
    // THE KCOMETEN6 HIGH-PRECISION FPS FRAME GOVERNOR:
    // Tracks frame processing speeds and clamps performance tightly to your slider target!
    // ============================================================================
double frame_start_tick = get_elapsed_time();

    if (g_scene) {
        double current_time = get_elapsed_time();
        g_scene->process(current_time);

        // Match the up-close camera depth vector coordinate precisely
        Vec3f camera_pos(0.0f, 0.0f, 600.0f);
        g_scene->render(camera_pos);
    }

    glutSwapBuffers();

    // Calculate how long a single frame is allowed to live in seconds
    double target_frame_duration = 1.0 / (double)settings->maxFps;

    // Force the CPU thread to yield nicely until your target frame time has passed
    while ((get_elapsed_time() - frame_start_tick) < target_frame_duration) {
        // Prevents raw high-frequency lockup loops, keeping the CPU completely cool!
        std::this_thread::yield();
    }
}

void idle_callback() {
    // Increment rotation matrix counters smoothly over time
    g_rot_x += 0.15f;
    g_rot_y += 0.25f;
    if (g_rot_x > 360.0f) g_rot_x -= 360.0f;
    if (g_rot_y > 360.0f) g_rot_y -= 360.0f;

    glutPostRedisplay();
}

void reshape_callback(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // FAR CLIPPING PLANE PROTECTION: Set to 10000.0 to stop corner black clipping!
    gluPerspective(60.0, static_cast<double>(w) / static_cast<double>(h), 1.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard_callback(unsigned char key, int x, int y) {
    if (key == 27) { // Escape Key
        delete g_scene;
        glDeleteTextures(5, g_textures);
        unlink("/tmp/kcometen.lock");
        exit(0);
    }
}

int main(int argc, char** argv) {
    // 1. MULTI-SPAWN RUNTIME PROTECTION LOCK
    int lock_fd = open("/tmp/kcometen.lock", O_RDWR | O_CREAT, 0666);
    if (lock_fd < 0 || flock(lock_fd, LOCK_EX | LOCK_NB) < 0) {
        std::cerr << "[SYSTEM PROTECTION] KCometen instance is already active! Exiting..." << std::endl;
        return 0;
    }

    // 2. INITIALIZE GLOBAL PATH VECTOR FIRST: Lock down default RAM disk asset layout
    g_bg_path = "/tmp/kcometen/live_desktop.png";

    // 3. AUTOMATED FOLDER LOOKUP: Switch focus to binary directory path
    if (argc > 0 && argv != nullptr && argv[0] != nullptr) {
        std::string binary_path = argv[0];
        size_t last_slash = binary_path.find_last_of("/\\");
        if (last_slash != std::string::npos) {
            std::string run_dir = binary_path.substr(0, last_slash);
            int chdir_status = chdir(run_dir.c_str());
            (void)chdir_status;
        }
    }

    // 4. STATIC BACKGROUND OVERRIDE FLAG CHECKER LOOP: Override path if user inputs flag
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--bg" && i + 1 < argc) {
            g_bg_path = argv[i+1];
        }
    }

    // 5. Register signal handler routines right underneath
//    std::signal(SIGTERM, handle_system_signals);
//    std::signal(SIGINT,  handle_system_signals);
//    std::signal(SIGHUP,  handle_system_signals);

    std::cout << "[DEBUG] Executing automated desktop canvas capture..." << std::endl;

    // Create the RAM disk target folder quietly
    int status_dir = system("mkdir -p /tmp/kcometen");
    (void)status_dir;

    // Command spectacle to capture the crisp authorized screenshot natively
    int status_shot = system("spectacle -b -n -o /tmp/kcometen/live_desktop.png");
    if (status_shot != 0) {
        std::cerr << "[WARNING] Spectacle capture execution failed! Falling back to cached layers..." << std::endl;
    } else {
        std::cout << "[SUCCESS] Native C++ binary took an authorized desktop snapshot to /tmp/kcometen/" << std::endl;
    }

    g_bg_path = "/tmp/kcometen/live_desktop.png";

    // Initialize the GLUT engine window wrapper context properties
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Fetch the native hardware screen spans before assigning scene box bounds
    int screen_w = glutGet(GLUT_SCREEN_WIDTH);
    int screen_h = glutGet(GLUT_SCREEN_HEIGHT);

    int boxWidth = (screen_w > 0) ? screen_w : 3840;
    int boxHeight = (screen_h > 0) ? screen_h : 2160;

    glutInitWindowSize(boxWidth, boxHeight);
    int window_id = glutCreateWindow("KCometen6 Standalone");
    (void)window_id;
    glutFullScreen();

     // Allocate memory cleanly inside main context loop wrapper pass
    settings = new Settings();

    // ============================================================================
    // THE 100% PORTABLE CONF PROFILE TEXT PARSER HOOK
    // ============================================================================
    std::string run_dir = ".";
    if (argc > 0 && argv != nullptr && argv[0] != nullptr) {
        std::string binary_path = argv[0];
        size_t last_slash = binary_path.find_last_of("/\\");
        if (last_slash != std::string::npos) {
            run_dir = binary_path.substr(0, last_slash);
        }
    }

    std::string config_file_path = run_dir + "/settings.conf";
    std::ifstream cfile(config_file_path.c_str());
    if (cfile.is_open()) {
        std::string line;
        while (std::getline(cfile, line)) {
            // Trim trailing carriage returns safely if editing on mixed systems
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }

            // Trim leading spaces from the raw line to check for comments cleanly
            size_t start_idx = line.find_first_not_of(" \t");
            if (start_idx == std::string::npos || line[start_idx] == '#') continue;

            size_t delim_pos = line.find('=');
            if (delim_pos != std::string::npos) {
                std::string key = line.substr(0, delim_pos);
                std::string val = line.substr(delim_pos + 1);

                // SPACE-SAFE TRIMMER: Only clean surrounding padding tabs/spaces,
                // leaving interior spaces inside your file paths completely whole!
                key.erase(0, key.find_first_not_of(" \t"));
                size_t end_key = key.find_last_not_of(" \t");
                if (end_key != std::string::npos) key.erase(end_key + 1);

                val.erase(0, val.find_first_not_of(" \t"));
                size_t end_val = val.find_last_not_of(" \t");
                if (end_val != std::string::npos) val.erase(end_val + 1);

                // Secure data linkage assignment checks
                // ============================================================================
                // Connected using continuous else if links so keys never fall through the cracks!
                // ============================================================================
                if (key == "createInterval")       settings->createInterval = std::stod(val);
                else if (key == "cometCount")      settings->cometCountValue = std::stoi(val); // Must match settings.h variable!
                else if (key == "sizeScale")       settings->sizeScale = std::stod(val);
                else if (key == "timeScale")       settings->timeScale = std::stod(val);
                else if (key == "rotateComet")     settings->rotateComet = (val == "true");
                else if (key == "splitComet")      settings->splitComet = (val == "true");
                else if (key == "curveComet")      settings->curveComet = (val == "true");
                else if (key == "usePointSprites") settings->usePointSprites = (val == "true");
                else if (key == "blitz")           settings->blitz = (val == "true");
                else if (key == "colorMode")       settings->color = std::stoi(val);
                else if (key == "bgMode" && val == "file") settings->bgType = 1;
//                else if (key == "customBgFile")    { settings->bgFile = val; g_bg_path = val; }
                // THE MASTER BACKGROUND CANVAS ROUTER MATRIX:
                // THE MASTER BACKGROUND CANVAS ROUTER MATRIX:
                // Evaluates the canvas mode, then handles file path overrides conditionally!
                if (key == "bgMode") {
                    if (val == "file") {
                        settings->bgType = 1; // Custom Static File Mode
                    } else {
                        settings->bgType = 0; // Live Desktop Screenshot Mode
                    }
                }
                else if (key == "customBgFile") {
                    // KCOMETEN6 WALL REVERSION ACCELERATOR:
                    // If the user selected 'file' mode, check if the file is actually there.
                    // If it's missing, forcefully drop back to Live Mode so the walls never render black!
                    if (settings->bgType == 1 && !file_exists_and_readable(val)) {
                        std::cout << "[WARNING] Custom wall file unreadable! Reverting to Live Desktop Mode..." << std::endl;
                        settings->bgType = 0; // Emergency fallback to Live Mode
                        settings->bgFile = "";
                        g_bg_path = "/tmp/kcometen/live_desktop.png"; // Reset asset pointer back to screenshot
                    } else {
                        // If everything is completely fine, load the user paths safely
                        settings->bgFile = val;
                        if (settings->bgType == 1) {
                            g_bg_path = val; // Lock active path to custom image selection
                        }
                    }
                }

                else if (key == "maxFps")          settings->maxFps = std::clamp<int>(std::stoi(val), 10, 999);
                else if (key == "mipmaps")         settings->mipmaps = (val == "true");

                // ============================================================================
                // KCOMETEN6 FAIL-SAFE TEXTURE ROUTER:
                // If the path from settings.conf is readable, use it. Otherwise, force local fallbacks!
                // ============================================================================
                else if (key == "customCeilingFile") {
                    if (file_exists_and_readable(val)) {
                        g_custom_ceiling_path = val;
                    } else {
                        std::cout << "[INFO] Ceiling path invalid. Using default_ceiling_1920x1080.png." << std::endl;
                        g_custom_ceiling_path = "default_ceiling_1920x1080.png"; // Fallback to local asset in run_dir!
                    }
                }
                else if (key == "customFloorFile") {
                    if (file_exists_and_readable(val)) {
                        g_custom_floor_path = val;
                    } else {
                        std::cout << "[INFO] Floor path invalid. Using default_floor_1920x1080.png." << std::endl;
                        g_custom_floor_path = "default_floor_1920x1080.png"; // Fallback to local asset in run_dir!
                    }
                }
            }
        }
        cfile.close();
        std::cout << "[SUCCESS] Space-safe settings.conf successfully overwrote active engine states." << std::endl;
    } else {
        std::cout << "[INFO] Portable configuration profile file not found. Operating on baseline defaults." << std::endl;
    }

    // ============================================================================
    // THE CLEAN 5-TEXTURE ALLOCATOR MATRICES
    // ============================================================================
    glGenTextures(5, g_textures);

    // Core particle system masks
    load_texture("particle.png", &g_textures[0]);
    load_texture("lightmap.png", &g_textures[1]);

    // Slot 2: Widescreen Walls (Processed EXACTLY once safely)
    if (settings->bgType == 1 && !settings->bgFile.empty()) {
        load_texture(settings->bgFile, &g_textures[2]);
    } else {
        load_texture(g_bg_path, &g_textures[2]);
    }

    // Slot 3: Ceiling Cap (Processed exactly once safely)
    if (!g_custom_ceiling_path.empty()) {
        load_texture(g_custom_ceiling_path, &g_textures[3]);
    } else {
        load_texture("default_ceiling_1920x1080.png", &g_textures[3]);
    }

    // Slot 4: Floor Cap (Processed exactly once safely)
    if (!g_custom_floor_path.empty()) {
        load_texture(g_custom_floor_path, &g_textures[4]);
    } else {
        load_texture("default_floor_1920x1080.png", &g_textures[4]);
    }

//
    // Pass the base array pointer directly to initialize the simulation scene matrix
    //g_scene = new CometenScene(g_textures, boxWidth, boxHeight, 1080.0);
    // FIXED: Aligning the parameter variables to match the true class constructor layout signatures!
    // Passes the high precision elapsed clock tracker cleanly into the second slot.
    g_scene = new CometenScene(g_textures, get_elapsed_time(), boxWidth, boxHeight);

    // Register execution callbacks cleanly into GLUT
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);
    glutIdleFunc(idle_callback);

    // Trigger master loop matrix
    glutMainLoop();
    return 0;
}
