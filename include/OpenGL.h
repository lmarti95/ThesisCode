#pragma once 

#if GRAPHICS

#include <chrono>

#include "CoordinateSystem.h"
#include "EvolutionaryAlgorithm.h"
#include "MST_Visualization.h"

enum class OpenGLMode{ CoordinateSystem, MST };

class OpenGL
{
public:


	void Setup(EvolutionaryAlgorithm* aEA);
	void SetText();
	void SetInfoText();
	void SetCoordinateSystemText();
	void SetWeightText();
	void SetWindow();
	void SetShaders();
	void SetVerticeBuffer();
	void SetColorBuffer();

	void Draw();
	void DrawVertices();
	void DrawText();
	void DrawInfoText();

	void SetRuntime();

	void Update();
	void UpdateInfoText();

	void ShutDown();

	static int GetWidth() { return mWidth; }
	static int GetHeight() { return mHeight; }

	std::string GetBitString();

	OpenGLMode GetOpenGLMode() { return mMode; }

	float TranslateCoordinateX(double aX);
	float TranslateCoordinateY(double aY);
	
private:
	static const int mWidth = 800;
	static const int mHeight = 800;
	OpenGLMode mMode = OpenGLMode::MST;

	GLFWwindow* mWindow;
	GLuint mShaderProgram;
	const GLchar* vertexShaderSource;
	const GLchar* fragmentShaderSource;

	GLuint mVAO;
	GLuint mVBO;
	GLuint mColorbufferID;
	GLfloat* mVertices;
	int mVerticeSize;
	GLfloat* mColor;
	int mColorSize;

	CoordinateSystem* mCoordinateSystem;
	MST_Visualization* mMST;

	GLTtext* mYText;
	GLTtext* mXText;
	GLTtext* mEAName;
	GLTtext* mCostFunctionName;
	GLTtext* mBitStringText;
	GLTtext* mIterationsText;
	GLTtext* mFitnessValueText;

	double mInfoXValue;

	std::vector<Text> mXValuesText;
	std::vector<Text> mYValuesText;

	EvolutionaryAlgorithm* mEA;

	std::chrono::steady_clock::time_point mStart;
	std::chrono::steady_clock::time_point mEnd;

	std::vector<Text> mWeightText;
};

#endif