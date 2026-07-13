//
// Created by sweth on 1/7/2026.
//

#ifndef GLPIFRAMEWORKINTRO_SHADERHELPER_H
#define GLPIFRAMEWORKINTRO_SHADERHELPER_H


#pragma once
#include "Platform.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class ShaderHelper {
public:
    // --- Core compile/link, shared by both loading methods below ---
    static GLuint compileShader(GLenum type, const char* src);
    static GLuint linkProgram(GLuint vert, GLuint frag);

    // --- Method 1: "stringify" — build directly from inline source strings ---
    static GLuint buildProgram(const char* vertSrc, const char* fragSrc);

    // --- Method 2: file-based — load shader source from disk / APK assets ---
#ifdef PLATFORM_ANDROID
    static std::string loadAsset(AAssetManager* mgr, const char* path);
    static GLuint buildProgramFromAssets(AAssetManager* mgr, const char* vertPath, const char* fragPath);
#else
    static std::string loadFile(const char* filename);
    static GLuint buildProgramFromFile(const char* vertFile, const char* fragFile);
#endif
};

#endif //GLPIFRAMEWORKINTRO_SHADERHELPER_H
