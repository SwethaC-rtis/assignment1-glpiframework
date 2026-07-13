//
// Created by sweth on 1/7/2026.
//

#ifndef GLPIFRAMEWORKINTRO_MODEL_H
#define GLPIFRAMEWORKINTRO_MODEL_H


#pragma once

class Model {
public:
    Model() {}
    virtual ~Model() {}

    virtual void InitModel() = 0;              // called once, before the first frame
    virtual void Render()    = 0;               // called once per frame
    virtual void Resize(int w, int h) {}        // called whenever the surface/window resizes
};


#endif //GLPIFRAMEWORKINTRO_MODEL_H
