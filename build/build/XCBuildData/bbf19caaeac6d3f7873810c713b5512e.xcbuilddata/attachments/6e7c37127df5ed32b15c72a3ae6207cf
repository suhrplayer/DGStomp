#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_Standalone/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/Standalone/DGStomp.app/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/Standalone/DGStomp.app/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Debug/Standalone/DGStomp.app/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_Standalone/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/Standalone/DGStomp.app/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/Standalone/DGStomp.app/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/Release/Standalone/DGStomp.app/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_Standalone/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/Standalone/DGStomp.app/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/Standalone/DGStomp.app/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/MinSizeRel/Standalone/DGStomp.app/Contents/Resources/DGStompUI.js
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E copy /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/JuceLibraryCode/DGStomp_Standalone/PkgInfo /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/Standalone/DGStomp.app/Contents
  cd /Users/jamesmcfarland/Downloads/DGStomp/build
  /opt/homebrew/bin/cmake -E make_directory /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/Standalone/DGStomp.app/Contents/Resources
  /opt/homebrew/bin/cmake -E copy_if_different /Users/jamesmcfarland/Downloads/DGStomp/Patch/DGStompUI.js /Users/jamesmcfarland/Downloads/DGStomp/build/DGStomp_artefacts/RelWithDebInfo/Standalone/DGStomp.app/Contents/Resources/DGStompUI.js
fi

