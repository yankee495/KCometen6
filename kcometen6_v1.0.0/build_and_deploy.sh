#!/usr/bin/env bash
# ============================================================================
# KCOMETEN6 CENTRAL AUTOMATED COMPILATION & PORTABLE RELEASE SUITE
# Engineered by John (Yankee) — Tailored for your build sandbox directory tree
# ============================================================================

set -e # Exit instantly if any compilation step encounters an error

echo "===================================================================="
echo "[1/4] Setting up clean release folders and wiping old deployments..."

SRC_DIR="$(pwd)"
BUILD_DIR="${SRC_DIR}/src/build"
DEPLOY_DIR="${SRC_DIR}/KCometen6"

# Ensure target directories exist and clear old packages safely
mkdir -p "${BUILD_DIR}"
rm -rf "${DEPLOY_DIR}"
mkdir -p "${DEPLOY_DIR}"

echo "[2/4] Executing core 3D particle graphics engine compilation pass..."
# Jumps directly into your build directory to run your exact working routine!
cd "${BUILD_DIR}"

# Run your verified cleanup array logic to clear stale recipes
FILES_TO_DELETE=("CMakeCache.txt" "CMakeFiles" "cmake_install.cmake" "files.txt" "kcometen6_run" "Makefile")
for ITEM in "${FILES_TO_DELETE[@]}"; do
    if [ -e "$ITEM" ]; then
        rm -rf "$ITEM"
    fi
done

# Run your native, working CMake build commands
cmake ..
make -j$(nproc)

echo "[3/4] Compiling custom leak-proof Wayland input-idle monitor daemon..."
# Automatically injects Tumbleweed's specific include path paths on the fly!
g++ -O2 -Wall "${SRC_DIR}/src/kcometen6_idle.cpp" $(pkg-config --cflags --libs wayland-client) -o "${BUILD_DIR}/kcometen6_idle"

echo "[4/4] Harvesting portable dashboard scripts and asset texture layers..."
# First, populate your active build directory with assets so it never runs bare!
cp "${SRC_DIR}/src"/*.png "${BUILD_DIR}/" 2>/dev/null || true
cp "${SRC_DIR}/src"/*.jpg "${BUILD_DIR}/" 2>/dev/null || true
cp "${SRC_DIR}/src/KCometen6_Configure_GUI" "${BUILD_DIR}/" 2>/dev/null || true

# Next, assemble your standalone portable folder
cp "${BUILD_DIR}/kcometen6_run" "${DEPLOY_DIR}/"
cp "${BUILD_DIR}/kcometen6_idle" "${DEPLOY_DIR}/"

# C your configuration dashboard panel to portable folder
cp "${SRC_DIR}/src/KCometen6_Configure_GUI" "${DEPLOY_DIR}/KCometen6_Configure_GUI"
chmod +x "${DEPLOY_DIR}/KCometen6_Configure_GUI"

# Sweep up your active textures, lightning wallpapers, and configurations into the final folder
cp "${SRC_DIR}/src"/*.png "${DEPLOY_DIR}/" 2>/dev/null || true
cp "${SRC_DIR}/src"/*.jpg "${DEPLOY_DIR}/" 2>/dev/null || true
cp "${SRC_DIR}/src/settings.conf" "${DEPLOY_DIR}/" 2>/dev/null || true
# C the menu item creation script to the portable folder
cp "${SRC_DIR}/src/integrate_menu.sh" "${DEPLOY_DIR}/" 2>/dev/null || true
chmod +x "${DEPLOY_DIR}/integrate_menu.sh"

echo "  "
echo "  "
echo "  "
echo "===================================================================="
echo "  [CREATING PORTABLE FOLDER SUCCESSFUL]"
echo "===================================================================="
echo "  Your clean, standalone KCometen6 folder is READY!"
echo "  Location: ${DEPLOY_DIR}"
echo "  "
echo "  Simply copy or move the 'KCometen6' folder out of your project tree"
echo "  and drop it anywhere on your machine—it is completely self-contained!"
echo "  "
echo "  Run integrate_menu.sh from that folder only to create a KDE menu entry for the"
echo "  KCometen6 configuration panel."
echo "===================================================================="

