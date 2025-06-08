#pragma once
#include "glad/glad.h"
#include "Model.h"

namespace DebugDraw
{
	inline GLuint gDebugShader = 0;
	inline GLuint gVAO = 0, gVBO = 0;
	inline GLint gMvpLoc = -1, gColorLoc = -1;
	inline bool gInitialized = false;
	inline RGB gLineColour = { 0.2f, 0.2, 0.2f };

	GLuint CreateShaderProgram();

	void DebugDrawInit();
	void SetLineColour(const RGB& colour);
	void DrawDebugLine3D(const float* modelMatrix, float* startPos, float* endPos);
	void DrawDebugBox3D(const BoundingVolume* bounds, const Matrix* modelMatrix);
};
