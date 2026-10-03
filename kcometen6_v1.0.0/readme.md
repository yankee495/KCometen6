# KCometen6 Standalone Portable Screen Sandbox (v1.0.0)

KCometen6 is a high-performance C++ modern revival of a classic 3D cosmic particle simulation engine, re-engineered for modern rolling-release **Wayland** desktops such as Arch, CachyOS and openSUSE Tumbleweed.  It has also been tested on Mageia 10. It functions as a completely standalone, directory-isolated visual engine that operates independently of system power management or restrictive desktop screensaver frameworks.

## ✨ Core Engineering Features
* **Native Wayland Input Tracking:** Built directly on the native `ext-idle-notify-v1` specifications, monitoring system idle securely at 0% idle CPU overhead.
* **Smart Lock-Screen Intercept Gating:** Automatically queries native desktop session flags via fast, non-blocking D-Bus connections, silently skipping window spawning if the machine is locked to preserve privacy.

* **Systemd Hardware Sleep Protection Shield:** Syncs with systemd-logind power management (`PreparingForSleep`). Automatically sweeps and kills graphics memory *before* a system suspend state occurs, completely eliminating post-wake visual frame flashes.

* **100% Standalone Portability:** Run-directory aware architecture mimicking classic portable application design. Build it once with a single script, move the resulting deployment folder anywhere, and let'er rip! If you want to uninstall it, just delete the single folder—leaving zero configuration clutter behind.


## 📦 System Build Dependencies
**Live Screenshot Feature Requires Spectacle**
Before running the automated compilation script, ensure your Linux system has the foundational development utilities, OpenGL windowing toolkits, and Wayland client packages installed:

- **Arch Linux / CachyOS:**

`sudo pacman -S base-devel cmake freeglut wayland wayland-protocols python-pyqt6 python-dbus spectacle`


- **openSUSE Tumbleweed:** 

`sudo zypper in patterns-devel-base-devel_basis cmake freeglut-devel wayland-devel wayland-protocols-devel python313-Pyqt6 python313-dbus-python spectacle`

- **Mageia 10:**

`sudo dnf install -y task-c++-devel cmake lib64glut-devel lib64wayland-client-devel wayland-protocols-devel python3-qt6 python3-dbus spectacle`

- **Fedora:**

`sudo dnf install -y gcc-c++ cmake freeglut-devel libwayland-client-devel wayland-protocols-devel python3-pyqt6 python3-dbus spectacle`


## 🛠️ Automated Compilation & Harvesting
To compile the entire 3D engine, link your custom input-idle daemon, and gather all high-performance assets into a single clean distribution folder, execute the central builder script from the root folder:

```bash
chmod +x build_and_deploy.sh
./build_and_deploy.sh
```

This will automatically assemble a standalone **`KCometen6`** deployment folder containing:
* `kcometen6_run` (Core 3D Engine Executable)
* `kcometen6_idle` (Native Wayland Input Tracker Daemon)
* `KCometen6_Configure_GUI` (PyQt6 Configuration Panel Dashboard)
* `integrate_menu.sh` (Dynamic KDE Application Menu Installer)
*  Default Images and configuration.


## 🎨 Widescreen Texture Guidelines
KCometen6 uses a true 16:9 3D cube environment grid mapping array. To ensure custom images display with absolute distortion-free aspect ratio perfection without edge-to-edge stretching or compressing of images, follow these dimension recommendations:
* **Side Walls:** Use your native widescreen monitor aspect ratio (e.g., `1920x1080`, `3840x2160`).  The default is a live screenshot of your desktop.
* **Ceiling & Floor Caps:** Use GIMP or Krita to crop your chosen 4K wallpaper into a true **1:1 square canvas** (e.g., `1920x1920` cropped or centered out of a 4K widescreen file). This keeps circular logos, text layout lines, and emblem paths flawlessly round and sharp edge-to-edge with zero stretching or compressing!

## 🚀 Recommended Configuration In Screensaver Mode
Open your `KCometen6_Configure_GUI` panel on the `Core Features` tab and check `Enable Idle Screensaver Mode`, then set your idle activation delay. Next, optionally set your system power settings into a seamless automation staircase:
1. **At 1 Minute:** KCometen6 launches natively to display your cosmic particle storms.
2. **At 2 Minutes:** System display screen blanking engages smoothly.
3. **At 3 Minutes:** The kernel enters hardware suspend/sleep.


## 🎨 Creative Credits & Asset Attributions
The premium high-fidelity 3D theme assets provided in this release showcase community craftsmanship:
* **Core 3D Engine:** This project is based on work by Peter Müller, who created the initial version for KDE3 under the name KCometen3. It was later ported and updated for KDE4 as KCometen4 by John Stamp. Ported to KDE Plasma 6 & Wayland as KCometen6 by John Smith.

* **Images:** The images used in the distro packs are credited to the original artists who graciously shared their work with the open-source community.
