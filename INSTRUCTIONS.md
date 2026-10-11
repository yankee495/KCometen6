# 🌌 KCometen6 Architecture Execution Guide

This guide details how to build KCometen6 as a standalone portable application folder, or install it natively as a system-wide desktop utility.

## 🚀 The Portable Route (Any Linux Distribution)

Runs as an portable application folder. It does not touch system roots and uninstalls cleanly by deleting the folder.

* **`./build_and_deploy.sh`**
  * Run this directly inside the inner source directory.
  * Compiles, links, and harvests all graphics assets into a standalone `KCometen6` folder.

## 💻 The Native System Route (Arch Linux & CachyOS)

Integrates the 3D physics engine directly into standard system binaries and adds a desktop launcher in KDE's menu under Utilities.

* **`./arch_install.sh`**
  * Run this from your master repository root folder.
  * Automates asset copying, packages local tarballs, and installs system-wide via pacman entirely hands-free.
* **`makepkg -Csic`**
  * The manual packaging command.
  * Cleans the build path, auto-resolves dependencies, installs the desktop package, and wipes the temporary build cache.

## 🧼 System Uninstallation

* **`sudo pacman -R kcometen6`**
  * Safely strips away system executables, menu profiles, and assets without touching core Qt packages.
