#!/bin/bash
# ============================================================================
# KCometen6 Automated Universal Release Staging Script (Root Folder Track)
# ============================================================================
# arch_release.sh

set -e

# 📂 1. LOCK ONTO THE ACTUAL WORKING REPOSITORY ROOT
LAUNCH_DIR="$(pwd)"
DEV_DIR="${LAUNCH_DIR}"

CURRENT_FOLDER="$(basename "${LAUNCH_DIR}")"

# 📂 1. RESOLVE ABSOLUTE DEVELOPMENT CODE ROOT BOUNDARY
# If $1 is passed by arch_install.sh, use that absolute path directly.
# Otherwise, assume the script was run manually from inside the repository root.
if [ -n "$1" ]; then
    DEV_DIR="$1"
else
    DEV_DIR="$(cd "$(dirname "$0")" && pwd)"
fi

echo ""
echo "===================================================================="
echo "🚀 Arch Release Active Targets Linked"
echo "===================================================================="
echo "Arch Release DEV_DIR: ${DEV_DIR}"
echo "Launch Directory:     ${LAUNCH_DIR}"
echo "===================================================================="
echo ""

# Wherever you execute this script is where your code lives!
#DEV_DIR="$(cd "$(pwd)" && pwd)"
TREE_DIR="$(cd "$(pwd)" && pwd)"
TARGET_FOLDER="${1:-KCometen6}"
#DEV_DIR="${PARENT_DIR}/${TARGET_FOLDER}"


echo ""
echo "Arch Release DEV_DIR: ${DEV_DIR}"
echo ""
echo "Launch Directory: ${LAUNCH_DIR}"
echo ""
echo "Current Folder: ${CURRENT_FOLDER}"
echo ""
echo ""
echo ""


# Extract the version number dynamically from your local PKGBUILD string
TARGET_VERSION="$(sed -n 's/^pkgver=//p' "${DEV_DIR}/PKGBUILD" | tr -d '[:space:]')"
VERSION="${TARGET_VERSION}"

SRC_TREE_PREFIX="kcometen6_v"
TAR_BALL="${SRC_TREE_PREFIX}${VERSION}"

STAGE_ROOT="/tmp/kcometen6_staging"
STAGE_DIR="${STAGE_ROOT}/${TAR_BALL}"

#DEV_DIR="${TREE_DIR}/${SRC_TREE_PREFIX}${VERSION}"   --- ${LAUNCH_DIR}/${DEV_DIR}
#DEV_DIR="${LAUNCH_DIR}/${DEV_DIR}"

echo "🧹 Clearing previous staging buffers..."
rm -rf "${STAGE_ROOT}"
mkdir -p "${STAGE_DIR}/src"
mkdir -p "${STAGE_DIR}/textures"

echo "📦 Harvesting bare source components from Root: ${DEV_DIR}"
cp "${DEV_DIR}/PKGBUILD" "${STAGE_DIR}/"
cp "${DEV_DIR}/PKGBUILD" "${STAGE_ROOT}/"
cp "${DEV_DIR}/src"/*.cpp "${STAGE_DIR}/src/"
cp "${DEV_DIR}/src"/*.h "${STAGE_DIR}/src/"
cp "${DEV_DIR}/src"/*.png "${STAGE_DIR}/textures/"
cp "${DEV_DIR}/src/CMakeLists.txt" "${STAGE_DIR}/"
cp "${DEV_DIR}/src/KCometen6_Configure_GUI" "${STAGE_DIR}/"
cp "${DEV_DIR}/src/settings.conf" "${STAGE_DIR}/"
cp "${DEV_DIR}/README.md" "${STAGE_DIR}/"

echo "🗑️ Filtering legacy files out of the release bundle..."
rm -f "${STAGE_DIR}/src/kcometen4."*
rm -f "${STAGE_DIR}/src/kglcometen4."*
rm -f "${STAGE_DIR}/src/settings.cpp"
rm -f "${STAGE_DIR}/src/integrate_menu.sh"
rm -f "${STAGE_DIR}/textures/kcometen4"*

echo "🗜️ Compressing clean source archive into kcometen6-${VERSION}.tar.gz..."
cd "${STAGE_ROOT}"

# Compresses your pristine staging sandbox and drops the tarball straight out
# to the parent layer so makepkg can grab it natively!
#tar -czf "$(dirname "${DEV_DIR}")/kcometen6-${VERSION}.tar.gz" "${TAR_BALL}"

tar -czf "${STAGE_ROOT}/kcometen6-${VERSION}.tar.gz" "${TAR_BALL}"

echo ""
echo "===================================================================="
echo "🚀 SUCCESS! Pristine release tarball is ready for makepkg!"
echo "===================================================================="
echo "📦 Your tarball is located at ${STAGE_ROOT}"
echo "===================================================================="
echo ""

