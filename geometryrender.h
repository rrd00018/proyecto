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
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

typedef float Mat4x4[16];

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;

private:
    GLuint program;

    bool debug = false;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;

    // Geometry data
    std::vector<Vec4> vertices;
    std::vector<unsigned int> indices;

    GLuint locModel;

    void debugShader(void) const;

    void loadGeometry(void);

    glm::mat4 matModel;

    void translate(float x, float y, float z);

    void scale(float x, float y, float z);

    void rotatez(float a);

    void rotatex(float a);

    void rotatey(float a);

    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) override;

    void loadObjFile(std::string fileName);

    static float verticesDimension( const std::vector<Vec4>& vertices, int dimension);

    std::vector<float> getOrigin(const std::vector<Vec4>& vertices);
};
