#pragma once 

#include "CoordinateSystem.h"

enum class OpenGLMode{ CoordinateSystem, MST };

class OpenGL
{
public:
	void Setup();
	void Draw();
	void ShutDown();

	static int GetWidth() { return mWidth; }
	static int GetHeight() { return mHeight; }
private:
	static const int mWidth = 800;
	static const int mHeight = 800;
	static const OpenGLMode mMode = OpenGLMode::CoordinateSystem;

	GLFWwindow* mWindow;
	GLuint mShaderProgram;
	const GLchar* vertexShaderSource;
	const GLchar* fragmentShaderSource;

	GLuint mVAO;
	GLuint mVBO;
	GLuint mColorbufferID;
	GLfloat* mVertices;
	GLfloat* mColor;

	CoordinateSystem mCoordinateSystem;

	GLTtext* mYText;
	GLTtext* mXText;

	std::vector<Text> mXValuesText;
	std::vector<Text> mYValuesText;
};