#define LOG_TAG "Triangle"

#include "Triangle.h"
#include <cmath>
#include <string>

#ifdef PLATFORM_EMSCRIPTEN
static const char* kVertexSrc =
"#version 300 es\n"
"layout(location = 0) in vec4 VertexPosition;\n"
"layout(location = 1) in vec4 VertexColor;\n"
"\n"
"uniform float RadianAngle;\n"
"\n"
"out vec4 TriangleColor;\n"
"\n"
"void main()\n"
"{\n"
"    float c = cos(RadianAngle);\n"
"    float s = sin(RadianAngle);\n"
"\n"
"    vec2 rotated = mat2(c, s, -s, c) * VertexPosition.xy;\n"
"    gl_Position   = vec4(rotated, 0.0, 1.0);\n"
"    TriangleColor = VertexColor;\n"
"}\n";

static const char* kFragmentSrc =
"#version 300 es\n"
"precision mediump float;\n"
"\n"
"in vec4 TriangleColor;\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = TriangleColor;\n"
"}\n";
#endif
// ---------------------------------------------------------------------------
// Vertex data
// ---------------------------------------------------------------------------

static const GLfloat kPositions[] = {
        0.0f,  0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
};

static const GLfloat kColors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
};

// ---------------------------------------------------------------------------
// Triangle
// ---------------------------------------------------------------------------

#ifdef PLATFORM_ANDROID
Triangle::Triangle(AAssetManager* assetMgr) : mgr(assetMgr) {}
#else
Triangle::Triangle() = default;
#endif

Triangle::~Triangle()
{
    glDeleteBuffers(1, &vboColor);
    glDeleteBuffers(1, &vboPos);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);
}

void Triangle::InitModel()
{
    LOGI("Triangle::InitModel");

#ifdef PLATFORM_ANDROID
    programID = ShaderHelper::buildProgramFromAssets(mgr, "shader/BlueTriangleVertex.glsl", "shader/BlueTriangleFragment.glsl");
#elif defined(PLATFORM_EMSCRIPTEN)
    programID = ShaderHelper::buildProgram(kVertexSrc, kFragmentSrc);
#else
    programID = ShaderHelper::buildProgramFromFile("BlueTriangleVertex.glsl", "BlueTriangleFragment.glsl");
#endif
    if (!programID) { LOGE("Triangle: could not create program"); return; }

    uRadianAngle = glGetUniformLocation(programID, "RadianAngle");

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

    LOGI("Triangle::InitModel done");
}

void Triangle::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);

    float radian = degree++ / 57.2957795f;
    glUniform1f(uRadianAngle, radian);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Triangle::Resize(int w, int h)
{
    glViewport(0, 0, w, h);
}