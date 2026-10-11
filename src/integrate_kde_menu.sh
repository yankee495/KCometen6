#!/usr/bin/env bash
# ============================================================================
# KCOMETEN6 NATIVE DESKTOP MENU APPLICATION ENTRY INTEGRATOR
# Engineered by John (Yankee) — Dynamically routes your portable workspace paths
# ============================================================================

set -e

# 1. Grab the absolute running directory where this portable folder is planted
PORTABLE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAUNCHER_PATH="${PORTABLE_DIR}/KCometen6_Configure_GUI"
ICON_PATH="${PORTABLE_DIR}/kcometen6_icon.png"
TARGET_DESKTOP="${HOME}/.local/share/applications/kcometen6.desktop"

echo "[INIT] Initializing desktop app menu integration..."
echo "Detected Runtime Path: ${PORTABLE_DIR}"

# 2. Verify that the user has execution permissions set on their PyQt6 script file
chmod +x "${LAUNCHER_PATH}"

# 3. PROVISION THE NATIVE DESKTOP ENTRY METRICS BLOCK
cat << EOF > "${TARGET_DESKTOP}"
[Desktop Entry]
Comment[en_US]=
Categories=Qt;KDE;Utility;
Comment=
Exec=${LAUNCHER_PATH}
GenericName[en_US]=
GenericName=
Icon=${ICON_PATH}
MimeType=
Name[en_US]=KCometen6 GUI
Name=KCometen6 GUI
NoDisplay=false
Path=${PORTABLE_DIR}/
PrefersNonDefaultGPU=false
StartupNotify=true
Terminal=false
TerminalOptions=
Type=Application
X-KDE-SubstituteUID=false
X-KDE-Username=
EOF

# 4. FLUSH SYSTEM APPLICATION CACHE SO LINUX ADAPTS IMMEDIATELY
if command -v update-desktop-database &> /dev/null; then
    update-desktop-database ~/.local/share/applications/ &>/dev/null || true
fi

echo "===================================================================="
echo "  [DESKTOP MENU ENTRY CREATION SUCCESSFUL]"
echo "===================================================================="
echo "  KCometen6 has been installed to your application menu!"
echo "  You can now search for 'KCometen6 Config' inside your app launcher."
echo "===================================================================="
echo "===================================================================="
