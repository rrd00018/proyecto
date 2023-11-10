/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#include "openglwindow.h"

// Class for bridging between the C callback functions in glfw and C++ object
class glfwCallbackManager
{
    static OpenGLWindow* app;

    static void errorCallback(int error, const char* description)
    {
        if(app) 
            app->errorCallback(error,description);
    }
    
    static void resizeCallback(GLFWwindow* window, int width, int height)
    {
        if(app) 
            app->resizeCallback(window,width,height);
    }
    
public:
    static void initCallbacks(OpenGLWindow* glfwapp)
    {
        app = glfwapp;
        
        glfwSetErrorCallback(errorCallback);
        glfwSetFramebufferSizeCallback(app->window() , resizeCallback);
    }
};
