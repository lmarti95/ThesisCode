#pragma once

#if GRAPHICS

#include "EvolutionaryAlgorithm.h"
#include "Shape.h"
#include "MST.h"

class MSTVisualization {
public:
	MSTVisualization(EvolutionaryAlgorithm* aEA);
	~MSTVisualization();

	std::pair<GLfloat, GLfloat> GenerateRandomCoordinates();
	bool CheckCoordinates(std::pair<GLfloat, GLfloat> aCoordinates);
	void GenerateCircles();

	void SaveCircles();
	bool ReadInCircles();

	GLfloat* GetVertices();

	int GetNumberOfTriangles() { return mNumberOfTriangles; }
	int GetNumberOfLines() { return mNumberOfLines; }

	GLfloat* GetColor();

	std::vector<Shape*>* CreateLines();
	Circle* GetCircle(int aID);

	void SetWeightVisibility(bool aWeighVisibility){ mWeighVisibility = aWeighVisibility; }
	bool GetWeightVisibility() { return mWeighVisibility; }

	std::vector<Text> CreateWeighTexts();

	float TranslateCoordinateX(double aX);
	float TranslateCoordinateY(double aY);

private:
	MST* mCostFunction;
	std::vector<Circle*>* mCircles;

	double mCircleSize = 0.03;

	int mNumberOfTriangles = 0;
	int mNumberOfLines = 0;

	EvolutionaryAlgorithm* mEA;

	bool mWeighVisibility = true;
};

#endif