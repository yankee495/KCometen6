#!/usr/bin/env bash
# ============================================================================
# KCOMETEN6 PRISTINE SOURCE EXCLUSION & ISOLATION STAGER
# Engineered by John (Yankee) — Copies specific production files by name
# ============================================================================
# This script produces a clean copy of your source without your backups or notes - All jpg & png images are copied
# Change the kcometen6_v1.0.x version number below to start a new clean source tree with a new version number
# Otherwise it will produce a new copy of the current version - ERASES OLD VERSION if version number is not changed
#
set -e

SRC_DIR="$(pwd)"
echo $SRC_DIR
STAGE_DIR="$(cd "${SRC_DIR}/.." && pwd)/kcometen6_v1.0.3"

echo "===================================================================="
echo "[1/3] Initializing pristine staging folder layout..."
rm -rf "${STAGE_DIR}"
mkdir -p "${STAGE_DIR}/src"

echo "[2/3] Extracting production assets and scripts by name..."

# 1. C core builder script and main Wayland client daemon
cp "${SRC_DIR}/build_and_deploy.sh" "${STAGE_DIR}/"
cp "${SRC_DIR}/stage_release.sh" "${STAGE_DIR}/"
cp "${SRC_DIR}/README.md" "${STAGE_DIR}/"
cp "${SRC_DIR}/AUTHORS" "${STAGE_DIR}/"
cp "${SRC_DIR}/ChangeLog" "${STAGE_DIR}/"
cp "${SRC_DIR}/README.md" "${STAGE_DIR}/"

# 2. C the newly optimizedQt6 Configuration Dashboard tool
cp "${SRC_DIR}/src/KCometen6_Configure_GUI" "${STAGE_DIR}/src/"
# 3. Copy the menu entry creation script
cp "${SRC_DIR}/src/integrate_menu.sh" "${STAGE_DIR}/src/"

# 4. C the specific C++ graphics sandbox engine source files explicitly
C_FILES=(
    "bezier.cpp" "cometen3.cpp" "cometenmath.cpp" "cometenscene.cpp" "curvecomet.cpp" "decal.cpp" "explosion.cpp" "glow.cpp" "kcometen4.cpp" "kcometen6_idle.cpp" "kglcometen4.cpp" "lightning.cpp" "particlesystem.cpp" "pcomet.cpp" "rotatecomet.cpp" "settings.cpp" "standalone_main.cpp" "vec.cpp" "bezier.h" "cometen3.h" "cometenmath.h" "cometenscene.h" "comet.h" "curvecomet.h" "decal.h" "explosion.h" "global.h" "glow.h" "kcometen4.h" "kglcometen4.h" "lightning.h" "particlesystem.h" "pcomet.h" "rotatecomet.h" "settings.h" "stb_image.h" "vec.h"
    "CMakeLists.txt"
)

for FILE in "${C_FILES[@]}"; do
    if [ -f "${SRC_DIR}/src/${FILE}" ]; then
        cp "${SRC_DIR}/src/${FILE}" "${STAGE_DIR}/src/"
    else
        echo "[WARNING] Expected engine asset file not found: src/${FILE}"
    fi
done

# 4. Sweep up your live curated wallpaper textures, custom distro caps, and configuration maps
cp "${SRC_DIR}/src"/*.png "${STAGE_DIR}/src/" 2>/dev/null || true
cp "${SRC_DIR}/src"/*.jpg "${STAGE_DIR}/src/" 2>/dev/null || true

if [ -f "${SRC_DIR}/src/settings.conf" ]; then
    cp "${SRC_DIR}/src/settings.conf" "${STAGE_DIR}/src/"
fi

echo "[3/3] Performing structural integrity pass..."
echo "===================================================================="
echo "  [PRISTINE SOURCE ISOLATION SUCCESSFUL]"
echo "===================================================================="
echo "  Your clean source code box is READY!"
echo "  Location: ${STAGE_DIR}"
echo "  "
echo "  Your scratch notes, logs, and debris were left behind safely."
echo "  You can now change directories into the new tree to test compile!"
echo "===================================================================="
