#pragma once

#include "EvolutionaryAlgorithm.h"
#include "Shape.h"

class CoordinateSystem 
{
public:
    CoordinateSystem(EvolutionaryAlgorithm* aEA);

    GLfloat GetLocationXOnCoordinate(int aX);
    GLfloat GetLocationYOnCoordinate(int aY);

    std::vector<Shape*>* CreateX();
    std::vector<Shape*>* CreateY();

    std::vector<Shape*>* CreateOnBoardShapes();
    std::vector<Shape*>* CreateLines();

    std::vector<Shape*>* CreateLinesJumpOriginal();
    std::vector<Shape*>* CreateLinesJumpOffset();

    GLfloat* CreateVertices();
    GLfloat* CreateLinesColor();

    GLfloat* CreateColor();
    GLfloat* CreateLinesVertices();

    float TranslateCoordinateX(double aX);
    float TranslateCoordinateY(double aY);

    void CalculateTriangles();
    void CalculateLines();

    int GetNumberOfTriangles() { return mNumberOfTriangles; }
    int GetNumberOfLines() { return mNumberOfLines; }

    GLfloat* ModifyCircle();
    int GetRedCircleStart() { return mRedCircleStart; }

    std::vector<Text> CreateXValuesText();
    std::vector<Text> CreateYValuesText();

    EvolutionaryAlgorithm* GetEvolutionaryAlgorithm() { return mEA; }
private:
    int mScaleX;
    int mScaleY;
    int mGapSize;

    int mNumberOfTriangles = 0;
    int mNumberOfLines = 0;

    GLfloat mXCoordinateXStart = -0.91f;
    GLfloat mXCoordinateYStart = -0.95f;
    GLfloat mYCoordinateXStart = -0.98f;
    GLfloat mYCoordinateYStart = -0.87f;

    int mRedCircleStart = -1;

    EvolutionaryAlgorithm* mEA;

    int mXCoordinateOffset = 1;
    int mYCoordinateOffset = 1;
};