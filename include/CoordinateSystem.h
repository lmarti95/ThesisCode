#pragma once

#include "Shape.h"

class CoordinateSystem 
{
public:
    CoordinateSystem();

    GLfloat GetLocationXOnCoordinate(int aX);
    GLfloat GetLocationYOnCoordinate(int aY);

    std::vector<Shape*>* CreateX();
    std::vector<Shape*>* CreateY();

    std::vector<Shape*>* CreateOnBoardShapes();
    std::vector<Shape*>* CreateLines();

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

    GLfloat* ModifyCircle(int iterations);
    int GetRedCircleStart() { return mRedCircleStart; }

    std::vector<Text> CreateXValuesText();
    std::vector<Text> CreateYValuesText();
private:
    int mScaleX = 20;
    int mScaleY = 30;

    int mNumberOfTriangles = 0;
    int mNumberOfLines = 0;

    GLfloat mXCoordinateXStart = -0.91f;
    GLfloat mXCoordinateYStart = -0.95f;
    GLfloat mYCoordinateXStart = -0.98f;
    GLfloat mYCoordinateYStart = -0.87f;

    int mRedCircleStart = -1;
};