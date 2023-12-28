/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    virtual void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos);


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
    std::string textureFileName;
    std::string textureFilePath;
    bool textureShow = false;

    float fov = 60.0f;
    float farplane = 500.0f;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = pi_f/4.0f;
    float aspectRatio = 16/9;
    float nearPlane = 0.1f;
    float right = 0;
    float left = 0;
    float bottom = 0;
    float translationSpeed = 0.1f;

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

    glm::vec3 materialAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 materialDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

    float materialShininess = 1.0f;


private:


    virtual void DrawGui() = 0;

    GLFWwindow* glfwWindow;
    int windowWidth = 0;
    int windowHeight = 0;
};
