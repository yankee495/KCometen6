# Maintainer: John (Yankee)
pkgname=kcometen6
pkgver=1.0.2
pkgrel=1
pkgdesc="Standalone Wayland 3D particle screensaver & comet engine (KCometen4 revival)"
arch=('x86_64')
url="https://github.com/yankee495/KCometen6"
license=('GPL-3.0-or-later')
depends=('wayland' 'freeglut' 'python' 'python-pyqt6' 'python-dbus')
makedepends=('cmake' 'gcc' 'wayland-protocols' 'pkgconf' 'glu')

# --- AUTOMATED DUAL-AWARE SOURCE MAP ---
# Checks for your local tarball archive file first.
if [ -f "/${pkgname}-${pkgver}.tar.gz" ]; then
  # Local Track: Locks onto your development drive asset
  source=("${pkgname}-${pkgver}.tar.gz")
else
  # Public Track: Securely renames the web asset to match your script variables perfectly!
  source=("${pkgname}-${pkgver}.tar.gz::${url}/releases/download/v${pkgver}/${pkgname}-${pkgver}.tar.gz")
fi

sha256sums=('SKIP')

build() {
  # Dynamically detect if we are compiling your local folder or the unzipped web release
  if [ -d "$srcdir/${pkgname}_v${pkgver}" ]; then
    cd "$srcdir/${pkgname}_v${pkgver}"
  else
    cd "$srcdir/${pkgname}-${pkgver}"
  fi

  # 1. Compile the Main Engine Binaries out-of-source via CMake
  mkdir -p build
  cd build
  cmake ..
  make -j$(nproc)

  # 2. Compile custom leak-proof Wayland input-idle monitor daemon from src/
  g++ -O2 -Wall ../src/kcometen6_idle.cpp $(pkg-config --cflags --libs wayland-client) -o ../kcometen6_idle
}

package() {
  # Match the exact directory tracking map resolved during the build step
  if [ -d "$srcdir/${pkgname}_v${pkgver}" ]; then
    cd "$srcdir/${pkgname}_v${pkgver}"
  else
    cd "$srcdir/${pkgname}-${pkgver}"
  fi

  # 1. Deploy System Executables and Idle Daemons
  install -Dm755 build/kcometen6_run  "$pkgdir/usr/bin/kcometen6_run"
  install -Dm755 kcometen6_idle       "$pkgdir/usr/bin/kcometen6_idle"

  # 2. Deploy Python Configuration Dashboard GUI
  install -Dm755 KCometen6_Configure_GUI "$pkgdir/usr/bin/kcometen6-gui"

  # 3. Create Shared Directory Architecture and Deploy Static Textures
  install -dm755 "$pkgdir/usr/share/kcometen6"
  cp -r textures/* "$pkgdir/usr/share/kcometen6/"
  chmod 644 "$pkgdir/usr/share/kcometen6"/*

  # Deploy Master Parameters and Core System Icons
  install -Dm644 settings.conf      "$pkgdir/usr/share/kcometen6/settings.conf"
  install -Dm644 textures/kcometen6_icon.png "$pkgdir/usr/share/pixmaps/kcometen6_icon.png"

  # 4. Inject Native Desktop Application Launcher Profile Strings
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
