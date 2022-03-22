#pragma once

#if GRAPHICS

#include "EvolutionaryAlgorithm.h"
#include "Shape.h"
#include "MST.h"

class MST_Visualization {
public:
	MST_Visualization(MST* aCostFunction, EvolutionaryAlgorithm* aEA);
	~MST_Visualization();

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
private:
	MST* mCostFunction;
	std::vector<Circle*>* mCircles;

	double mCircleSize = 0.03;

	int mNumberOfTriangles = 0;
	int mNumberOfLines = 0;

	EvolutionaryAlgorithm* mEA;
};

#endif