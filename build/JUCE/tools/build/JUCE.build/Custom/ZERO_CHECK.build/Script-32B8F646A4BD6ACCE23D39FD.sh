#!/bin/sh
set -e
if test "$CONFIGURATION" = "Custom"; then :
  cd /Users/jamesmcfarland/Downloads/DGStomp/build/JUCE/tools
  make -f /Users/jamesmcfarland/Downloads/DGStomp/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi

