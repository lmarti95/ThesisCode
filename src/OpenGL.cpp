#ifdef GRAPHICS

#include "OpenGL.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "CoordinateSystem.h"

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

void OpenGL::SetText()
{
    gltInit();
    mYText = gltCreateText();
    gltSetText(mYText, "Y");
    mXText = gltCreateText();
    gltSetText(mXText, "X");

    mEAName = gltCreateText();
    gltSetText(mEAName, mEA->GetEAName().c_str());

    mCostFunctionName = gltCreateText();
    gltSetText(mCostFunctionName, mEA->GetCostFunctionName().c_str());

    mBitStringText = gltCreateText();
    mIterationsText = gltCreateText();

    mXValuesText = mCoordinateSystem->CreateXValuesText();
    mYValuesText = mCoordinateSystem->CreateYValuesText();

    if(mEA->GetN() <= 30)
    {
        mInfoXValue = 0.65 - (mEA->GetN()/10)*0.14;
    }
    else
    {
        mInfoXValue = 0.5;
    }

    mFitnessValueText = gltCreateText();
}

void OpenGL::SetWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(mWindow, &screenWidth, &screenHeight);

    if(nullptr == mWindow)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(mWindow);

    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    std::cout << "Opengl version: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGL::SetShaders()
{
    vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout(location = 1) in vec3 color; \n"
        "out vec3 vcolor;"
        "void main()\n"
        "{\n"
        "vcolor = color;"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    fragmentShaderSource = "#version 330 core\n"
        "in vec3 vcolor;"
        "out vec3 fragColor;\n"
        "void main()\n"
        "{\n"
        "fragColor.x = vcolor.x;"
        "fragColor.y = vcolor.y;"
        "fragColor.z = vcolor.z;"
        "}\n\0";

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

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGL::SetVerticeBuffer()
{
    mVertices = mCoordinateSystem->CreateVertices();

    GLCall(glGenBuffers(1, &mVBO));
    GLCall(glBindVertexArray(mVAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (mCoordinateSystem->GetNumberOfTriangles() * 9 + mCoordinateSystem->GetNumberOfLines() * 6), mVertices, GL_STREAM_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0));
    GLCall(glEnableVertexAttribArray(0));
}


void OpenGL::SetColorBuffer()
{
    mColor = mCoordinateSystem->CreateColor();

    glGenBuffers(1, &mColorbufferID);

    glBindBuffer(GL_ARRAY_BUFFER, mColorbufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (mCoordinateSystem->GetNumberOfTriangles() * 9 + mCoordinateSystem->GetNumberOfLines() * 6), mColor, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void OpenGL::Setup(EvolutionaryAlgorithm* aEA)
{
    mEA = aEA;
    mCoordinateSystem = new CoordinateSystem(mEA);

    SetWindow();
    SetShaders();    
    SetText();

    GLCall(glGenVertexArrays(1, &mVAO));
   
    SetVerticeBuffer();
    SetColorBuffer();
}

void OpenGL::Draw()
{
    while(!glfwWindowShouldClose(mWindow) && glfwGetKey(mWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        mStart = std::chrono::steady_clock::now();

        glfwPollEvents();

        DrawVertices();
        DrawText();

        glfwSwapBuffers(mWindow);

        SetRuntime();
    }
}

void OpenGL::DrawVertices()
{
    GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUseProgram(mShaderProgram));
    GLCall(glBindVertexArray(mVAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));

    Update();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, mCoordinateSystem->GetNumberOfTriangles() * 3));
    GLCall(glDrawArrays(GL_LINES, mCoordinateSystem->GetNumberOfTriangles() * 3, mCoordinateSystem->GetNumberOfLines() * 2));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void OpenGL::DrawText()
{
    GLCall(gltBeginDraw());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    GLCall(gltDrawText2D(mYText, mCoordinateSystem->TranslateCoordinateX(-0.9), mCoordinateSystem->TranslateCoordinateY(0.97), 1));
    GLCall(gltDrawText2D(mXText, mCoordinateSystem->TranslateCoordinateX(0.95), mCoordinateSystem->TranslateCoordinateY(-0.87), 1));

    GLCall(gltDrawText2D(mEAName, mCoordinateSystem->TranslateCoordinateX(mInfoXValue), mCoordinateSystem->TranslateCoordinateY(0.95), 1));
    GLCall(gltDrawText2D(mCostFunctionName, mCoordinateSystem->TranslateCoordinateX(mInfoXValue), mCoordinateSystem->TranslateCoordinateY(0.9), 1));

    UpdateText();

    for(auto& t : mXValuesText)
    {
        GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
    }

    for(auto& t : mYValuesText)
    {
        GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
    }

    GLCall(gltEndDraw());
}

void OpenGL::SetRuntime()
{
    mEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsedSeconds = mEnd - mStart;
    if(elapsedSeconds.count() < 1000/60)
    {
        int t = 1000 / 60 - elapsedSeconds.count();
        std::this_thread::sleep_for(std::chrono::microseconds(t));
    }
}

void OpenGL::Update()
{
    auto points = mCoordinateSystem->ModifyCircle();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCoordinateSystem->GetRedCircleStart(), sizeof(GLfloat) * circleTriangles * 9, points));
    delete points;
}

void OpenGL::UpdateText()
{
    if(mEA->GetN() <= 30)
    {
        std::string str = GetBitString();

        gltSetText(mBitStringText, str.c_str());
        GLCall(gltDrawText2D(mBitStringText, mCoordinateSystem->TranslateCoordinateX(mInfoXValue), mCoordinateSystem->TranslateCoordinateY(0.75), 1));
    }

    std::string iterations = "Iterations: " + std::to_string(mCoordinateSystem->GetEvolutionaryAlgorithm()->GetIterations());

    gltSetText(mIterationsText, iterations.c_str());
    GLCall(gltDrawText2D(mIterationsText, mCoordinateSystem->TranslateCoordinateX(mInfoXValue), mCoordinateSystem->TranslateCoordinateY(0.85), 1));

    int roundedFitnessValue = mEA->GetFitnessValue();
    std::string fitness = "Fitness: " + std::to_string(roundedFitnessValue);
    gltSetText(mFitnessValueText, fitness.c_str());
    GLCall(gltDrawText2D(mFitnessValueText, mCoordinateSystem->TranslateCoordinateX(mInfoXValue), mCoordinateSystem->TranslateCoordinateY(0.80), 1));
}

void OpenGL::ShutDown()
{

    gltDeleteText(mYText);
    gltDeleteText(mXText);

    for(auto& t : mXValuesText)
    {
        gltDeleteText(t.gltText);
    }

    for(auto& t : mYValuesText)
    {
        gltDeleteText(t.gltText);
    }

    gltTerminate();

    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);

    glDeleteBuffers(1, &mColorbufferID);

    glfwTerminate();

    delete mVertices;
    delete mColor;
}

std::string OpenGL::GetBitString()
{
    auto bitString = mCoordinateSystem->GetEvolutionaryAlgorithm()->GetBitString();

    std::string str;

    if(bitString != nullptr)
    {
        for(auto& b : *bitString)
        {
            str.append(std::to_string(b));
        }
    }

    delete bitString;

    return str;
}

#endif