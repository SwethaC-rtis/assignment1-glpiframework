#pragma once

/**
 * Triangle.h
 *
 * Rotating triangle Model. Loads its shader source from .glsl files
 * (via ShaderHelper) instead of embedding shader text as C-strings.
 * Used by Desktop (Windows/GLFW), Emscripten, and Android builds,
 * all driven through the Renderer via the Model interface.
 */

#include "Platform.h"
#include "Model.h"
#include "ShaderHelper.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Triangle : public Model {
public:
#ifdef PLATFORM_ANDROID
    explicit Triangle(AAssetManager* assetMgr);
#else
    Triangle();
#endif
    ~Triangle() override;

    void InitModel() override;
    void Render() override;
    void Resize(int w, int h) override;

private:
#ifdef PLATFORM_ANDROID
    AAssetManager* mgr = nullptr;
#endif
    GLuint programID    = 0;
    GLuint vao          = 0;
    GLuint vboPos       = 0;
    GLuint vboColor     = 0;
    GLint  uRadianAngle = -1;
    float  degree       = 0.0f;
};