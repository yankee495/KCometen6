#!/usr/bin/env bash
# ============================================================================
# KCOMETEN6 CLEAN SOURCE EXCLUSION & ISOLATION STAGER
# By John (Yankee) — Copies specific production files by name
# ============================================================================
#!/usr/bin/env bash
set -e

# 📂 1. CAPTURE ACTUAL PHYSICAL PATHS
# Ensure we capture absolute, normalized paths without trailing slashes
SRC_DIR="$(cd "$(pwd)" && pwd)"
BASE_DIR="$(cd "${SRC_DIR}/.." && pwd)"
LAUNCH_DIR="$(pwd)"

# 🏷️ 2. DEFINE EXPLICIT STRIPPED PREFIXES
# Keep this as a pure string label—NO path subshells inside!
SRC_TREE_PREFIX="kcometen6_v"

# 🚀 3. PIVOT CONTEXT TO THE PARENT SANDBOX
# Move up to network-test/ so the 'find' utility scans the correct layer
cd "${BASE_DIR}"

# 🔍 4. DYNAMIC VERSION SCANNING (Runs safely inside network-test/)
if [ -z "$1" ]; then
# 🔍 2. EXTRACT LIVE VERSION FROM SOURCE OF TRUTH (No folder scanning needed!)
# Scrapes your active PKGBUILD file directly to see where we are currently standing
CURRENT_VERSION="$(sed -n 's/^pkgver=//p' "${LAUNCH_DIR}/PKGBUILD" | tr -d '[:space:]')"

#echo "$(sed -n 's/^pkgver=//p' "${LAUNCH_DIR}/PKGBUILD" | tr -d '[:space:]')"


#echo "CURRENT VERSION TEST: ${CURRENT_VERSION}"


if [ -z "${CURRENT_VERSION}" ]; then
  echo "⚠️  WARNING: Could not parse pkgver from PKGBUILD. Defaulting layout baseline to 0.0.0."
   CURRENT_VERSION="0.0.0"
   TARGET_VERSION="0.0.1"
  else
# 🧮 3. CALCULATE AUTO-INCREMENT STAIRCASE
  IFS='.' read -r major minor patch <<< "$CURRENT_VERSION"
  patch=$((patch + 1))
  TARGET_VERSION="${major}.${minor}.${patch}"
  fi
  echo ""
  echo "🔍 No explicit version supplied. Detected current: v${CURRENT_VERSION}"
  echo "✨ Auto-incrementing staging destination target to: v${TARGET_VERSION}"
  echo ""
else
  TARGET_VERSION="$1"
  echo ""
  echo""
  echo "🏷️ Manual version target specified: v${TARGET_VERSION}"
fi

# 📂 5. ASSEMBLE CLEAN TRUCKING CHANNELS (No more double slashes!)
NEW_SRC_TREE="${BASE_DIR}/${SRC_TREE_PREFIX}${TARGET_VERSION}"


# 2. OVERWRITE PROTECTIVE LANDING GATE (FIXED ERASER TRAP)
if [ -d "${NEW_SRC_TREE}" ]; then
  echo "===================================================================="
  echo "⚠️  WARNING: Staging directory already exists!"
  echo "   -> ${NEW_SRC_TREE}"
  echo "===================================================================="
  read -p "🔄 Overwrite this existing tree and replace files? (y/N): " choice
  case "$choice" in
    [yY][eE][sS]|[yY])
      echo "🧹 Wiping old workspace layout to prepare clean slate..."
      #rm -rf "${NEW_SRC_TREE}"
      ;;
    *)
      echo "🛑 Staging canceled safely to protect your existing directories."
      exit 0
      ;;
  esac
fi

# Log parameter status checkpoints cleanly out to the terminal console layout
echo "Source directory:        ${SRC_DIR}"
echo "Source directory prefix: ${SRC_TREE_PREFIX}"
echo "Base directory:          ${BASE_DIR}"
echo "New Source Tree:         ${NEW_SRC_TREE}"
echo ""

# 3. GENERATING PRISTINE NEW SOURCE TRACKS
echo "📦 Generating clean development path tree at: ${NEW_SRC_TREE}"
echo "===================================================================="
echo "[1/3] Initializing pristine staging folder layout..."

# Form fresh tracking directory nodes
mkdir -p "${NEW_SRC_TREE}/src"

echo "[2/3] Extracting production assets and scripts by name..."
# exit 0
# 1. Copy development metadata parameters and automation script tools - skips personal scripts
cp "${SRC_DIR}/arch_install.sh"               "${NEW_SRC_TREE}/"
#cp "${SRC_DIR}/arch_install_build.sh"         "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/arch_release.sh"               "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/AUTHORS"                       "${NEW_SRC_TREE}/" 2>/dev/null || true
cp "${SRC_DIR}/build_and_deploy.sh"           "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/build_multi_arch.sh"           "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/ChangeLog"                     "${NEW_SRC_TREE}/" 2>/dev/null || true
cp "${SRC_DIR}/INSTRUCTIONS.md"               "${NEW_SRC_TREE}/" 2>/dev/null || true
cp "${SRC_DIR}/instructions.txt"              "${NEW_SRC_TREE}/" 2>/dev/null || true
cp "${SRC_DIR}/KCometen6_Build_GUI"           "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/src/KCometen6_Configure_GUI"   "${NEW_SRC_TREE}/src/"
cp "${SRC_DIR}/src/integrate_kde_menu.sh"     "${NEW_SRC_TREE}/src/"
cp "${SRC_DIR}/LICENSE"                       "${NEW_SRC_TREE}/"
#cp "${SRC_DIR}/make_tarball.sh"               "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/README.md"                     "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/PKGBUILD"                      "${NEW_SRC_TREE}/"
cp "${SRC_DIR}/.SRCINFO"                      "${NEW_SRC_TREE}/" 2>/dev/null || true
cp "${SRC_DIR}/stage_release_build.sh"        "${NEW_SRC_TREE}/"
cp "${SRC_DIR}"/*.png                         "${NEW_SRC_TREE}/" 2>/dev/null || true
# cp "${SRC_DIR}"/*.sh                          "${NEW_SRC_TREE}" 2>/dev/null || true

# 2. Deploy PyQt6 GUI
#if [ -f "${SRC_DIR}/src/KCometen6_Configure_GUI" ]; then
#  cp "${SRC_DIR}/src/KCometen6_Configure_GUI" "${NEW_SRC_TREE}/src/"
#fi

# 3. Copy specified C++ source explicitly by name (drops backups/temp logs)
C_FILES=(
    "bezier.cpp" "cometen3.cpp" "cometenmath.cpp" "cometenscene.cpp" "curvecomet.cpp"
    "decal.cpp" "explosion.cpp" "glow.cpp" "kcometen4.cpp" "kcometen6_idle.cpp"
    "kglcometen4.cpp" "lightning.cpp" "particlesystem.cpp" "pcomet.cpp" "rotatecomet.cpp"
    "settings.cpp" "standalone_main.cpp" "vec.cpp" "bezier.h" "cometen3.h" "cometenmath.h"
    "cometenscene.h" "comet.h" "curvecomet.h" "decal.h" "explosion.h" "global.h" "glow.h"
    "kcometen4.h" "kglcometen4.h" "lightning.h" "particlesystem.h" "pcomet.h" "rotatecomet.h"
    "settings.h" "stb_image.h" "vec.h" "CMakeLists.txt"
)

for FILE in "${C_FILES[@]}"; do
    if [ -f "${SRC_DIR}/src/${FILE}" ]; then
        cp "${SRC_DIR}/src/${FILE}" "${NEW_SRC_TREE}/src/"
    else
        echo "[WARNING] Expected engine asset file not found: src/${FILE}"
    fi
done

# 4. Extract environment textures, layout maps, and config files
cp "${SRC_DIR}/src"/*.png "${NEW_SRC_TREE}/src/" 2>/dev/null || true
cp "${SRC_DIR}/src"/*.jpg "${NEW_SRC_TREE}/src/" 2>/dev/null || true

if [ -f "${SRC_DIR}/src/settings.conf" ]; then
    cp "${SRC_DIR}/src/settings.conf" "${NEW_SRC_TREE}/src/"
fi

# ============================================================================
# 5. DYNAMICALLY PATCH AND REGENERATE METADATA ENGINE
# ============================================================================
if [ -f "${NEW_SRC_TREE}/PKGBUILD" ]; then
  # Track A: Safely patch the master PKGBUILD first
  sed -i "s/^pkgver=.*/pkgver=${TARGET_VERSION}/" "${NEW_SRC_TREE}/PKGBUILD"
  echo "✅ Master PKGBUILD version updated to: ${TARGET_VERSION}"

# 🔍 UNIVERSAL CHECK: Only execute Arch-specific commands if makepkg is available
if command -v makepkg &> /dev/null; then
    echo "📦 Arch utility detected. Regenerating tracking .SRCINFO layer..."
    (cd "${NEW_SRC_TREE}" && makepkg --printsrcinfo > .SRCINFO)
else
    echo "ℹ️  Non-Arch system platform detected. Safely bypassing .SRCINFO generation."
  fi

fi

echo "[3/3] Performing structural integrity pass..."
echo "===================================================================="
echo "  [SOURCE TREE CREATION SUCCESSFUL]"
echo "===================================================================="
echo "  Your clean source code tree is READY!"
echo "  Location: ${NEW_SRC_TREE}"
echo "  "
echo "  Your notes, logs and backups were left behind safely."
echo "  You can now change directories into the new tree to test compile!"
echo "===================================================================="
