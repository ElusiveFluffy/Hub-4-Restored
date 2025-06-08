#include "pch.h"
#include "DebugDraw.h"
#include "TygerFrameworkAPI.hpp"
#include "MinHook.h"

//TygerMemory
#include "core.h"

const char* vertexShaderSrc = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * modelMatrix * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core

uniform vec3 Colour;
out vec4 FragColour;

void main() {
    FragColour = vec4(Colour, 1.0);
}
)";

GLuint CompileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        OutputDebugStringA(log);
        return 0;
    }

    return shader;
}

GLuint DebugDraw::CreateShaderProgram()
{
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glBindAttribLocation(program, 0, "aPos");
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        OutputDebugStringA(log);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void DebugDraw::DebugDrawInit()
{
    gDebugShader = CreateShaderProgram();

    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

    glEnableVertexAttribArray(0); // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glBindVertexArray(0);
}

void DebugDraw::SetLineColour(const RGB& colour)
{
    gLineColour = colour;
}

void DebugDraw::DrawDebugLine3D(const float* modelMatrix, float* startPos, float* endPos) {
    if (!gInitialized)
    {
        if (!gladLoadGL()) {
            OutputDebugStringA("GLAD failed to initialize\n");
        }
        else {
            OutputDebugStringA("GLAD initialized successfully\n");
            DebugDrawInit();
            gInitialized = true;
        }
    }
    glUseProgram(gDebugShader);

    // Set uniforms
    GLint viewProjLoc = glGetUniformLocation(gDebugShader, "viewProj");
    GLint modelLoc = glGetUniformLocation(gDebugShader, "modelMatrix");
    GLint colourLoc = glGetUniformLocation(gDebugShader, "Colour");

    float* viewProjection = (float*)(Core::moduleBase + 0x27F688);

    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, viewProjection);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)modelMatrix);
    glUniform3f(colourLoc, gLineColour.R, gLineColour.G, gLineColour.B);

    float lineVerts[6] = { startPos[0], startPos[1], startPos[2],
                           endPos[0], endPos[1], endPos[2] };

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts), lineVerts, GL_DYNAMIC_DRAW);

    glBindVertexArray(gVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    glUseProgram(0);
}

void DebugDraw::DrawDebugBox3D(const BoundingVolume* bounds, const Matrix* modelMatrix)
{
    if (!gInitialized)
    {
        if (!gladLoadGL()) {
            OutputDebugStringA("GLAD failed to initialize\n");
        }
        else {
            OutputDebugStringA("GLAD initialized successfully\n");
            DebugDrawInit();
            gInitialized = true;
        }
    }
    Vector4f endPos = {
        bounds->StartPos.x + bounds->Length.x,
        bounds->StartPos.y + bounds->Length.y,
        bounds->StartPos.z + bounds->Length.z,
        0.0f
    };

    float corners[8][3] = {
        {bounds->StartPos.x,  bounds->StartPos.y, bounds->StartPos.z},
        {endPos.x,  bounds->StartPos.y, bounds->StartPos.z},
        {endPos.x, endPos.y, bounds->StartPos.z},
        {bounds->StartPos.x, endPos.y, bounds->StartPos.z},
        {bounds->StartPos.x,  bounds->StartPos.y, endPos.z},
        {endPos.x,  bounds->StartPos.y, endPos.z},
        {endPos.x, endPos.y, endPos.z},
        {bounds->StartPos.x, endPos.y, endPos.z} 
    };

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bottom
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // top
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // verticals
    };

    float lineVerts[72]; // 12 lines × 2 points × 3 floats

    for (int i = 0; i < 12; ++i) {
        int idx0 = edges[i][0];
        int idx1 = edges[i][1];

        // Line start
        lineVerts[i * 6 + 0] = corners[idx0][0];
        lineVerts[i * 6 + 1] = corners[idx0][1];
        lineVerts[i * 6 + 2] = corners[idx0][2];

        // Line end
        lineVerts[i * 6 + 3] = corners[idx1][0];
        lineVerts[i * 6 + 4] = corners[idx1][1];
        lineVerts[i * 6 + 5] = corners[idx1][2];
    }

    glUseProgram(gDebugShader);

    // Set uniforms
    GLint viewProjLoc = glGetUniformLocation(gDebugShader, "viewProj");
    GLint modelLoc = glGetUniformLocation(gDebugShader, "modelMatrix");
    GLint colourLoc = glGetUniformLocation(gDebugShader, "Colour");

    // Ty's view projection matrix
    float* viewProjection = (float*)(Core::moduleBase + 0x27F688);

    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, viewProjection);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)modelMatrix);
    glUniform3f(colourLoc, gLineColour.R, gLineColour.G, gLineColour.B);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts), lineVerts, GL_DYNAMIC_DRAW);

    glBindVertexArray(gVAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);

    glUseProgram(0);
}
