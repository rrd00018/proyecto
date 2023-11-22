/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "3dstudio.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"

const float pi_f = 3.1415926f;

class OpenGLWindow
{
public:
    OpenGLWindow(std::string title, int width, int height);
    ~OpenGLWindow();

    GLFWwindow* window() const;
    virtual void errorCallback(int error, const char* desc);
    virtual void resizeCallback(GLFWwindow* window, int width, int height);
    virtual void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

    void start();
    virtual void initialize() = 0;
    virtual void display() = 0;
    void displayNow();

protected:
    bool checkOpenGLError() const;
    int width() const;
    int height() const;

    std::string readShaderSource(const std::string shaderFile) const;
    GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

    void reshape(const int width, const int height) const;

    std::string objFileName;
    std::string objFilePath;

    float fov = 60.0f;
    float farplane = 500.0f;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = pi_f/4.0f;

private:


    virtual void DrawGui() = 0;

    GLFWwindow* glfwWindow;
    int windowWidth = 0;
    int windowHeight = 0;
};
