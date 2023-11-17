/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"

using namespace std;

// Initialize OpenGL
/**
 * @brief initializes OpenGL and the shaders
 */
void GeometryRender::initialize()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a program object with shaders
    program = initProgram("vshader.glsl", "fshader.glsl");
    // Installs the program object as part of current rendering state
    glUseProgram(program);
    GLuint locModel;
    locModel = glGetUniformLocation(program,"M");
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel);

    // Creat a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer in the shared display list space and
    // bind it as VertexBuffer (GL_ARRAY_BUFFER)
    glGenBuffers( 1, &vBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, vBuffer);

    /* Create buffer in the shared display list space and 
       bind it as GL_ELEMENT_ARRAY_BUFFER */
    glGenBuffers(1, &iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");

    glBindVertexArray(0);
    glUseProgram(0);

    loadGeometry();

}
/**
 * @brief Reload the vao with the new attribs of the figure
 */
void GeometryRender::loadGeometry(void)
{
    if(vertices.empty()) {
        // Define vertices in array
        vertices.push_back(Vec4(-0.5f, -0.75f, 0.0f, 1.0f));
        vertices.push_back(Vec4(0.0f, 0.75f, 0.0f, 1.0f));
        vertices.push_back(Vec4(0.5f, -0.75f, 0.0f, 1.0f));
        vertices.push_back(Vec4(-0.5f, 0.75f, 0.0f, 1.0f));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);
    }
    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(Vec4);
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}

// Check if any error has been reported from the shader
void GeometryRender::debugShader(void) const
{
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );
    if (logSize > 0) {
        std::cerr << "Failure in shader "  << std::endl;
        char logMsg[logSize+1];
        glGetProgramInfoLog( program, logSize, nullptr, &(logMsg[0]) );
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }

}

// Render object
/**
 * @brief display the current object located into the vertex and indexes array
 */
void GeometryRender::display()
{
    glUseProgram(program);
    glBindVertexArray(vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));


    // Not to be called in release...
    debugShader();
    if(debug) {
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << vertices[i].values[0] << " " << vertices[i].values[1] << " " << vertices[i].values[2] << endl;
        }

        for (int i = 0; i < indices.size(); i += 3) {
            std::cout << indices[i] << " " << indices[i + 1] << " " << indices[i + 2] << endl;
        }
        std::cout << "----------------------------------" << std::endl;
    }
    glBindVertexArray(0);
    glUseProgram(0);

}

/**
 * @brief Translates the vertices
 * @param x Floating units to translate in x
 * @param y Floating units to translate in y
 * @param z Floating units to translate in z
 */
void GeometryRender::translate(float x, float y, float z) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(x, y, z));

    for (int i = 0; i < vertices.size(); i++) {
        glm::vec4 vertex = glm::vec4(vertices[i].values[0], vertices[i].values[1], vertices[i].values[2], vertices[i].values[3]);
        vertex = trans * vertex;

        vertices[i].values[0] = vertex.x;
        vertices[i].values[1] = vertex.y;
        vertices[i].values[2] = vertex.z;
        vertices[i].values[3] = vertex.w;
    }
    loadGeometry();
}

/**
 * @brief Scale the vertices
 * @param x Scale in x-axis
 * @param y Scale in y-axis
 * @param z Scale in z-axis
 */
void GeometryRender::scale(float x, float y, float z) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(x, y, z));

    for (int i = 0; i < vertices.size(); i++) {
        glm::vec4 vertex = glm::vec4(vertices[i].values[0], vertices[i].values[1], vertices[i].values[2], vertices[i].values[3]);
        vertex = trans * vertex;

        vertices[i].values[0] = vertex.x;
        vertices[i].values[1] = vertex.y;
        vertices[i].values[2] = vertex.z;
        vertices[i].values[3] = vertex.w;
    }
    loadGeometry();
}


/**
 * @brief rotate in z axis direction
 * @param a degrees of rotation, cna be + or -
 */
void GeometryRender::rotatez(float a) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::degrees(a), glm::vec3(0, 0, 1));

    for (int i = 0; i < vertices.size(); i++) {
        glm::vec4 vertex = glm::vec4(vertices[i].values[0], vertices[i].values[1], vertices[i].values[2], vertices[i].values[3]);
        vertex = trans * vertex;

        vertices[i].values[0] = vertex.x;
        vertices[i].values[1] = vertex.y;
        vertices[i].values[2] = vertex.z;
        vertices[i].values[3] = vertex.w;
    }
    loadGeometry();
}


/**
 * @brief rotate in x axis direction
 * @param a degrees of rotation, cna be + or -
 */
void GeometryRender::rotatex(float a) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::degrees(a), glm::vec3(1, 0, 0));

    for (int i = 0; i < vertices.size(); i++) {
        glm::vec4 vertex = glm::vec4(vertices[i].values[0], vertices[i].values[1], vertices[i].values[2], vertices[i].values[3]);
        vertex = trans * vertex;

        vertices[i].values[0] = vertex.x;
        vertices[i].values[1] = vertex.y;
        vertices[i].values[2] = vertex.z;
        vertices[i].values[3] = vertex.w;
    }
    loadGeometry();
}


/**
 * @brief rotate in y axis direction
 * @param a degrees of rotation, cna be + or -
 */
void GeometryRender::rotatey(float a) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::degrees(a), glm::vec3(0, 1, 0));

    for (int i = 0; i < vertices.size(); i++) {
        glm::vec4 vertex = glm::vec4(vertices[i].values[0], vertices[i].values[1], vertices[i].values[2], vertices[i].values[3]);
        vertex = trans * vertex;

        vertices[i].values[0] = vertex.x;
        vertices[i].values[1] = vertex.y;
        vertices[i].values[2] = vertex.z;
        vertices[i].values[3] = vertex.w;
    }
    loadGeometry();
}

/**
 * @brief Define actions to take when a key is pressed
 * UP-> rotate y 10 \n
 * DOWN-> rotate y -10 \n
 * LEFT-> rotate x 10 \n
 * RIGHT-> rotate x -10 \n
 * L-> translate x 0.1 \n
 * J-> translate x -0.1 \n
 * I-> translate y 0.1 \n
 * K-> translate k -0.1 \n
 * NUMERIC 8-> translate z 0.1 \n
 * NUMERIC 2-> translate z -0.1 \n
 * NUMERIC 4-> rotate z 10 \n
 * NUMERIC 6-> rotate z -10 \n
 * ESCAPE-> close \n
 * O-> charge OBJ file from resources
 */
void GeometryRender::keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action != GLFW_PRESS){ //Evita que una liberacion de tecla o cualquier otra accion modifique el programa
        return;
    }
    switch(key){
        case GLFW_KEY_UP:
            rotatey(10);
            break;

        case GLFW_KEY_DOWN:
            rotatey(-10);
            break;

        case GLFW_KEY_LEFT:
            rotatex(10);
            break;

        case GLFW_KEY_RIGHT:
            rotatex(-10);
            break;
        case GLFW_KEY_L:
            translate(0.1,0,0);
            break;

        case GLFW_KEY_J:
            translate(-0.1,0,0);
            break;

        case GLFW_KEY_I:
            translate(0,0.1,0);
            break;

        case GLFW_KEY_K:
            translate(0,-0.1,0);
            break;

        case GLFW_KEY_KP_8:
            translate(0,0,0.1);
            break;

        case GLFW_KEY_KP_2:
            translate(0,0,-0.1);

        case GLFW_KEY_KP_4:
            rotatez(10);
            break;

        case GLFW_KEY_KP_6:
            rotatez(-10);
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GLFW_TRUE);
            break;

        case GLFW_KEY_O:
            std::cout << "Write file name: " << std::endl;
            std::string file;
            std::getline(std::cin,file);
            loadObjFile(file);
            break;
    }
}

/**
 * @brief Load an OBJ file into the vertex array
 * @param fileName name of the file
 * @post FILE IS LOCATED IN THE RESOURCES FOLDER
 */
void GeometryRender::loadObjFile(std::string fileName) {
    std::string nombre = "../resources/"+fileName;
    std::ifstream file(nombre);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << nombre << std::endl;
    }

    vertices.clear();
    indices.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        // Lee la primera palabra de la línea para determinar el tipo de entrada
        std::string type;
        iss >> type;

        // Procesa la entrada basada en su tipo
        if (type == "v") {
            // Lee las coordenadas de un vértice y crea un objeto Vec4 para representarlo
            Vec4 vertex(0, 0, 0, 1);
            iss >> vertex.values[0] >> vertex.values[1] >> vertex.values[2];
            vertices.push_back(vertex);
        } else if (type == "f") {
            // Lee los índices de una cara y los agrega al vector de índices
            std::stringstream linea(line);
            std::string values = line.substr(2,line.size()-2); //Coge los datos eliminando el prefijo
            std::stringstream ss(values);
            std::vector<string> v = {"","",""};
            for(int i = 0; i < v.size(); i++) {
                getline(ss, v[i], ' '); //Carga en cada posicion del vetor v una de las tuplas de datos
                std::stringstream s1(v[i]);
                getline(s1, v[i], '/'); //Cuando llega a la primera barra para para coger el indice
                int index = stoi(v[i]);
                indices.push_back(index-1);
            }

        } else if (type == "vn"){

        } else if (type == "vt"){

        }
    }
    file.close();


    //Calculate scalate factor as 1/maxDimension
    float maxDimension = std::max(std::max(verticesDimension(vertices, 0), verticesDimension(vertices, 1)), verticesDimension(vertices, 2));
    float scaleFactor = 1.0f / maxDimension;

    //Apply the scalate factor to each coordinate of vertexs
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].values[0] *= scaleFactor;
        vertices[i].values[1] *= scaleFactor;
        vertices[i].values[2] *= scaleFactor;
    }

    loadGeometry();
    display();
}

/**
 * @brief used to calculate the maximum dimension of the vertices in a specific dimension
 * @param vertices vector of vertices
 * @param dimension 0-> x 1->y 2->z
 * @return float value of the dimension
 */
float GeometryRender::verticesDimension(const std::vector<Vec4>& vertices, int dimension) {
    float maxCoord = std::numeric_limits<float>::lowest();
    float minCoord = std::numeric_limits<float>::max();

    for (const auto& vertex : vertices) {
        if (vertex.values[dimension] > maxCoord) {
            maxCoord = vertex.values[dimension];
        }
        if (vertex.values[dimension] < minCoord) {
            minCoord = vertex.values[dimension];
        }
    }

    return maxCoord - minCoord;
}