//
// Created by sweth on 1/7/2026.
//

#define LOG_TAG "Square"

#include "Square.h"
#include <string>

// ---------------------------------------------------------------------------
// Vertex data — a square offset to the right side of clip space so it
// doesn't overlap the triangle, drawn as a GL_TRIANGLE_FAN (4 verts).
// ---------------------------------------------------------------------------

static const GLfloat kPositions[] = {
        0.4f,  0.6f,
        0.4f,  0.2f,
        0.8f,  0.2f,
        0.8f,  0.6f
};

static const GLfloat kColors[] = {
        1.0f, 1.0f, 0.0f,   // yellow
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
};

// ---------------------------------------------------------------------------
// Square
// ---------------------------------------------------------------------------

#ifdef PLATFORM_ANDROID
Square::Square(AAssetManager* assetMgr) : mgr(assetMgr) {}
#else
Square::Square() = default;
#endif

Square::~Square()
{
    glDeleteBuffers(1, &vboColor);
    glDeleteBuffers(1, &vboPos);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);
}

void Square::InitModel()
{
    LOGI("Square::InitModel");

#ifdef PLATFORM_ANDROID
    programID = ShaderHelper::buildProgramFromAssets(mgr, "shader/BlueTriangleVertex.glsl", "shader/BlueTriangleFragment.glsl");
#else
    programID = ShaderHelper::buildProgramFromFile("BlueTriangleVertex.glsl", "BlueTriangleFragment.glsl");
#endif
    if (!programID) { LOGE("Square: could not create program"); return; }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kPositions), kPositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    LOGI("Square::InitModel done");
}

void Square::Render()
{
    glUseProgram(programID);

    // Square's shader also has a RadianAngle uniform (shared .glsl files
    // with Triangle) — set to 0 so it stays fixed, not rotating.
    GLint uRadianAngle = glGetUniformLocation(programID, "RadianAngle");
    if (uRadianAngle >= 0) glUniform1f(uRadianAngle, 0.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void Square::Resize(int /*w*/, int /*h*/)
{
    // Square doesn't need to react to resize individually —
    // Renderer already calls glViewport via Triangle's Resize.
}
