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

    static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods){
        if(app)
            app->keyCallBack(window,key,scancode,action,mods);
    }

    static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos){
        if(app)
            app->cursorPositionCallBack(window,xpos,ypos);
    }
    static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods){
        if(app)
            app->mouseButtonCallBack(window,button,action,mods);
    }
    
public:
    static void initCallbacks(OpenGLWindow* glfwapp)
    {
        app = glfwapp;
        
        glfwSetErrorCallback(errorCallback);
        glfwSetFramebufferSizeCallback(app->window() , resizeCallback);
        glfwSetKeyCallback(app->window(),keyCallBack);
        glfwSetCursorPosCallback(app->window(), cursorPositionCallBack);
    }
};
