#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION
#include "../gltext.h"

#include <iostream>
#include <chrono>
#include <math.h>  
#include <string>
#include <thread>
#include <vector>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error](" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color; \n"
"out vec3 vcolor;"
"void main()\n"
"{\n"
"vcolor = color;"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 vcolor;"
"out vec3 fragColor;\n"
"void main()\n"
"{\n"
"fragColor.x = vcolor.x;"
"fragColor.y = vcolor.y;"
"fragColor.z = vcolor.z;"
"}\n\0";


const int scaleX = 20;
const int scaleY = 30;
const int circleTriangles = 20;

int numberOfTriangles = 0;
int numberOfLines = 0;

const GLfloat pi = 3.14159265f;

const GLuint width = 800, height = 800;

const GLfloat XCoordinateXStart = -0.91f;
const GLfloat XCoordinateYStart = -0.95f;
const GLfloat YCoordinateXStart = -0.98f;
const GLfloat YCoordinateYStart = -0.87f;

int RedCircleStart = -1;

struct Text{
    GLTtext* gltText;
    float pixelPositionX;
    float pixelPositionY;
};

class Shape{
public:
    Shape(GLfloat* aColor)
    {
        std::copy(aColor, aColor + 3, mColor);
        std::copy(aColor, aColor + 3, mColor + 3);
        std::copy(aColor, aColor + 3, mColor + 6);
    }

    virtual ~Shape() {}
    
    virtual std::vector<GLfloat>* GetColor() = 0;
    virtual std::vector<GLfloat>* GetPoints() = 0;
    virtual int GetNumberOfTriangles() = 0;

    void SetID(int aID) { mID = aID; }
    int GetiD() { return mID; }

protected:
    GLfloat mColor[9];
    int mID = -1;
};

class Triangle : Shape{
public:
    Triangle(GLfloat* aPoints, GLfloat* aColor) : Shape(aColor)
    {
        std::copy(aPoints, aPoints + 9, mPoints);
    }

    std::vector<GLfloat>* GetPoints()
    {
        std::vector<GLfloat>* points = new std::vector<GLfloat>;
        for(int i = 0; i < 9; ++i)
        {
            points->push_back(mPoints[i]);
        }
        return points;
    }

    int GetNumberOfTriangles()
    {
        return 1;
    }

    std::vector<GLfloat>* GetColor()
    {
        std::vector<GLfloat>* pointsColor = new std::vector<GLfloat>;
        for(int i = 0; i < 9; ++i)
        {
            pointsColor->push_back(mColor[i]);
        }
        return pointsColor;
    }
private:
    GLfloat mPoints[9];
};

class Rectangle : public Shape{
public:
    Rectangle(GLfloat* aPoints, GLfloat* aColor) : Shape(aColor)
    {
        t1 = new Triangle(aPoints, aColor);
        GLfloat points[9];
        for(int i = 0; i < 3; ++i)
        {
            points[i] = aPoints[i];
        }
        for(int i = 6; i < 12; ++i)
        {
            points[i-3] = aPoints[i];
        }
        t2 = new Triangle(points, aColor);
    }
    ~Rectangle()
    {
        delete t1;
        delete t2;
    }

    std::vector<GLfloat>* GetPoints()
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

    int GetNumberOfTriangles()
    {
        return 2;
    }

    std::vector<GLfloat>* GetColor()
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
private:
    Triangle* t1;
    Triangle* t2;
};

class Circle : public Shape{
public:
    Circle(GLfloat aX, GLfloat aY, GLfloat aR, GLfloat* aColor) : Shape(aColor)
    {
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
                aX+(aR*cos((360/circleTriangles*i) * pi/180)), aY + (aR * sin((360 / circleTriangles * i) * pi / 180)), 0,
                aX + (aR * cos((360 / circleTriangles * next) * pi / 180)), aY + (aR * sin((360 / circleTriangles * next) * pi / 180)), 0,
            };

            mTriangles.push_back(new Triangle(points, aColor));
        }

    }
    ~Circle()
    {
        for(auto& t : mTriangles)
        {
            delete t;
        }
    }

    std::vector<GLfloat>* GetPoints()
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

    int GetNumberOfTriangles()
    {
        return circleTriangles;
    }

    std::vector<GLfloat>* GetColor()
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
private:
    std::vector<Triangle*> mTriangles;
};

class Line : public Shape{
public:
    Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat* aColor) : Shape(aColor)
    {
        mPoints[0] = x1;
        mPoints[1] = y1;
        mPoints[2] = 0.0f;
        mPoints[3] = x2;
        mPoints[4] = y2;
        mPoints[5] = 0.0f;
    }

    std::vector<GLfloat>* GetPoints()
    {
        std::vector<GLfloat>* points = new std::vector<GLfloat>;
        for(int i = 0; i < 6; ++i)
        {
            points->push_back(mPoints[i]);
        }
        return points;
    }

    int GetNumberOfTriangles()
    {
        return 1;
    }

    std::vector<GLfloat>* GetColor()
    {
        std::vector<GLfloat>* pointsColor = new std::vector<GLfloat>;
        for(int i = 0; i < 4; ++i)
        {
            pointsColor->push_back(mColor[i]);
        }
        return pointsColor;
    }
private:
    GLfloat mPoints[6];
};

GLfloat GetLocationXOnCoordinate(int aX)
{
    GLfloat step = (0.88f + 0.86f) / scaleX;

    return XCoordinateXStart + step * aX+0.02;
}

GLfloat GetLocationYOnCoordinate(int aY)
{
    GLfloat step = (0.88f + 0.86f) / scaleY;

    return YCoordinateYStart + step * aY-0.02;
}

std::vector<Shape*>* CreateX()
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
    GLfloat step = (0.88f +0.86f) / scaleX;
    for(int i = 1; i < scaleX+1; ++i)
    {
        GLfloat pointsScale[12] = {
        -0.9f  + step * i, -0.93f,  0.0f,
        -0.9f  + step * i, -0.85f,  0.0f,
        -0.89f + step * i, -0.85f, 0.0f,
        -0.89f + step * i, -0.93f, 0.0f,
        };

        shapes->push_back((Shape*)(new Rectangle(pointsScale, black)));
    }

    return shapes;
}

std::vector<Shape*>* CreateY()
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
    GLfloat step = (0.88f + 0.86f) / scaleY;
    for(int i = 1; i < scaleY + 1; ++i)
    {
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

std::vector<Shape*>* CreateOnBoardShapes()
{
    std::vector<Shape*>* shapes = new std::vector<Shape*>;

    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    Circle* c = new Circle(GetLocationXOnCoordinate(3), GetLocationYOnCoordinate(9), 0.03f, red);
    c->SetID(1);
    shapes->push_back((Shape*)(c));

    GLfloat black[3] = {0.0f, 0.0f, 0.0f};
    Circle* c2 = new Circle(GetLocationXOnCoordinate(scaleX), GetLocationYOnCoordinate(26), 0.02f, black);
    shapes->push_back((Shape*)(c2));

    return shapes;
}

std::vector<Shape*>* CreateLines()
{
    std::vector<Shape*>* lines = new std::vector<Shape*>;
    GLfloat black[3] = {0.0f, 0.0f, 0.0f};

    Line* line1 = new Line(GetLocationXOnCoordinate(0), GetLocationYOnCoordinate(6), GetLocationXOnCoordinate(15), GetLocationYOnCoordinate(21), black);
    lines->push_back((Shape*)line1);

    Line* line2 = new Line(GetLocationXOnCoordinate(16), GetLocationYOnCoordinate(4), GetLocationXOnCoordinate(scaleX), GetLocationYOnCoordinate(0), black);
    lines->push_back((Shape*)line2);

    return lines;
}

GLfloat* CreateVertices()
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

    GLfloat* vertices = new GLfloat[numberOfTriangles*9+numberOfLines*6];

    int copied = 0;

    for(auto& s : *shapes)
    {
        if(s->GetiD() == 1)
        {
            RedCircleStart = copied;
        }
        auto points = s->GetPoints();
        for(int i = 0; i < s->GetNumberOfTriangles() * 9; ++i)
        {
            vertices[copied+i] =points->at(i);
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

GLfloat* CreateLinesColor()
{
    auto lines = CreateLines();

    GLfloat* color = new GLfloat[numberOfLines * 6];

    int copied = 0;

    for(auto& l : *lines)
    {
        auto colorPoints = l->GetColor();
        for(int i = 0; i < numberOfLines*6; ++i)
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

GLfloat* CreateColor()
{
    auto* shapes = CreateX();
    auto* y = CreateY();
    auto onboardShapes = CreateOnBoardShapes();

    //GLfloat* linesColor = CreateLinesColor();

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

    GLfloat* color = new GLfloat[numberOfTriangles * 9];

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

    for(int i = 0; i < numberOfLines * 6; ++i)
    {
        //color[copied + i] = linesColor[i];
    }

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;

    return color;
}

GLfloat* CreateLinesVertices()
{
    auto lines = CreateLines();

    GLfloat* vertices = new GLfloat[numberOfLines * 6];

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

float TranslateCoordinateX(double aX)
{
    return (aX + 1) / 2 * width;
}

float TranslateCoordinateY(double aY)
{
    return (-aY + 1) / 2 * height;
}

void CalculateTriangles()
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

    numberOfTriangles = triangles;

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;
}

void CalculateLines()
{
    auto* shapes = CreateLines();

    numberOfLines = shapes->size();

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;
}

void ModifyCircle(int iterations)
{
    iterations = iterations % 21;
    int y;
    if(iterations < 16 || iterations == 20)
    {
        y = iterations + 6;
    }
    else
    {
        y = 20-iterations;
    }
    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    Circle* c = new Circle(GetLocationXOnCoordinate(iterations), GetLocationYOnCoordinate(y), 0.03f, red);
    auto points = c->GetPoints();

    GLfloat* arrayPoints = new GLfloat[c->GetNumberOfTriangles() * 9];

    for(int i = 0; i < c->GetNumberOfTriangles() * 9; ++i)
    {
        arrayPoints[i] = points->at(i);
    }

    GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat)*RedCircleStart, sizeof(GLfloat) * c->GetNumberOfTriangles() * 9, arrayPoints));

    delete c;
    delete arrayPoints;
}

std::vector<Text> CreateXValuesText()
{
    std::vector<Text> XValuesText;

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / scaleX;
    for(int i = 1; i < scaleX + 1; ++i)
    {
        GLTtext* t = gltCreateText();
        gltSetText(t, std::to_string(i).c_str());

        GLfloat x = XCoordinateXStart + step * i;
        GLfloat y = XCoordinateYStart;

        XValuesText.push_back(Text{t, TranslateCoordinateX(x), TranslateCoordinateY(y)});
    }

    return XValuesText;
}

std::vector<Text> CreateYValuesText()
{
    std::vector<Text> YValuesText;

    //go from -0.86 to +0.88
    GLfloat step = (0.88f + 0.86f) / scaleY;
    for(int i = 1; i < scaleY + 1; ++i)
    {
        GLTtext* t = gltCreateText();
        gltSetText(t, std::to_string(i).c_str());

        GLfloat x = YCoordinateXStart;
        GLfloat y = YCoordinateYStart + step * i;

        YValuesText.push_back(Text{t, TranslateCoordinateX(x), TranslateCoordinateY(y)});
    }

    return YValuesText;
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if(nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);*/

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    gltInit();
    GLTtext* Y = gltCreateText();
    gltSetText(Y, "Y");
    GLTtext* X = gltCreateText();
    gltSetText(X, "X");

    std::vector<Text> XValuesText = CreateXValuesText();
    std::vector<Text> YValuesText = CreateYValuesText();

    CalculateTriangles();
    CalculateLines();

    GLfloat* vertices = CreateVertices();
    GLfloat* color = CreateColor();

    GLuint VBO, VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindVertexArray(VAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*(numberOfTriangles*9+numberOfLines*6), vertices, GL_STREAM_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0* sizeof(GLfloat), (GLvoid*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLuint colorbufferID;
    glGenBuffers(1, &colorbufferID);
   
    glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*(numberOfTriangles*9), color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    auto start = std::chrono::steady_clock::now();
    int fps = 0;
    int iterations = 0;
    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        auto start2 = std::chrono::steady_clock::now();
        fps++;
        glfwPollEvents();

        GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shaderProgram));
        GLCall(glBindVertexArray(VAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        ModifyCircle(iterations);
        GLCall(glDrawArrays(GL_TRIANGLES, 0, numberOfTriangles*3));
        GLCall(glDrawArrays(GL_LINES, numberOfTriangles*3, numberOfLines*2));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindVertexArray(0));

        GLCall(gltBeginDraw());

        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLCall(gltDrawText2D(Y, TranslateCoordinateX(-0.9), TranslateCoordinateY(0.97), 1));
        GLCall(gltDrawText2D(X, TranslateCoordinateX(0.95), TranslateCoordinateY(-0.87), 1));

        for(auto& t : XValuesText)
        {
            GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
        }

        for(auto& t : YValuesText)
        {
            GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
        }

        GLCall(gltEndDraw());

        glfwSwapBuffers(window);



        auto end2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::micro> elapsedSeconds2 = end2 - start2;
        if(elapsedSeconds2.count() < 14000)
        {
            int t = 14000 - elapsedSeconds2.count();
            std::this_thread::sleep_for(std::chrono::microseconds(t));
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;

        if(elapsedSeconds.count() > 1)
        {
            iterations++;
            std::cout << fps << std::endl;
            fps = 0;
            start = std::chrono::steady_clock::now();
        }
    }

    gltDeleteText(Y);
    gltDeleteText(X);

    for(auto& t : XValuesText)
    {
        gltDeleteText(t.gltText);
    }

    for(auto& t : YValuesText)
    {
        gltDeleteText(t.gltText);
    }

    gltTerminate();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    /*glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);

    glDeleteBuffers(1, &colorbufferID);*/

    glfwTerminate();

    delete vertices;
    delete color;

    return EXIT_SUCCESS;
}


