//
// Created by sweth on 1/7/2026.
//

#ifndef GLPIFRAMEWORKINTRO_SQUARE_H
#define GLPIFRAMEWORKINTRO_SQUARE_H

#pragma once

/**
 * Square.h
 *
 * Second Model in the scene, rendered alongside Triangle. Follows the
 * exact same pattern as Triangle: builds its own shader program via
 * ShaderHelper, owns its own VAO/VBOs, and does not share GPU resources
 * with any other Model.
 */

#include "Platform.h"
#include "Model.h"
#include "ShaderHelper.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Square : public Model {
public:
#ifdef PLATFORM_ANDROID
    explicit Square(AAssetManager* assetMgr);
#else
    Square();
#endif
    ~Square() override;

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
};

#endif //GLPIFRAMEWORKINTRO_SQUARE_H
