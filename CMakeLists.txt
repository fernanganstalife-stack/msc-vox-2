
cmake_minimum_required(VERSION 3.22)

project(MSC_VOXITY_V2 VERSION 2.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

juce_add_plugin(MSC_VOXITY_V2
    COMPANY_NAME "MySpaceCore Audio"
    BUNDLE_IDENTIFIER "com.myspacecore.mscvoxityv2"
    PLUGIN_NAME "MSC VOXITY V2"
    PLUGIN_DESC "Professional Vocal Processing VST3 Plugin"
    PLUGIN_MANUFACTURER "MySpaceCore"
    PLUGIN_CODE "Mvox"
    FORMATS VST3 Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(MSC_VOXITY_V2 PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/DSPModules.h
    Source/VisualizerComponents.h
    Source/AnimationEngine.h
    Source/PresetManager.h
)

target_compile_definitions(MSC_VOXITY_V2 PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(MSC_VOXITY_V2 PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_gui_basics
    juce::juce_dsp
    juce::juce_core
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
