#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"
mkdir -p build
cmake -G Xcode \
    -B build \
    -S . \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
echo ""
echo "✓ DGStomp Xcode project generated in: $SCRIPT_DIR/build"
echo ""
echo "Build commands:"
echo "  cmake --build build --config Debug   --target DGStomp_Standalone"
echo "  cmake --build build --config Release --target DGStomp_VST3"
echo "  cmake --build build --config Release --target DGStomp_AU"
echo "  sudo cmake --install build --config Release"
