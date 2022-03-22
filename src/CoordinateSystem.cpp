#ifdef GRAPHICS

#include "CoordinateSystem.h"

#include <iostream>
#include <string>

#include "Jump.h"
#include "OpenGL.h"

CoordinateSystem::CoordinateSystem(EvolutionaryAlgorithm* aEA)
{
    mEA = aEA;

    mGapSize = 0;
    if(dynamic_cast<Jump*>(mEA->GetCostFunction()) != nullptr)
    {
        mGapSize = dynamic_cast<Jump*>(mEA->GetCostFunction())->GetGapSize();
    }

    mScaleX = mEA->GetN();
    mScaleY = (mEA->GetN()+ mGapSize)*1.2;

    bool finished = false;
    int i = 1;

    mXCoordinateOffset = mScaleX / 10;
    mYCoordinateOffset = mScaleY / 10;

    if(mXCoordinateOffset < 1)
    {
        mXCoordinateOffset = 1;
    }

    if(mYCoordinateOffset < 1)
    {
        mYCoordinateOffset = 1;
    }

    CalculateTriangles();
    CalculateLines();
}

GLfloat CoordinateSystem::GetLocationXOnCoordinate(int aX)
{
    GLfloat step = (0.88f + 0.86f) / mScaleX;

    return mXCoordinateXStart + step * aX + 0.02;
}

GLfloat CoordinateSystem::GetLocationYOnCoordinate(int aY)
{
    GLfloat step = (0.88f + 0.86f) / mScaleY;

    return mYCoordinateYStart + step * aY - 0.02;
}

std::vector<Shape*>* CoordinateSystem::CreateX()
{
    std::vector<Shape*>* shapes = new std::vector<Shape*>;

    GLfloat black[3] = {0.0f, 0.0f, 0.0f};
    GLfloat pointsRectangle[12] = {
        -0.9f, -0.9f,  0.0f,
        -0.9f, -0.88f, 0.0f,
         0.9f, -0.88f, 0.0f,
         0.9f, -0.9f,  0.0f
    };

    GLfloat pointsTriangle[9] = {
        0.9f,  -0.92f, 0.0f,
        0.9f,  -0.86f, 0.0f,
        0.93f, -0.89f, 0.0f
    };

    shapes->push_back((Shape*)(new Rectangle(pointsRectangle, black)));
    shapes->push_back((Shape*)(new Triangle(pointsTriangle, black)));

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / mScaleX;
    for(int i = 0; i < mScaleX + 1; i=i + mXCoordinateOffset)
    {
        if(i == 0)
        {
            continue;
        }

        GLfloat pointsScale[12] = {
        -0.9f + step * i, -0.93f,  0.0f,
        -0.9f + step * i, -0.85f,  0.0f,
        -0.89f + step * i, -0.85f, 0.0f,
        -0.89f + step * i, -0.93f, 0.0f,
        };

        shapes->push_back((Shape*)(new Rectangle(pointsScale, black)));
    }

    return shapes;
}

std::vector<Shape*>* CoordinateSystem::CreateY()
{
    std::vector<Shape*>* shapes = new std::vector<Shape*>;

    GLfloat black[3] = {0.0f, 0.0f, 0.0f};
    GLfloat points[12] = {
        -0.9f,  -0.9f, 0.0f,
        -0.9f,   0.9f, 0.0f,
        -0.88f,  0.9f, 0.0f,
        -0.88f, -0.9f, 0.0f
    };

    GLfloat pointsTriangle[9] = {
        -0.92f,  0.9f,  0.0f,
        -0.86f,  0.9f,  0.0f,
        -0.89f,  0.93f, 0.0f
    };

    shapes->push_back((Shape*)(new Rectangle(points, black)));
    shapes->push_back((Shape*)(new Triangle(pointsTriangle, black)));

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / mScaleY;
    for(int i = 0; i < mScaleY + 1; i = i + mYCoordinateOffset)
    {
        if(i == 0)
        {
            continue;
        }

        GLfloat pointsScale[12] = {
        -0.93f, -0.9f + step * i,  0.0f,
        -0.93f, -0.89f + step * i,  0.0f,
        -0.85f, -0.89f + step * i, 0.0f,
        -0.85f, -0.9f + step * i, 0.0f,
        };

        shapes->push_back((Shape*)(new Rectangle(pointsScale, black)));
    }

    return shapes;
}

std::vector<Shape*>* CoordinateSystem::CreateOnBoardShapes()
{
    std::vector<Shape*>* shapes = new std::vector<Shape*>;

    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    Circle* c = new Circle(GetLocationXOnCoordinate(0), GetLocationYOnCoordinate(0), 0.03f, red);
    c->SetID(1);
    shapes->push_back((Shape*)(c));

    GLfloat black[3] = {0.0f, 0.0f, 0.0f};

    int XLocation = mEA->GetN();
    if(dynamic_cast<Jump*>(mEA->GetCostFunction())->GetJumpType() == JumpType::OffsetSpike)
    {
        XLocation = mEA->GetN() * 3 / 4 + mGapSize / 2;
    }

    Circle* c2 = new Circle(GetLocationXOnCoordinate(XLocation), GetLocationYOnCoordinate(mEA->GetCostFunction()->GetMaximumFitnessValue()), 0.02f, black);
    shapes->push_back((Shape*)(c2));

    return shapes;
}

std::vector<Shape*>* CoordinateSystem::CreateLines()
{
    if(dynamic_cast<Jump*>(mEA->GetCostFunction()) != nullptr)
    {
        if(dynamic_cast<Jump*>(mEA->GetCostFunction())->GetJumpType() == JumpType::Original)
        {
            return CreateLinesJumpOriginal();
        }

        if(dynamic_cast<Jump*>(mEA->GetCostFunction())->GetJumpType() == JumpType::Offset)
        {
            return CreateLinesJumpOffset();
        }

        if(dynamic_cast<Jump*>(mEA->GetCostFunction())->GetJumpType() == JumpType::OffsetSpike)
        {
            return CreateLinesJumpOffset();
        }
    }

    return nullptr;
}


std::vector<Shape*>* CoordinateSystem::CreateLinesJumpOriginal()
{
    std::vector<Shape*>* lines = new std::vector<Shape*>;
    GLfloat black[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    Line* line1 = new Line(GetLocationXOnCoordinate(0), GetLocationYOnCoordinate(mGapSize), GetLocationXOnCoordinate(mEA->GetN() - mGapSize), GetLocationYOnCoordinate(mEA->GetN()), black);
    lines->push_back((Shape*)line1);

    Line* line2 = new Line(GetLocationXOnCoordinate(mEA->GetN() - mGapSize), GetLocationYOnCoordinate(mGapSize - 1), GetLocationXOnCoordinate(mEA->GetN()), GetLocationYOnCoordinate(0), black);
    lines->push_back((Shape*)line2);

    return lines;
}

std::vector<Shape*>* CoordinateSystem::CreateLinesJumpOffset()
{
    std::vector<Shape*>* lines = new std::vector<Shape*>;
    GLfloat black[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    mEA->GetCostFunction();

    Line* line1 = new Line(GetLocationXOnCoordinate(0), GetLocationYOnCoordinate(mGapSize), GetLocationXOnCoordinate(mEA->GetN()*3/4), GetLocationYOnCoordinate(mEA->GetN()*3/4+ mGapSize), black);
    lines->push_back((Shape*)line1);

    Line* line2 = new Line(GetLocationXOnCoordinate(mEA->GetN()*3/4), GetLocationYOnCoordinate(mGapSize - 1), GetLocationXOnCoordinate(mEA->GetN()*3/4+mGapSize), GetLocationYOnCoordinate(0), black);
    lines->push_back((Shape*)line2);

    Line* line3 = new Line(GetLocationXOnCoordinate(mEA->GetN() * 3 / 4 + mGapSize), GetLocationYOnCoordinate(mEA->GetN() * 3 / 4+mGapSize+mGapSize), GetLocationXOnCoordinate(mEA->GetN()), GetLocationYOnCoordinate(mEA->GetN()+mGapSize), black);
    lines->push_back((Shape*)line3);

    return lines;
}

GLfloat* CoordinateSystem::CreateVertices()
{
    auto* shapes = CreateX();
    auto* y = CreateY();
    auto onboardShapes = CreateOnBoardShapes();

    for(auto& s : *y)
    {
        shapes->push_back(s);
    }
    delete y;

    for(auto& s : *onboardShapes)
    {
        shapes->push_back(s);
    }
    delete onboardShapes;

    GLfloat* vertices = new GLfloat[mNumberOfTriangles * 9 + mNumberOfLines * 6];

    int copied = 0;

    for(auto& s : *shapes)
    {
        if(s->GetiD() == 1)
        {
            mRedCircleStart = copied;
        }
        auto points = s->GetPoints();
        for(int i = 0; i < s->GetNumberOfTriangles() * 9; ++i)
        {
            vertices[copied + i] = points->at(i);
        }
        copied += s->GetNumberOfTriangles() * 9;

        delete points;
    }

    auto lines = CreateLines();

    for(auto& l : *lines)
    {
        auto points = l->GetPoints();
        for(int i = 0; i < 6; ++i)
        {
            vertices[copied + i] = points->at(i);
        }
        copied += 6;

        delete points;
    }

    for(auto& l : *lines)
    {
        delete l;
    }
    delete lines;

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;

    return vertices;
}

GLfloat* CoordinateSystem::CreateLinesColor()
{
    auto lines = CreateLines();

    GLfloat* color = new GLfloat[mNumberOfLines * 6];

    int copied = 0;

    for(auto& l : *lines)
    {
        auto colorPoints = l->GetColor();
        for(int i = 0; i < 6; ++i)
        {
            color[copied + i] = colorPoints->at(i);
        }
        copied += 6;

        delete colorPoints;
    }

    for(auto& l : *lines)
    {
        delete l;
    }
    delete lines;

    return color;
}

GLfloat* CoordinateSystem::CreateColor()
{
    auto* shapes = CreateX();
    auto* y = CreateY();
    auto onboardShapes = CreateOnBoardShapes();

    GLfloat* linesColor = CreateLinesColor();

    for(auto& s : *y)
    {
        shapes->push_back(s);
    }
    delete y;

    for(auto& s : *onboardShapes)
    {
        shapes->push_back(s);
    }
    delete onboardShapes;

    GLfloat* color = new GLfloat[mNumberOfTriangles * 9 + mNumberOfLines*6];

    int copied = 0;

    for(auto& s : *shapes)
    {
        auto colorPoints = s->GetColor();
        for(int i = 0; i < s->GetNumberOfTriangles() * 9; ++i)
        {
            color[copied + i] = colorPoints->at(i);
        }
        copied += s->GetNumberOfTriangles() * 9;

        delete colorPoints;
    }

    for(int i = 0; i < mNumberOfLines * 6; ++i)
    {
        color[copied + i] = linesColor[i];
    }

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;

    return color;
}

GLfloat* CoordinateSystem::CreateLinesVertices()
{
    auto lines = CreateLines();

    GLfloat* vertices = new GLfloat[mNumberOfLines * 6];

    int copied = 0;

    for(auto& l : *lines)
    {
        auto points = l->GetPoints();
        for(int i = 0; i < 6; ++i)
        {
            vertices[copied + i] = points->at(i);
        }
        copied += 6;

        delete points;
    }

    for(auto& l : *lines)
    {
        delete l;
    }
    delete lines;

    return vertices;
}

float CoordinateSystem::TranslateCoordinateX(double aX)
{
    return (aX + 1) / 2 * OpenGL::GetWidth();
}

float CoordinateSystem::TranslateCoordinateY(double aY)
{
    return (-aY + 1) / 2 * OpenGL::GetHeight();
}

void CoordinateSystem::CalculateTriangles()
{
    auto* shapes = CreateX();
    auto* y = CreateY();
    auto onboardShapes = CreateOnBoardShapes();

    for(auto& s : *y)
    {
        shapes->push_back(s);
    }
    delete y;

    for(auto& s : *onboardShapes)
    {
        shapes->push_back(s);
    }
    delete onboardShapes;

    int triangles = 0;

    for(auto& s : *shapes)
    {
        triangles += s->GetNumberOfTriangles();
    }

    mNumberOfTriangles = triangles;

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;
}

void CoordinateSystem::CalculateLines()
{
    auto* shapes = CreateLines();

    mNumberOfLines = shapes->size();

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;
}

GLfloat* CoordinateSystem::ModifyCircle()
{
    double fitnessValue = mEA->GetFitnessValue();
    int sum = mEA->GetCostFunction()->GetSum();

    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    Circle* c = new Circle(GetLocationXOnCoordinate(sum), GetLocationYOnCoordinate(fitnessValue), 0.03f, red);
    auto points = c->GetPoints();

    GLfloat* arrayPoints = new GLfloat[c->GetNumberOfTriangles() * 9];

    for(int i = 0; i < c->GetNumberOfTriangles() * 9; ++i)
    {
        arrayPoints[i] = points->at(i);
    }

    delete points;
    delete c;

    return arrayPoints;
}

std::vector<Text> CoordinateSystem::CreateXValuesText()
{
    std::vector<Text> XValuesText;

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / mScaleX;
    for(int i = 0; i < mScaleX + 1; i = i + mXCoordinateOffset)
    {
        if(i == 0)
        {
            continue;
        }

        GLTtext* t = gltCreateText();
        gltSetText(t, std::to_string(i).c_str());

        GLfloat x = mXCoordinateXStart + step * i;
        GLfloat y = mXCoordinateYStart;

        XValuesText.push_back(Text{t, TranslateCoordinateX(x), TranslateCoordinateY(y)});
    }

    return XValuesText;
}

std::vector<Text> CoordinateSystem::CreateYValuesText()
{
    std::vector<Text> YValuesText;

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / mScaleY;
    for(int i = 0; i < mScaleY + 1; i = i + mYCoordinateOffset)
    {
        if(i == 0)
        {
            continue;
        }

        GLTtext* t = gltCreateText();
        gltSetText(t, std::to_string(i).c_str());

        GLfloat x = mYCoordinateXStart;
        GLfloat y = mYCoordinateYStart + step * i;

        YValuesText.push_back(Text{t, TranslateCoordinateX(x), TranslateCoordinateY(y)});
    }

    return YValuesText;
}

#endif