//
// Created by sweth on 1/7/2026.
//

#include "ShaderHelper.h"
#include <string>

// ---------------------------------------------------------------------------
// Core compile/link — moved from Triangle.cpp, renamed as ShaderHelper methods
// ---------------------------------------------------------------------------

GLuint ShaderHelper::compileShader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    if (!shader) { LOGE("glCreateShader failed (type=0x%x)", type); return 0; }
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(static_cast<size_t>(len), '\0');
        glGetShaderInfoLog(shader, len, nullptr, &log[0]);
        LOGE("Shader compile error:\n%s", log.c_str());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ShaderHelper::linkProgram(GLuint vert, GLuint frag)
{
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    GLint ok = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        std::string log(static_cast<size_t>(len), '\0');
        glGetProgramInfoLog(prog, len, nullptr, &log[0]);
        LOGE("Program link error:\n%s", log.c_str());
        glDeleteProgram(prog);
        prog = 0;
    }
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}

// ---------------------------------------------------------------------------
// Method 1: "stringify" — build directly from inline source strings
// ---------------------------------------------------------------------------

GLuint ShaderHelper::buildProgram(const char* vertSrc, const char* fragSrc)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc);
    if (!vs) return 0;
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fs) { glDeleteShader(vs); return 0; }
    return linkProgram(vs, fs);
}

// ---------------------------------------------------------------------------
// Method 2: file-based — load shader source from disk / APK assets
// ---------------------------------------------------------------------------

#ifdef PLATFORM_ANDROID

std::string ShaderHelper::loadAsset(AAssetManager* mgr, const char* path)
{
    if (!mgr) { LOGE("loadAsset: null AAssetManager"); return ""; }

    AAsset* asset = AAssetManager_open(mgr, path, AASSET_MODE_BUFFER);
    if (!asset) { LOGE("loadAsset: could not open %s", path); return ""; }

    size_t size = AAsset_getLength(asset);
    std::string content(size, '\0');
    AAsset_read(asset, &content[0], size);
    AAsset_close(asset);
    return content;
}

GLuint ShaderHelper::buildProgramFromAssets(AAssetManager* mgr, const char* vertPath, const char* fragPath)
{
    std::string vertSrc = loadAsset(mgr, vertPath);
    std::string fragSrc = loadAsset(mgr, fragPath);
    if (vertSrc.empty() || fragSrc.empty()) {
        LOGE("buildProgramFromAssets: failed to load shader source");
        return 0;
    }
    return buildProgram(vertSrc.c_str(), fragSrc.c_str());
}

#else

std::string ShaderHelper::loadFile(const char* filename)
{
    std::string candidates[] = {
        std::string("assets/shader/") + filename,
        std::string(filename),
        std::string("shader/") + filename,
        std::string("assets/") + filename
    };

    for (const auto& path : candidates) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
    }

    LOGE("loadFile: could not find %s in any known location", filename);
    return "";
}

GLuint ShaderHelper::buildProgramFromFile(const char* vertFile, const char* fragFile)
{
    std::string vertSrc = loadFile(vertFile);
    std::string fragSrc = loadFile(fragFile);
    if (vertSrc.empty() || fragSrc.empty()) {
        LOGE("buildProgramFromFile: failed to load shader source");
        return 0;
    }
    return buildProgram(vertSrc.c_str(), fragSrc.c_str());
}

#endif