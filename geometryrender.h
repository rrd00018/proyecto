/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umeå University
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
#include "stb_image.h"

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

    bool debug = true;
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
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec2> textureCoordinates;

    GLuint locModel;
    GLuint locView;
    GLuint locProjection;

    //Lightning used
    GLuint locCameraPosition;
    GLuint locLightPos;
    GLuint locLightColor;
    GLuint locAmbientColor;
    GLuint locMaterialAmbient;
    GLuint locMaterialDiffuse;
    GLuint locMaterialSpecular;
    GLuint locMaterialShininess;

    //Shader used to control
    GLuint locIlluminationModel;
    int illuminationModel;

    //Texture
    GLuint locTexture;
    GLuint locTextureCoordinates;


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

    static float verticesDimension( const std::vector<glm::vec4>& vertices, int dimension);

    virtual void DrawGui() override;

    glm::vec4 getOrigin(const std::vector<glm::vec4>& vertices);

    void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) override;

    void computeCameraMouse(float x, float y);

    GLuint loadTexture(const std::string& filePath);

    void activateTexture();

    void deactivateTexture();


    std::string objFileName;
    std::string objFilePath;
    std::string textureFileName;
    std::string textureFilePath;
    int textureShow = 1;
    GLuint locTextureShow;

    float fov = 60.0f;
    float farplane = 500.0f;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = pi_f/4.0f;
    float aspectRatio = (float) width()/height();
    float nearPlane = 0.1f;
    float right = 0;
    float left = 0;
    float bottom = 0;
    float translationSpeed = 0.1f;

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

    glm::vec3 materialAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 materialDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

    float materialShininess = 1.0f;

};
