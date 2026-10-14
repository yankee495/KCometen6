#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <sys/file.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <limits.h>
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

// 📡 Global environment screenshot utility
std::string g_screenshot_utility = ""; //"spectacle -b -n -o"; // Default fallback

// FORCE TRUE 5-SLOT HARDWARE TEXTURE ARRAY TRACKS:
GLuint g_textures[5] = {0, 0, 0, 0, 0};
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

// ============================================================================
// DUAL-AWARE ENVIRONMENT PATH ROUTING ENGINE
// Automatically maps lookups between user configs, system share, or portables
// ============================================================================

std::string get_binary_runtime_directory() {
    char exe_path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", exe_path, PATH_MAX);
    if (count != -1) {
        std::string path_str(exe_path, count);
        size_t last_slash = path_str.find_last_of("/");
        if (last_slash != std::string::npos) {
            return path_str.substr(0, last_slash);
        }
    }
    return ".";
}

std::string get_active_asset_path(const std::string& filename) {
    std::string run_dir = get_binary_runtime_directory();

    // 1. System Package Mode: If running out of /usr/bin, fetch static engine templates from /usr/share
    if (run_dir.rfind("/usr", 0) == 0) {
        std::string system_share_path = "/usr/share/kcometen6/" + filename;
        if (file_exists_and_readable(system_share_path)) {
            return system_share_path;
        }
    }

    // 2. Portable Sandbox Mode Fallback: Check right next to your running executable binary directory
    std::string portable_path = run_dir + "/" + filename;
    if (file_exists_and_readable(portable_path)) {
        return portable_path;
    }

    if (file_exists_and_readable(filename)) {
        return filename;
    }
    return filename;
}

std::string locate_active_config() {
    std::string run_dir = get_binary_runtime_directory();

    // If installed globally system-wide, read custom user parameters from ~/.config profile tracks
    if (run_dir.rfind("/usr", 0) == 0) {
        const char* home = std::getenv("HOME");
        if (home != nullptr) {
            std::string user_conf = std::string(home) + "/.config/kcometen6/settings.conf";
            if (file_exists_and_readable(user_conf)) return user_conf;
        }
        return "/usr/share/kcometen6/settings.conf";
    }

    // Otherwise, trap configurations entirely local inside your runtime portable folder directory
    std::string portable_conf = run_dir + "/settings.conf";
    if (file_exists_and_readable(portable_conf)) return portable_conf;

    return "settings.conf";
}

bool load_texture(const std::string& path, GLuint* texture_id) {
    if (path.empty()) return false;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "[ERROR] stbi_load failed to read path asset: " << path << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, *texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    std::cout << "[SUCCESS] Hardware processed and mapped image asset: " << path << std::endl;
    return true;
}

void display_callback() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 600.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glRotatef(g_rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rot_y, 0.0f, 1.0f, 0.0f);

    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    double frame_start_tick = get_elapsed_time();

    if (g_scene) {
        double current_time = get_elapsed_time();
        g_scene->process(current_time);
        Vec3f camera_pos(0.0f, 0.0f, 600.0f);
        g_scene->render(camera_pos);
    }

    glutSwapBuffers();

    double target_frame_duration = 1.0 / (double)settings->maxFps;
    while ((get_elapsed_time() - frame_start_tick) < target_frame_duration) {
        std::this_thread::yield();
    }
}

void idle_callback() {
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

    // 💡 ALLOCATE THE SETTINGS STRUCT IMMEDIATELY AT START (THE CRASH FIX)
    // This gives your parser a real memory buffer to write into, killing the SegFault!
    settings = new Settings();

    // 2. INITIALIZE GLOBAL PATH VECTOR FIRST
    g_bg_path = "/tmp/kcometen/live_desktop.png";

    // 3. AUTOMATED FOLDER LOOKUP: Switch focus to binary directory path IF not a system package
    std::string run_dir = get_binary_runtime_directory();
    if (run_dir.rfind("/usr", 0) != 0) {
        int chdir_status = chdir(run_dir.c_str());
        (void)chdir_status;
    }

    // 4. STATIC BACKGROUND OVERRIDE FLAG CHECKER LOOP
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--bg" && i + 1 < argc) {
            g_bg_path = argv[i+1];
        }
    }

    // ============================================================================
    // 💡 STEP A: READ THE CONFIG FILE FIRST (Moved up from below)
    // ============================================================================
    // ============================================================================
    // DUAL-AWARE CONF PROFILE TEXT PARSER HOOK
    // ============================================================================
    std::string config_file_path = locate_active_config();
    std::ifstream cfile(config_file_path.c_str());
    if (cfile.is_open()) {
        std::string line;
        while (std::getline(cfile, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }

            size_t start_idx = line.find_first_not_of(" \t");
            if (start_idx == std::string::npos || line[start_idx] == '#') continue;

            size_t delim_pos = line.find('=');
            if (delim_pos != std::string::npos) {
                std::string key = line.substr(0, delim_pos);
                std::string val = line.substr(delim_pos + 1);

                key.erase(0, key.find_first_not_of(" \t"));
                size_t end_key = key.find_last_not_of(" \t");
                if (end_key != std::string::npos) key.erase(end_key + 1);

                val.erase(0, val.find_first_not_of(" \t"));
                size_t end_val = val.find_last_not_of(" \t");
                if (end_val != std::string::npos) val.erase(end_val + 1);

                // Secure data linkage assignment checks
                if (key == "createInterval")       settings->createInterval = std::stod(val);
                else if (key == "cometCount")      settings->cometCountValue = std::stoi(val);
                else if (key == "sizeScale")       settings->sizeScale = std::stod(val);
                else if (key == "timeScale")       settings->timeScale = std::stod(val);
                else if (key == "rotateComet")     settings->rotateComet = (val == "true");
                else if (key == "splitComet")      settings->splitComet = (val == "true");
                else if (key == "curveComet")      settings->curveComet = (val == "true");
                else if (key == "usePointSprites") settings->usePointSprites = (val == "true");
                else if (key == "blitz")           settings->blitz = (val == "true");
                else if (key == "colorMode")       settings->color = std::stoi(val);

                // Evaluates the canvas mode, then handles file path overrides conditionally!
                else if (key == "bgMode") {
                    if (val == "file") {
                        settings->bgType = 1;
                    } else {
                        settings->bgType = 0;
                    }
                }
                else if (key == "customBgFile") {
                    if (settings->bgType == 1 && !file_exists_and_readable(val)) {
                        std::cout << "[WARNING] Custom wall file unreadable! Reverting to Live Desktop Mode..." << std::endl;
                        settings->bgType = 0;
                        settings->bgFile = "";
                        g_bg_path = "/tmp/kcometen/live_desktop.png";
                    } else {
                        settings->bgFile = val;
                        if (settings->bgType == 1) {
                            g_bg_path = val;
                        }
                    }
                }
                else if (key == "maxFps")          settings->maxFps = std::clamp<int>(std::stoi(val), 10, 999);
                else if (key == "mipmaps")         settings->mipmaps = (val == "true");

                else if (key == "customCeilingFile") {
                    if (file_exists_and_readable(val)) {
                        g_custom_ceiling_path = val;
                    } else {
                        std::cout << "[INFO] Ceiling path invalid. Using fallback asset tracking." << std::endl;
                        g_custom_ceiling_path = get_active_asset_path("default_ceiling_1920x1080.png");
                    }
                }
                else if (key == "customFloorFile") {
                    if (file_exists_and_readable(val)) {
                        g_custom_floor_path = val;
                    } else {
                        std::cout << "[INFO] Floor path invalid. Using fallback asset tracking." << std::endl;
                        g_custom_floor_path = get_active_asset_path("default_floor_1920x1080.png");
                    }
                }
                // 📸 THE MODULAR INFRASTRUCTURE FIX: Read user selected screenshot utility straight from settings.conf
                else if (key == "screenshotUtility") {
                    g_screenshot_utility = val;
                    // Note: Your val variable is already stripped of spaces by your parser layout above!
                }
            }
        }
        cfile.close();
        std::cout << "[SUCCESS] Space-safe settings.conf successfully overwrote active engine states." << std::endl;
    } else {
        std::cout << "[INFO] Configuration profile file not found. Operating on baseline defaults." << std::endl;
    }
    // ============================================================================
    // 💡 STEP B: FIRE THE SCREENSHOT CAPTURE ENGINE SECOND
    // ============================================================================
    std::cout << "[DEBUG] Executing automated desktop canvas capture..." << std::endl;
    int status_dir = system("mkdir -p /tmp/kcometen");
    (void)status_dir;

    unlink("/tmp/kcometen/live_desktop.png");

    if (g_screenshot_utility.empty()) {
        std::cout << "[INFO] Screenshot command is blank. Skipping live snapshot and utilizing cached asset states." << std::endl;
    } else {
        std::string full_constructed_cmd = g_screenshot_utility + " /tmp/kcometen/live_desktop.png";
        std::cout << "[DEBUG] Shell Execution Command: [" << full_constructed_cmd << "]" << std::endl;

        int raw_status = system(full_constructed_cmd.c_str());

        int exit_code = 0;
        if (raw_status != -1) {
            #ifndef _WIN32
            exit_code = WEXITSTATUS(raw_status);
            #else
            exit_code = raw_status;
            #endif
        } else {
            exit_code = -1;
        }

        bool file_successfully_generated = file_exists_and_readable("/tmp/kcometen/live_desktop.png");

        if (exit_code != 0 || !file_successfully_generated) {
            std::cerr << "[WARNING] Capture execution failed! Falling back to cached layers..." << std::endl;

            // 🖼️ THE DEFAULT WALL ALIGNMENT LINE:
            // Forcefully redirect the background engine to your safe default texture asset!
            g_bg_path = get_active_asset_path("default_walls_1920x1080.png"); // or your preferred default fallback filename
        } else {
            std::cout << "[SUCCESS] Native C++ binary took an authorized desktop snapshot to /tmp/kcometen/" << std::endl;
            g_bg_path = "/tmp/kcometen/live_desktop.png";
        }
      }

    // ============================================================================
    // 💡 STEP C: INITIALIZE THE GLUT RUNTIME SYSTEM WINDOW (Keeps remaining setup pure)
    // ============================================================================
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

// 🕵️ SCAN FOR THE WINDOWED PARAMETER FLAG
bool run_windowed = false;
for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-w" || std::string(argv[i]) == "--windowed") {
        run_windowed = true;
        break;
    }
}

int screen_w = glutGet(GLUT_SCREEN_WIDTH);
int screen_h = glutGet(GLUT_SCREEN_HEIGHT);
int boxWidth = (screen_w > 0) ? screen_w : 3840;
int boxHeight = (screen_h > 0) ? screen_h : 2160;

if (run_windowed) {
    // 🪟 WINDOWED CANVAS: Set an elegant 1080p frame for 4K screens, scale down for others
    if (boxWidth >= 3840) {
        boxWidth = 1920;
        boxHeight = 1080;
    } else {
        boxWidth = static_cast<int>(boxWidth * 0.80);
        boxHeight = static_cast<int>(boxHeight * 0.80);
    }

    glutInitWindowSize(boxWidth, boxHeight);

    // Center the frame beautifully on the monitor workspace
    int posX = (screen_w - boxWidth) / 2;
    int posY = (screen_h - boxHeight) / 2;
    glutInitWindowPosition(posX, posY);

    int window_id = glutCreateWindow("KCometen6 Live Window");
    (void)window_id;
    // 🚀 Skip glutFullScreen() entirely so it remains windowed!
} else {
    // 🌌 SCREEN SAVER MODE: Lock absolute, pristine full screen
    glutInitWindowSize(boxWidth, boxHeight);
    int window_id = glutCreateWindow("KCometen6 Standalone");
    (void)window_id;
    glutFullScreen();
}

    // ============================================================================
    // THE CLEAN 5-TEXTURE ALLOCATOR MATRICES
    // ============================================================================
    // Forcefully pass the clean memory address pointer to the first index block element!
    glGenTextures(5, &g_textures[0]);

    // Core particle system masks loaded with dual-aware safety mapping fallbacks
    load_texture(get_active_asset_path("particle.png"), &g_textures[0]);
    load_texture(get_active_asset_path("lightmap.png"), &g_textures[1]);

    // Slot 2: Widescreen Walls
    if (settings->bgType == 1 && !settings->bgFile.empty()) {
        load_texture(settings->bgFile, &g_textures[2]);
    } else {
        load_texture(g_bg_path, &g_textures[2]);
    }

    // Slot 3: Ceiling Cap
    if (!g_custom_ceiling_path.empty() && file_exists_and_readable(g_custom_ceiling_path)) {
        load_texture(g_custom_ceiling_path, &g_textures[3]);
    } else {
        load_texture(get_active_asset_path("default_ceiling_1920x1080.png"), &g_textures[3]);
    }

    // Slot 4: Floor Cap
    if (!g_custom_floor_path.empty() && file_exists_and_readable(g_custom_floor_path)) {
        load_texture(g_custom_floor_path, &g_textures[4]);
    } else {
        load_texture(get_active_asset_path("default_floor_1920x1080.png"), &g_textures[4]);
    }

    // Aligning parameter variables to match class constructor layout signatures!
    g_scene = new CometenScene(&g_textures[0], get_elapsed_time(), boxWidth, boxHeight);

    // Register execution callbacks cleanly into GLUT
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);
    glutIdleFunc(idle_callback);

    // Trigger master loop matrix
    glutMainLoop();
    return 0;
}
