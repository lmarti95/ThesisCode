#if GRAPHICS

#include "Shape.h"

#include <math.h>

//Shape

Shape::Shape(GLfloat* aColor)
{
    std::copy(aColor, aColor + 3, mColor);
    std::copy(aColor, aColor + 3, mColor + 3);
    std::copy(aColor, aColor + 3, mColor + 6);
}

//Triangle

Triangle::Triangle(GLfloat* aPoints, GLfloat* aColor) : Shape(aColor)
{
    std::copy(aPoints, aPoints + 9, mPoints);
}

std::vector<GLfloat>* Triangle::GetPoints()
{
    std::vector<GLfloat>* points = new std::vector<GLfloat>;
    for(int i = 0; i < 9; ++i)
    {
        points->push_back(mPoints[i]);
    }
    return points;
}

std::vector<GLfloat>* Triangle::GetColor()
{
    std::vector<GLfloat>* pointsColor = new std::vector<GLfloat>;
    for(int i = 0; i < 9; ++i)
    {
        pointsColor->push_back(mColor[i]);
    }
    return pointsColor;
}

//Rectangle

Rectangle::Rectangle(GLfloat* aPoints, GLfloat* aColor) : Shape(aColor)
{
    t1 = new Triangle(aPoints, aColor);
    GLfloat points[9];
    for(int i = 0; i < 3; ++i)
    {
        points[i] = aPoints[i];
    }
    for(int i = 6; i < 12; ++i)
    {
        points[i - 3] = aPoints[i];
    }
    t2 = new Triangle(points, aColor);
}
Rectangle::~Rectangle()
{
    delete t1;
    delete t2;
}

std::vector<GLfloat>* Rectangle::GetPoints()
{
    auto t1points = t1->GetPoints();
    auto t2points = t2->GetPoints();
    for(int i = 0; i < 9; ++i)
    {
        t1points->push_back(t2points->at(i));
    }

    delete t2points;
    return t1points;
}

std::vector<GLfloat>* Rectangle::GetColor()
{
    auto t1pointsColor = t1->GetColor();
    auto t2pointsColor = t2->GetColor();
    for(int i = 0; i < 9; ++i)
    {
        t1pointsColor->push_back(t2pointsColor->at(i));
    }

    delete t2pointsColor;
    return t1pointsColor;
}

//Circle

Circle::Circle(GLfloat aX, GLfloat aY, GLfloat aR, GLfloat* aColor) : Shape(aColor)
{
    mCenter = std::make_pair(aX, aY);

    for(int i = 0; i < circleTriangles; ++i)
    {
        int next;
        if(i != circleTriangles - 1)
        {
            next = i + 1;
        }
        else
        {
            next = 0;
        }

        GLfloat points[9] =
        {
            aX, aY, 0,
            aX + (aR * cos((360 / circleTriangles * i) * pi / 180)), aY + (aR * sin((360 / circleTriangles * i) * pi / 180)), 0,
            aX + (aR * cos((360 / circleTriangles * next) * pi / 180)), aY + (aR * sin((360 / circleTriangles * next) * pi / 180)), 0,
        };

        mTriangles.push_back(new Triangle(points, aColor));
    }

}
Circle::~Circle()
{
    for(auto& t : mTriangles)
    {
        delete t;
    }
}

std::vector<GLfloat>* Circle::GetPoints()
{
    std::vector<GLfloat>* points = new std::vector<GLfloat>;
    for(auto& t : mTriangles)
    {
        auto pointsTriangle = t->GetPoints();
        for(int i = 0; i < 9; ++i)
        {
            points->push_back(pointsTriangle->at(i));
        }

        delete pointsTriangle;
    }
    return points;
}

std::vector<GLfloat>* Circle::GetColor()
{
    std::vector<GLfloat>* pointsColor = new std::vector<GLfloat>;
    for(auto& t : mTriangles)
    {
        auto colorPointsTriangle = t->GetColor();
        for(int i = 0; i < 9; ++i)
        {
            pointsColor->push_back(colorPointsTriangle->at(i));
        }

        delete colorPointsTriangle;
    }
    return pointsColor;
}

//Line

Line::Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat* aColor) : Shape(aColor)
{
    mPoints[0] = x1;
    mPoints[1] = y1;
    mPoints[2] = 0.0f;
    mPoints[3] = x2;
    mPoints[4] = y2;
    mPoints[5] = 0.0f;
}

std::vector<GLfloat>* Line::GetPoints()
{
    std::vector<GLfloat>* points = new std::vector<GLfloat>;
    for(int i = 0; i < 6; ++i)
    {
        points->push_back(mPoints[i]);
    }
    return points;
}

std::vector<GLfloat>* Line::GetColor()
{
    std::vector<GLfloat>* pointsColor = new std::vector<GLfloat>;
    for(int i = 0; i < 6; ++i)
    {
        pointsColor->push_back(mColor[i]);
    }
    return pointsColor;
}

#endif