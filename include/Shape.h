#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

#include <vector>

const int circleTriangles = 20;
const GLfloat pi = 3.14159265f;

class Shape{
public:
    Shape(GLfloat* aColor);

    virtual ~Shape() {}

    virtual std::vector<GLfloat>* GetPoints() = 0;
    virtual std::vector<GLfloat>* GetColor() = 0;

    virtual int GetNumberOfTriangles() = 0;

    void SetID(int aID) { mID = aID; }
    int GetiD() { return mID; }

protected:
    GLfloat mColor[9];
    int mID = -1;
};

class Triangle : Shape{
public:
    Triangle(GLfloat* aPoints, GLfloat* aColor);

    std::vector<GLfloat>* GetPoints();
    std::vector<GLfloat>* GetColor();

    int GetNumberOfTriangles() { return 1; }

private:
    GLfloat mPoints[9];
};

class Rectangle : public Shape{
public:
    Rectangle(GLfloat* aPoints, GLfloat* aColor);
    ~Rectangle();

    std::vector<GLfloat>* GetPoints();
    std::vector<GLfloat>* GetColor();

    int GetNumberOfTriangles() { return 2; }

private:
    Triangle* t1;
    Triangle* t2;
};

class Circle : public Shape{
public:
    Circle(GLfloat aX, GLfloat aY, GLfloat aR, GLfloat* aColor);
    ~Circle();

    std::vector<GLfloat>* GetPoints();
    std::vector<GLfloat>* GetColor();

    int GetNumberOfTriangles() { return circleTriangles; }

private:
    std::vector<Triangle*> mTriangles;
};

class Line : public Shape{
public:
    Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat* aColor);

    std::vector<GLfloat>* GetPoints();
    std::vector<GLfloat>* GetColor();

    int GetNumberOfTriangles() { return 1; }
    
private:
    GLfloat mPoints[6];
};

struct Text{
    GLTtext* gltText;
    float pixelPositionX;
    float pixelPositionY;
};