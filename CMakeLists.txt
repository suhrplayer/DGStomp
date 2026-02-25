cmake_minimum_required(VERSION 3.22)
project(DGStomp VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS version")
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Universal binary")

set(JUCE_PATH "$ENV{HOME}/SDKs/JUCE" CACHE PATH "Path to JUCE root")
if(NOT EXISTS "${JUCE_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE not found at: ${JUCE_PATH}")
endif()
add_subdirectory(${JUCE_PATH} JUCE EXCLUDE_FROM_ALL)

juce_add_plugin(DGStomp
    COMPANY_NAME            "JAMAHA"
    BUNDLE_ID               "com.jamaha.dgstomp"
    PLUGIN_NAME             "DG Stomp"
    PLUGIN_DESCRIPTION      "Yamaha DG Stomp Amp Sim — Allan Holdsworth Edition"
    PLUGIN_MANUFACTURER_CODE Jmha
    PLUGIN_CODE             DgSt
    FORMATS                 VST3 AU Standalone
    VST3_CATEGORIES         "Fx" "Distortion"
    AU_MAIN_TYPE            kAudioUnitType_Effect
    IS_SYNTH                FALSE
    NEEDS_MIDI_INPUT        FALSE
    NEEDS_MIDI_OUTPUT       FALSE
    IS_MIDI_EFFECT          FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VERSION                 "1.0.0"
)

target_sources(DGStomp PRIVATE
    Source/DGStompProcessor.cpp
    Source/DGStompProcessor.h
    Source/DGStompEditor.h
    Source/DGStompDSP.h
)

target_include_directories(DGStomp PRIVATE Source)

target_compile_definitions(DGStomp PUBLIC
    JUCE_WEB_BROWSER=1
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
)

target_link_libraries(DGStomp PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Copy DGStompUI.js into every plugin bundle's Resources folder
set(_JS "${CMAKE_SOURCE_DIR}/Patch/DGStompUI.js")

foreach(_tgt IN ITEMS DGStomp_VST3 DGStomp_AU DGStomp_Standalone)
    if(TARGET ${_tgt})
        add_custom_command(TARGET ${_tgt} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_BUNDLE_CONTENT_DIR:${_tgt}>/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_JS}"
                "$<TARGET_BUNDLE_CONTENT_DIR:${_tgt}>/Resources/DGStompUI.js"
            COMMENT "Copying DGStompUI.js into ${_tgt} bundle"
        )
    endif()
endforeach()

# Install rules — also copy JS alongside plugin
install(FILES "${_JS}"
    DESTINATION "/Library/Audio/Plug-Ins/VST3/DGStomp.vst3/Contents/Resources"
    COMPONENT VST3
)
install(FILES "${_JS}"
    DESTINATION "/Library/Audio/Plug-Ins/Components/DGStomp.component/Contents/Resources"
    COMPONENT AU
)

# Code signing — ad-hoc for local dev
foreach(_fmt IN ITEMS DGStomp DGStomp_VST3 DGStomp_AU DGStomp_Standalone)
    if(TARGET ${_fmt})
        set_target_properties(${_fmt} PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY           "-"
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED        "YES"
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED         "YES"
            XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS        "--deep"
            XCODE_ATTRIBUTE_EXCLUDED_SOURCE_FILE_NAMES  "*.js *.html"
            XCODE_ATTRIBUTE_CODE_SIGN_INJECT_BASE_ENTITLEMENTS "NO"
        )
    endif()
endforeach()

install(TARGETS DGStomp_VST3
    DESTINATION "/Library/Audio/Plug-Ins/VST3" COMPONENT VST3)
install(TARGETS DGStomp_AU
    DESTINATION "/Library/Audio/Plug-Ins/Components" COMPONENT AU)
