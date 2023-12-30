/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize() override;
    virtual void display() override;

private:
    GLuint program;

    bool debug = false;
    bool mouse = false;
    bool firsClick = false;
    int degrees = 0;
    float posX;
    float posY;
    float cameraSpeed = 1.0f;
    float yaw = -90,pitch = 0.0f;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;
    GLuint locNormals;

    // Geometry data
    std::vector<Vec4> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;

    GLuint locModel;
    GLuint locView;
    GLuint locProjection;

    //Shader used
    GLuint locCameraPosition;
    GLuint locLightPos;
    GLuint locLightColor;
    GLuint locAmbientColor;
    GLuint locMaterialAmbient;
    GLuint locMaterialDiffuse;
    GLuint locMaterialSpecular;
    GLuint locMaterialShininess;

    //SHader used to control
    GLuint locIlluminationModel;
    int illuminationModel;


    void debugShader(void) const;

    void loadGeometry(void);

    glm::mat4 matModel;
    glm::mat4 matView;
    glm::mat4 matProjection;

    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    glm::vec3 cameraPosition;
    glm::vec3 lookAt;
    glm::vec3 upVector;

    void translate(float x, float y, float z);

    void scale(float x, float y, float z);

    void rotate(float a, int axis);

    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) override;

    void loadObjFile();

    static float verticesDimension( const std::vector<Vec4>& vertices, int dimension);

    virtual void DrawGui() override;

    std::vector<float> getOrigin(const std::vector<Vec4>& vertices);

    void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) override;

    void computeCameraMouse(float x, float y);
};
