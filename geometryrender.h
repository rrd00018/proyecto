/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>

typedef float Mat4x4[16];

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;
    void processIput(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    GLuint program;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;

    // Geometry data
    std::vector<Vec4> vertices;
    std::vector<unsigned int> indices;

    void debugShader(void) const;

    void loadGeometry(void);

    Mat4x4 matModel = {1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0};

    void translate(float x, float y, float z);

    void scale(float x, float y, float z);

    void rotatez(float a);

    void rotatex(float a);

    void rotatey(float a);

    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) override;

    void loadObjFile(std::string fileName);

    float verticesDimension( const std::vector<Vec4>& vertices, int dimension);
};
