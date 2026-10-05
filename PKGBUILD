# Maintainer: John (Yankee)
pkgname=kcometen6
pkgver=1.0.2
pkgrel=1
pkgdesc="Standalone Wayland 3D particle screensaver & comet engine (KCometen4 revival)"
arch=('x86_64')
url="https://github.com"
license=('GPL-3.0-or-later')
depends=('wayland' 'freeglut' 'python' 'python-pyqt6' 'python-dbus')
makedepends=('cmake' 'gcc' 'wayland-protocols' 'pkgconf' 'glu')

# --- DUAL-AWARE SOURCE RESOLVER (LOCAL DEV + PUBLIC AUR CO-EXISTENCE) ---
# Check if the release tarball exists in your active local working directory
if [ -f "./${pkgname}-${pkgver}.tar.gz" ]; then
  # Local Development Track: Use the file sitting right on your drive
  source=("${pkgname}-${pkgver}.tar.gz")
else
  # Public AUR Track: Securely stream the archive directly from the cloud
  source=("https://github.com/releases/download/v${pkgver}/${pkgname}-${pkgver}.tar.gz")
fi

sha256sums=('SKIP')

build() {
  # FIXED ARCHIVE DIRECTORY LOOKUP TARGETING FROM kcometen6-1.0.2 TO kcometen6_v1.0.2
  cd "$srcdir/kcometen6_v1.0.2"

  # 1. Compile the Main Engine Binaries via CMake
  mkdir -p build
  cd build
  cmake ..
  make -j$(nproc)

  # 2. Compile custom leak-proof Wayland input-idle monitor daemon from src/
  g++ -O2 -Wall ../src/kcometen6_idle.cpp $(pkg-config --cflags --libs wayland-client) -o kcometen6_idle
}

package() {
  # FIXED ARCHIVE DIRECTORY LOOKUP TARGETING FROM kcometen6-1.0.2 TO kcometen6_v1.0.2
  cd "$srcdir/kcometen6_v1.0.2"

  # 1. Deploy System Executables and Idle Daemons
  install -Dm755 build/kcometen6_run  "$pkgdir/usr/bin/kcometen6_run"
  install -Dm755 build/kcometen6_idle "$pkgdir/usr/bin/kcometen6_idle"

  # 2. Deploy Python Configuration Dashboard GUI
  install -Dm755 KCometen6_Configure_GUI "$pkgdir/usr/bin/kcometen6-gui"

  # 3. Create Shared Directory Architecture and Deploy Static Textures
  install -dm755 "$pkgdir/usr/share/kcometen6"
  cp -r textures/* "$pkgdir/usr/share/kcometen6/"
  chmod 644 "$pkgdir/usr/share/kcometen6"/*

  # Deploy Master Parameters and Core System Icons matching the Python theme hook down to the character!
  install -Dm644 settings.conf      "$pkgdir/usr/share/kcometen6/settings.conf"
  install -Dm644 textures/kcometen6_icon.png "$pkgdir/usr/share/pixmaps/kcometen6_icon.png"

  # 4. Inject Native Desktop Application Launcher Profile Strings (Cross-Desktop Compliant)
  install -Dm644 /dev/stdin "$pkgdir/usr/share/applications/kcometen6.desktop" << 'EOF'
[Desktop Entry]
Name=KCometen6 GUI
GenericName=3D Comet Engine Configurator
Comment=Configure the KCometen6 3D screensaver and particle engine
Exec=kcometen6-gui
Icon=kcometen6_icon
Terminal=false
Type=Application
Categories=Qt;KDE;Utility;
StartupNotify=true
PrefersNonDefaultGPU=false
X-KDE-SubstituteUID=false
EOF
}
