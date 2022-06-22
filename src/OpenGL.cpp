#if GRAPHICS

#include "OpenGL.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "CoordinateSystem.h"
#include "MST.h"

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

    SetInfoText();
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        SetCoordinateSystemText();
    }

    if(mMode == OpenGLMode::MST)
    {
        SetWeightText();
    }
}

void OpenGL::SetInfoText()
{
    mEAName = gltCreateText();
    gltSetText(mEAName, mEA->GetEAName().c_str());

    mCostFunctionName = gltCreateText();
    gltSetText(mCostFunctionName, mEA->GetCostFunctionName().c_str());

    mBitStringText = gltCreateText();
    mIterationsText = gltCreateText();
    mStagnationDetectionText = gltCreateText();

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

void OpenGL::SetCoordinateSystemText()
{
    mYText = gltCreateText();
    gltSetText(mYText, "Y");
    mXText = gltCreateText();
    gltSetText(mXText, "X");

    mXValuesText = mCoordinateSystem->CreateXValuesText();
    mYValuesText = mCoordinateSystem->CreateYValuesText();
}

void OpenGL::SetWeightText()
{
    mWeightText = mMST->CreateWeighTexts();
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
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        mVertices = mCoordinateSystem->CreateVertices();
        mVerticeSize = mCoordinateSystem->GetNumberOfTriangles() * 9 + mCoordinateSystem->GetNumberOfLines() * 6;
    }
    if(mMode == OpenGLMode::MST)
    {
        mVertices = mMST->GetVertices();
        mVerticeSize = mMST->GetNumberOfTriangles()*9+mMST->GetNumberOfLines()*6;
    }

    GLCall(glGenBuffers(1, &mVBO));
    GLCall(glBindVertexArray(mVAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (mVerticeSize), mVertices, GL_STREAM_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0));
    GLCall(glEnableVertexAttribArray(0));
}


void OpenGL::SetColorBuffer()
{
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        mColor = mCoordinateSystem->CreateColor();
        mColorSize = mCoordinateSystem->GetNumberOfTriangles() * 9 + mCoordinateSystem->GetNumberOfLines() * 6;
    }
    if(mMode == OpenGLMode::MST)
    {
        mColor = mMST->GetColor();
        mColorSize = mMST->GetNumberOfTriangles()*9 + mMST->GetNumberOfLines() * 6;
    }

    glGenBuffers(1, &mColorbufferID);

    glBindBuffer(GL_ARRAY_BUFFER, mColorbufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (mColorSize), mColor, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void OpenGL::Setup(EvolutionaryAlgorithm* aEA)
{
    mEA = aEA;
    if(dynamic_cast<MST*>(mEA->GetCostFunction()) != nullptr)
    {
        mMode = OpenGLMode::MST;
    }
    else
    {
        mMode = OpenGLMode::CoordinateSystem;
    }

    if(mMode == OpenGLMode::CoordinateSystem)
    {
        mCoordinateSystem = new CoordinateSystem(mEA);
    }
    
    if(mMode == OpenGLMode::MST)
    {
        mMST = new MSTVisualization(mEA);
    }

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

    if(mMode == OpenGLMode::CoordinateSystem)
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, mCoordinateSystem->GetNumberOfTriangles() * 3));
        GLCall(glDrawArrays(GL_LINES, mCoordinateSystem->GetNumberOfTriangles() * 3, mCoordinateSystem->GetNumberOfLines() * 2));
    }

    if(mMode == OpenGLMode::MST)
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, mMST->GetNumberOfTriangles() * 3));
        GLCall(glDrawArrays(GL_LINES, mMST->GetNumberOfTriangles() * 3, mMST->GetNumberOfLines() * 2));
    }

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void OpenGL::DrawText()
{
    GLCall(gltBeginDraw());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    DrawInfoText();

    if(mMode == OpenGLMode::CoordinateSystem)
    {
        GLCall(gltDrawText2D(mYText, mCoordinateSystem->TranslateCoordinateX(-0.9), mCoordinateSystem->TranslateCoordinateY(0.97), 1));
        GLCall(gltDrawText2D(mXText, mCoordinateSystem->TranslateCoordinateX(0.95), mCoordinateSystem->TranslateCoordinateY(-0.87), 1));

        for(auto& t : mXValuesText)
        {
            GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
        }

        for(auto& t : mYValuesText)
        {
            GLCall(gltDrawText2D(t.gltText, t.pixelPositionX, t.pixelPositionY, 1));
        }
    }

    if(mMode == OpenGLMode::MST)
    {
        if(mWeightVisibility)
        {
            auto bitString = mEA->GetBitString();
            for(int i = 0; i < mWeightText.size();++i)
            {
                if(bitString->at(i) == 1)
                {
                    GLCall(gltDrawText2D(mWeightText.at(i).gltText, mWeightText.at(i).pixelPositionX, mWeightText.at(i).pixelPositionY, 1));
                }
            }

            delete bitString;
        }
    }

    GLCall(gltEndDraw());
}

void OpenGL::DrawInfoText()
{
    GLCall(gltDrawText2D(mEAName, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(0.95), 1));
    GLCall(gltDrawText2D(mCostFunctionName, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(0.9), 1));

    UpdateInfoText();
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
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        auto points = mCoordinateSystem->ModifyCircle();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCoordinateSystem->GetRedCircleStart(), sizeof(GLfloat) * circleTriangles * 9, points));
        delete points;
    }

    if(mMode == OpenGLMode::MST)
    {
        GLfloat* color = new GLfloat[mMST->GetNumberOfLines() * 6];
        GLfloat* vertices = new GLfloat[mMST->GetNumberOfLines() * 6];
        auto lines = mMST->CreateLines();

        int copied = 0;
        for(auto& l : *lines)
        {
            auto verticePoints = l->GetPoints();
            for(int i = 0; i < 6; ++i)
            {
                vertices[copied + i] = verticePoints->at(i);
            }
            copied += 6;

            delete verticePoints;
        }


        GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mMST->GetNumberOfTriangles() * 9, sizeof(GLfloat) * mMST->GetNumberOfLines() * 6, vertices));

        copied = 0;
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

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, mColorbufferID));

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mMST->GetNumberOfTriangles()*9, sizeof(GLfloat) * mMST->GetNumberOfLines() * 6, color));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, mVBO));

        delete color;
        delete vertices;
    }
}

void OpenGL::UpdateInfoText()
{
    if(mEA->GetN() <= 30)
    {
        std::string str = GetBitString();

        gltSetText(mBitStringText, str.c_str());
        GLCall(gltDrawText2D(mBitStringText, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(0.75), 1));
    }

    std::string iterations = "Iterations: " + std::to_string(mEA->GetIterations());

    gltSetText(mIterationsText, iterations.c_str());
    GLCall(gltDrawText2D(mIterationsText, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(0.85), 1));

    int roundedFitnessValue;
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        roundedFitnessValue = mEA->GetFitnessValue();
    }

    if(mMode == OpenGLMode::MST)
    {
        roundedFitnessValue = (double)-1*mEA->GetFitnessValue();
    }

    std::string fitness = "Fitness: " + std::to_string(roundedFitnessValue);
    gltSetText(mFitnessValueText, fitness.c_str());
    GLCall(gltDrawText2D(mFitnessValueText, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(0.80), 1));

    if(mEA->GetStagnationDetection() != StagnationDetection::NotApplicable)
    {
        std::string str;
        if(mEA->GetStagnationDetection() == StagnationDetection::On)
        {
            str = "SD has been used";
        }
        else
        {
            str = "SD hasn't been used";
        }

        gltSetText(mStagnationDetectionText, str.c_str());

        double yCoordinate = 0.75;
        if(mEA->GetN() <= 30)
        {
            yCoordinate = 0.7;
        }

        GLCall(gltDrawText2D(mStagnationDetectionText, TranslateCoordinateX(mInfoXValue), TranslateCoordinateY(yCoordinate), 1));
    }
}

void OpenGL::ShutDown()
{
    if(mMode == OpenGLMode::CoordinateSystem)
    {
        gltDeleteText(mYText);
        gltDeleteText(mXText);
    }
    
    for(auto& t : mXValuesText)
    {
        gltDeleteText(t.gltText);
    }

    for(auto& t : mYValuesText)
    {
        gltDeleteText(t.gltText);
    }

    for(auto& t : mWeightText)
    {
        gltDeleteText(t.gltText);
    }

    gltDeleteText(mEAName);
    gltDeleteText(mCostFunctionName);
    gltDeleteText(mBitStringText);
    gltDeleteText(mIterationsText);
    gltDeleteText(mFitnessValueText);
    gltDeleteText(mStagnationDetectionText);


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
    auto bitString = mEA->GetBitString();

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


float OpenGL::TranslateCoordinateX(double aX)
{
    return (aX + 1) / 2 * OpenGL::GetWidth();
}

float OpenGL::TranslateCoordinateY(double aY)
{
    return (-aY + 1) / 2 * OpenGL::GetHeight();
}

#endif