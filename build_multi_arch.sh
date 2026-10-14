#!/bin/bash
# ============================================================================
# KCometen6 Automated Universal Multi-Architecture Compiler & Engine Forge
# ============================================================================
# build_and_deploy.sh

set -e

LAUNCH_DIR="$(cd "$(pwd)" && pwd)"
BUILD_DIR="${LAUNCH_DIR}/src/build"
PORTABLE_ROOT="${LAUNCH_DIR}/KCometen6"

echo ""
echo "=============================================================================="
echo "🌌 KCometen6 Universal Multi-Architecture Engine Builder"
echo "=============================================================================="
echo ""
echo "=============================================================================="
echo " [*] NOTICE: The active src/build folder will always cleanly match your"
echo "     native host hardware architecture."
echo ""
echo " [!] ATTENTION: This tracking behavior is bypassed if you explicitly"
echo "     select to build the standalone base x86_64 compatibility package only."
echo "=============================================================================="
echo ""

# 🔍 1. DETECT HIGHEST HOST CAPABILITY NATIVELY
echo "🔎 Querying system glibc capabilities..."
MAX_SUPPORTED=1

if /lib/ld-linux-x86-64.so.2 --help | grep -q "x86-64-v4 (supported, searched)"; then
    MAX_SUPPORTED=4
    echo "🚀 Hardware Capability Detected: x86-64-v4 (AVX-512 Complete)"
elif /lib/ld-linux-x86-64.so.2 --help | grep -q "x86-64-v3 (supported, searched)"; then
    MAX_SUPPORTED=3
    echo "🏎️  Hardware Capability Detected: x86-64-v3 (AVX2 / Modern Ryzen & Core)"
else
    echo "💻 Hardware Capability Detected: x86-64 Legacy Base"
fi
echo ""

# 📋 2. ENFORCED DYNAMIC MENU LOGIC BASED ON MACHINE POWER
BUILD_64=0
BUILD_V3=0
BUILD_V4=0

if [ "$MAX_SUPPORTED" -eq 4 ]; then
    echo "Select your compilation configuration profile:"
    echo "  1) Build Optimized x86_V4 only"
    echo "  2) Build Universal x86_64 + Optimized x86_V3 Dual-Pack"
    echo "  3) Build Complete Suite (x86_64 + x86_V3 + x86_V4 Triple-Pack)"
    echo ""
    read -p "Enter your choice (1-3) [Default: 3]: " USER_INPUT </dev/tty
    CHOICE=${USER_INPUT:-3}

    case "$CHOICE" in
        1) BUILD_V4=1 ;;
        2) BUILD_64=1; BUILD_V3=1 ;;
        *) BUILD_64=1; BUILD_V3=1; BUILD_V4=1 ;;
    esac

elif [ "$MAX_SUPPORTED" -eq 3 ]; then
    echo "Select your compilation configuration profile:"
    echo "  1) Build Optimized x86_V3 only"
    echo "  2) Build Universal x86_64 + Optimized x86_V3 Dual-Pack"
    echo ""
    read -p "Enter your choice (1-2) [Default: 2]: " USER_INPUT </dev/tty
    CHOICE=${USER_INPUT:-2}

    case "$CHOICE" in
        1) BUILD_V3=1 ;;
        *) BUILD_64=1; BUILD_V3=1 ;;
    esac

else
    echo "🌿 Host environment is restricted to base x86_64. Compiling universal fallback binary only..."
    BUILD_64=1
fi

echo ""
echo "========================================================================="
echo "🎉 Universal selected architecture standalone folders being generated!"
echo "========================================================================="
echo ""

# 🧹 3. WIPE AND INITIALIZE MAIN DEPLOYMENT ROOT
rm -rf "${PORTABLE_ROOT}"
mkdir -p "${PORTABLE_ROOT}"

# 💻 4. HARVEST AND SEED FLAT ENVIRONMENT BUNDLES
deploy_target_architecture() {
    local arch_label=$1
    local arch_flags=$2
    local folder_name=$3

    local TARGET_DIR="${PORTABLE_ROOT}/${folder_name}"

    echo ""
    echo "🔨 [BUILD] Initializing compilation loop for target architecture: ${arch_label}"

    # Reset internal build buffers completely to ensure native clean boundaries
    rm -rf "${BUILD_DIR}"
    mkdir -p "${BUILD_DIR}"
    cd "${BUILD_DIR}"

    # Compile the custom core 3D engine using the architecture flag
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="${arch_flags}" ../../src
    make -j$(nproc)

    # Build the flat isolated subfolder structure (NO nested texture folders!)
    mkdir -p "${TARGET_DIR}"

    # Deploy core compiled executable engine straight to the target folder
    cp "${BUILD_DIR}/kcometen6_run" "${TARGET_DIR}/"
    cp "${BUILD_DIR}/kcometen6_idle" "${TARGET_DIR}/"

    echo "📦 [HARVEST] Gathering supporting daemons, tools, and graphics textures into a flat layout..."

    # Replicate asset layouts and PyQt6 management configurations into the flat folder layout
    cp "${LAUNCH_DIR}/src/KCometen6_Configure_GUI" "${TARGET_DIR}/"
    cp "${LAUNCH_DIR}/src/settings.conf" "${TARGET_DIR}/"

    # 🧮 FLAT HARVEST FIX: Dumps PNG textures right into the flat root of the architecture folder!
    cp "${LAUNCH_DIR}/src"/*.png "${TARGET_DIR}/"
    cp "${LAUNCH_DIR}/README.md" "${TARGET_DIR}/" 2>/dev/null || true

    # Make everything in the folder cleanly executable
    chmod +x "${TARGET_DIR}/kcometen6_run"
    chmod +x "${TARGET_DIR}/KCometen6_Configure_GUI"
    chmod +x "${TARGET_DIR}/kcometen6_idle"

    echo "✅ Successfully completed portable standalone bundle folder: ${folder_name}"
}

# 🚀 5. EXECUTE STAIRCASE: COMPILING NON-NATIVE ARCHITECTURES FIRST, NATIVE HOST LAST!

# Pass 1: Generic compliance (Built first)
# AFTER:
if [ "$BUILD_64" -eq 1 ]; then
    deploy_target_architecture "x86-64 Base Generic" "-march=x86-64" "KCometen6_x86_64"

echo ""
echo "=============================================================================="
echo "🌌 KCometen6 Universal Selected Architecture x86_64 Engine Built"
echo "   Location: ${PORTABLE_ROOT}"
echo "   Your active build workspace folder matches your host machine!"
echo "=============================================================================="
echo ""

fi

# Pass 2: Native V3 optimization (Built last for V3 users leaving your active build cache perfectly synced!)
if [ "$BUILD_V3" -eq 1 ]; then
    deploy_target_architecture "x86-64-v3 (AVX2/BMI/FMA)" "-march=x86-64-v3 -O3" "KCometen6_x86_V3"

echo ""
echo "=============================================================================="
echo "🌌 KCometen6 Universal Selected Architecture x86-64-v3 Engine Built"
echo ""
echo " [*] NOTICE: The active src/build folder will always cleanly match your"
echo "     native host hardware architecture."
echo ""
echo " [!] ATTENTION: This tracking behavior is bypassed if you explicitly"
echo "     select to build the standalone base x86_64 compatibility package only."
echo "=============================================================================="
echo ""

fi

# Pass 3: Extreme V4 optimization (Built last for V4 users leaving your active build cache perfectly synced!)
if [ "$BUILD_V4" -eq 1 ]; then
    deploy_target_architecture "x86-64-v4 (AVX-512)" "-march=x86-64-v4 -O3" "KCometen6_x86_V4"

echo ""
echo "=============================================================================="
echo "🌌 KCometen6 Universal Selected Architecture x86-64-v4 Engine Built"
echo "=============================================================================="
echo ""

fi

echo ""
echo "=============================================================================="
echo "🎉 SUCCESS: Universal Selected architecture portable folders generated!"
echo "   Location: ${PORTABLE_ROOT}"
echo "   Your active build workspace folder matches your host machine!"
echo "=============================================================================="
cd "${LAUNCH_DIR}"
