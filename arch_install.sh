#!/bin/bash
# ============================================================================
# KCometen6 Automated System Installer for Arch Linux & CachyOS (v1.0.3)
# ============================================================================
# arch_install.sh
#DEV_DIR="$(cd "$(dirname "$0")" && pwd)"
LAUNCH_DIR="$(pwd)"
PARENT_DIR="$(dirname "${LAUNCH_DIR}")"
STAGE_ROOT="/tmp/kcometen6_staging"

#echo "${LAUNCH_DIR}"
#echo "${PARENT_DIR}"
echo ""
echo ""
echo "============================================================================"
echo "🌌 KCometen6 Automated System Installer for Arch Linux & CachyOS (v1.0.3)"
echo "============================================================================"


# 1. Verify the script is being executed from your actual repository root folder
#if [ ! -d "./src" ] || [ ! -f "./PKGBUILD" ]; then
#  echo "❌ ERROR: Please execute this installer directly from the KCometen6 root folder!"
#  exit 1
#fi

echo "📦 Copying deployment assets up one folder layer..."
# 2. Replicate scripts and your PKGBUILD into the parent workspace folder
#cp "./arch_release.sh" "${PARENT_DIR}/"
#cp "./PKGBUILD" "${PARENT_DIR}/"
cp "./PKGBUILD" "${STAGE_ROOT}/"
# 3. Pivot the active terminal execution pane out to the parent folder space
#cd "${PARENT_DIR}"

echo "🗜️ Step 1: Running arch_release.sh to generate local tarball..."
# 4. Execute the release packager inside the parent folder layer
./arch_release.sh

echo "🛠️ Step 2: Compiling graphics engine system-wide via makepkg..."
# 5. Run makepkg to build the package from the parent folder path
#cd "${PARENT_DIR}"

cd "${STAGE_ROOT}"
makepkg -C -s -i -c


echo ""
echo "======================================================================="
echo "📦 Your tarball is located at ${STAGE_ROOT}"
echo "======================================================================="
echo ""
echo "======================================================================="
echo "🎉 SUCCESS! KCometen6 has been natively registered with Pacman."
echo "   Type 'kcometen6-gui' to launch your core configuration dashboard!"
echo "   Or find it in your KDE menu. It's also in /usr/bin/."
echo "======================================================================="

# 6. Return the user's terminal session context back safely to the root tree
cd "${LAUNCH_DIR}"
