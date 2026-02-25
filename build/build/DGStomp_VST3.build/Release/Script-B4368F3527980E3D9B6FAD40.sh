#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_VST3/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -Dsrc=/Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3 -P /Users/jamesmcfarland/SDKs/JUCE/extras/Build/CMake/checkBundleSigning.cmake
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo removing\ moduleinfo.json
  /opt/homebrew/bin/cmake -E remove -f /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/moduleinfo.json /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo creating\ /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/Resources
  /Users/jamesmcfarland/Downloads/DGStomp/build/Debug/DGStomp_vst3_helper > /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/VST3/DGStomp.vst3/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_VST3/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -Dsrc=/Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3 -P /Users/jamesmcfarland/SDKs/JUCE/extras/Build/CMake/checkBundleSigning.cmake
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo removing\ moduleinfo.json
  /opt/homebrew/bin/cmake -E remove -f /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/moduleinfo.json /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo creating\ /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/Resources
  /Users/jamesmcfarland/Downloads/DGStomp/build/Release/DGStomp_vst3_helper > /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/VST3/DGStomp.vst3/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_VST3/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -Dsrc=/Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3 -P /Users/jamesmcfarland/SDKs/JUCE/extras/Build/CMake/checkBundleSigning.cmake
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo removing\ moduleinfo.json
  /opt/homebrew/bin/cmake -E remove -f /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/moduleinfo.json /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo creating\ /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/Resources
  /Users/jamesmcfarland/Downloads/DGStomp/build/MinSizeRel/DGStomp_vst3_helper > /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/VST3/DGStomp.vst3/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_VST3/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -Dsrc=/Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3 -P /Users/jamesmcfarland/SDKs/JUCE/extras/Build/CMake/checkBundleSigning.cmake
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo removing\ moduleinfo.json
  /opt/homebrew/bin/cmake -E remove -f /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/moduleinfo.json /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E echo creating\ /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/Resources
  /Users/jamesmcfarland/Downloads/DGStomp/build/RelWithDebInfo/DGStomp_vst3_helper > /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/Resources/moduleinfo.json
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/VST3/DGStomp.vst3/Contents/Resources/DGStompUI.js
fi

