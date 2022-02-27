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
const int numberOfTriangles = 6+scaleX*2 +scaleY*2+ circleTriangles*3+4;

const GLfloat pi = 3.14159265f;

GLfloat location15X;
GLfloat location21Y;

GLfloat location16X;
GLfloat location5Y;

GLfloat location6Y;

GLfloat locationMaxX;
GLfloat locationMaxY;

const GLuint width = 800, height = 800;

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
        std::copy(aColor, aColor + 3, mColor+3);
        std::copy(aColor, aColor + 3, mColor+6);
    }
    
    virtual GLfloat* GetColor() = 0;
    virtual GLfloat* GetPoints() = 0;
    virtual int GetNumberOfTriangles() = 0;

protected:
    GLfloat mColor[9];
};

class Triangle : Shape{
public:
    Triangle(GLfloat* aPoints, GLfloat* aColor) : Shape(aColor)
    {
        std::copy(aPoints, aPoints + 9, mPoints);
    }

    GLfloat* GetPoints()
    {
        return mPoints;
    }

    int GetNumberOfTriangles()
    {
        return 1;
    }

    GLfloat* GetColor()
    {
        return mColor;
    }
private:
    GLfloat mPoints[9];
};

class Rectangle : Shape{
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

    GLfloat* GetPoints()
    {
        GLfloat points[18];
        std::copy(t1->GetPoints(), t1->GetPoints() + 9, points);
        std::copy(t2->GetPoints(), t2->GetPoints() + 9, points+9);
        return points;
    }

    int GetNumberOfTriangles()
    {
        return 2;
    }

    GLfloat* GetColor()
    {
        GLfloat color[18];
        std::copy(t1->GetColor(), t1->GetColor() + 9, color);
        std::copy(t2->GetColor(), t2->GetColor() + 9, color + 9);
        return color;
    }
private:
    Triangle* t1;
    Triangle* t2;
};

class Circle : Shape{
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

    GLfloat* GetPoints()
    {
        GLfloat points[circleTriangles*9];
        int i = 0;
        for(auto& t : mTriangles)
        {
            std::copy(t->GetPoints(), t->GetPoints() + 9, points+i*9);
            i++;
        }
        return points;
    }

    int GetNumberOfTriangles()
    {
        return circleTriangles;
    }

    GLfloat* GetColor()
    {
        GLfloat color[circleTriangles * 9];
        int i = 0;
        for(auto& t : mTriangles)
        {
            std::copy(t->GetColor(), t->GetColor() + 9, color + i * 9);
            i++;
        }
        return color;
    }
private:
    std::vector<Triangle*> mTriangles;
};

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
    Circle* c = new Circle(0.03f, 0.09, 0.03f, red);
    shapes->push_back((Shape*)(c));

    GLfloat black[3] = {0.0f, 0.0f, 0.0f};
    Circle* c2 = new Circle(locationMaxX, locationMaxY, 0.02f, black);
    shapes->push_back((Shape*)(c2));

    GLfloat points[12] = {
        -0.9f,  location6Y-0.05, 0.0f,
        -0.905f,  location6Y+0.005f-0.05, 0.0f,
        location15X-0.005f,  location21Y, 0.0f,
        location15X,  location21Y - 0.005f, 0.0f
    };

    shapes->push_back((Shape*)(new Rectangle(points, black)));

    GLfloat points2[12] = {
        locationMaxX - 0.003f,  -0.9f, 0.0f,
        location16X - 0.005f,  location5Y+0.045, 0.0f,
        location16X,  location5Y + 0.05f, 0.0f,
        locationMaxX+0.003,  -0.895f, 0.0f
    };

    shapes->push_back((Shape*)(new Rectangle(points2, black)));


    return shapes;
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
    

    GLfloat* vertices = new GLfloat[numberOfTriangles*9];

    int copied = 0;

    for(auto& s : *shapes)
    {
        for(int i = 0; i < s->GetNumberOfTriangles() * 9; ++i)
        {
            vertices[copied+i] = s->GetPoints()[i];
        }
        copied += s->GetNumberOfTriangles() * 9;
    }

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;

    return vertices;
}

GLfloat* CreateColor()
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

    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    Circle* c = new Circle(0.0f, 0.0f, 0.03f, red);

    shapes->push_back((Shape*)(c));

    GLfloat* color = new GLfloat[numberOfTriangles * 9];

    int copied = 0;

    for(auto& s : *shapes)
    {
        for(int i = 0; i < s->GetNumberOfTriangles() * 9; ++i)
        {
            color[copied + i] = s->GetColor()[i];
        }
        copied += s->GetNumberOfTriangles() * 9;
    }

    for(auto& s : *shapes)
    {
        delete s;
    }
    delete shapes;

    return color;
}

float TranslateCoordinateX(double aX)
{
    return (aX + 1) / 2 * width;
}

float TranslateCoordinateY(double aY)
{
    return (-aY + 1) / 2 * height;
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

        GLfloat x = -0.91f + step * i;
        GLfloat y = -0.95f;

        if(i == 15)
        {
            location15X = x;
        }
        if(i == 16)
        {
            location16X = x;
        }
        if(i == scaleX)
        {
            locationMaxX = x;
        }

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

        GLfloat x = -0.98f;
        GLfloat y = -0.87f + step * i;

        if(i == 5)
        {
            location5Y = y;
        }
        if(i == 6)
        {
            location6Y = y;
        }
        if(i == 21)
        {
            location21Y = y;
        }
        if(i == scaleY)
        {
            locationMaxY = y;
        }

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


    GLfloat* vertices = CreateVertices();
    GLfloat* color = CreateColor();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*numberOfTriangles*9, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    GLuint colorbufferID;
    glGenBuffers(1, &colorbufferID);
   

    glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*numberOfTriangles*9, color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    auto start = std::chrono::steady_clock::now();
    int fps = 0;

    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        auto start2 = std::chrono::steady_clock::now();
        fps++;
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3*numberOfTriangles);
        glBindVertexArray(0);

        gltBeginDraw();

        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(Y, TranslateCoordinateX(-0.9), TranslateCoordinateY(0.97), 1);
        gltDrawText2D(X, TranslateCoordinateX(0.95), TranslateCoordinateY(-0.87), 1);

        for(auto& t : XValuesText)
        {
            gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1);
        }

        for(auto& t : YValuesText)
        {
            gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1);
        }

        gltEndDraw();

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

    glfwTerminate();

    return EXIT_SUCCESS;
}


