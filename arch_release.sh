#!/bin/bash
# ============================================================================
# KCometen6 Automated Release Staging Script (v1.0.2)
# ============================================================================
DEV_DIR="/home/john/code-test/kcometen6_v1.0.2"
STAGE_ROOT="/tmp/kcometen6_staging"
STAGE_DIR="${STAGE_ROOT}/kcometen6_v1.0.2"
VERSION="1.0.2"

echo "🧹 Clearing previous staging buffers..."
rm -rf "${STAGE_ROOT}"
mkdir -p "${STAGE_DIR}"

echo "📦 Harvesting bare source components and separating texture layers..."
# 1. Create a clean split directory layout inside the public staging sandbox
mkdir -p "${STAGE_DIR}/src"
mkdir -p "${STAGE_DIR}/textures"

# 2. Copy the active C++ engine source files into the staging src folder
cp "${DEV_DIR}/src"/*.cpp "${STAGE_DIR}/src/"
cp "${DEV_DIR}/src"/*.h "${STAGE_DIR}/src/"

# 3. Separate your 4K image assets out into a dedicated public textures folder
cp "${DEV_DIR}/src"/*.png "${STAGE_DIR}/textures/"

# 4. Pull your core build parameters and tools up to the root staging layer
cp "${DEV_DIR}/src/CMakeLists.txt" "${STAGE_DIR}/"
cp "${DEV_DIR}/src/KCometen6_Configure_GUI" "${STAGE_DIR}/"
cp "${DEV_DIR}/src/settings.conf" "${STAGE_DIR}/"

# 5. Bring your top-level project metadata along
cp "${DEV_DIR}/README.md" "${STAGE_DIR}/"
cp "${DEV_DIR}/AUTHORS" "${STAGE_DIR}/" 2>/dev/null || true
cp "${DEV_DIR}/ChangeLog" "${STAGE_DIR}/" 2>/dev/null || true

echo "🗑️ Filtering legacy files out of the release bundle..."
# Delete old inactive files from the public snapshot so it stays lean
rm -f "${STAGE_DIR}/src/kcometen4."*
rm -f "${STAGE_DIR}/src/kglcometen4."*
rm -f "${STAGE_DIR}/src/settings.cpp"
rm -f "${STAGE_DIR}/src/integrate_menu.sh"
rm -f "${STAGE_DIR}/textures/kcometen4"*

echo "🗜️ Compressing clean source archive into kcometen6-${VERSION}.tar.gz..."
cd "${STAGE_ROOT}"
tar -czf "${DEV_DIR}/kcometen6-${VERSION}.tar.gz" kcometen6_v1.0.2/

echo "🚀 SUCCESS! Pristine release tarball is ready at: ${DEV_DIR}/kcometen6-${VERSION}.tar.gz"
