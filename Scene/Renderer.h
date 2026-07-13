//
// Created by sweth on 1/7/2026.
//

#ifndef GLPIFRAMEWORKINTRO_RENDERER_H
#define GLPIFRAMEWORKINTRO_RENDERER_H


#pragma once

/**
 * Renderer.h
 *
 * Singleton controller that owns the scene's Model collection and drives
 * their lifecycle (init, resize, render). This is the only class that
 * main.cpp / NativeTemplate.cpp should talk to directly — it hides which
 * concrete Model subclasses (Triangle, Square, ...) actually exist.
 */

#include "Model.h"
#include <vector>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Renderer {
public:
    static Renderer& Instance();

    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

#ifdef PLATFORM_ANDROID
    void setAssetManager(AAssetManager* mgr);
#endif

    bool initializeRenderer();
    void resize(int w, int h);
    void render();

private:
    Renderer()  = default;
    ~Renderer();

    void createModels();
    void initializeModels();
    void clearModels();

#ifdef PLATFORM_ANDROID
    AAssetManager* assetMgr = nullptr;
#endif

    std::vector<Model*> models;  // replaces the single Model* model
};


#endif //GLPIFRAMEWORKINTRO_RENDERER_H
