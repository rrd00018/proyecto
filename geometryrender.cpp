/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umeå University
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

    //Get matrix locations for each matrix
    locModel = glGetUniformLocation(program,"M");
    locView = glGetUniformLocation(program, "V");
    locProjection = glGetUniformLocation(program, "P");
    locCameraPosition = glGetUniformLocation(program,"cameraPosition");
    locAmbientColor = glGetUniformLocation(program,"ambientColor");
    locLightPos = glGetUniformLocation(program,"lightPos");
    locLightColor = glGetUniformLocation(program,"lightColor");
    locMaterialAmbient = glGetUniformLocation(program,"materialAmbient");
    locMaterialDiffuse = glGetUniformLocation(program,"materialDiffuse");
    locMaterialSpecular = glGetUniformLocation(program,"materialSpecular");
    locMaterialShininess = glGetUniformLocation(program,"materialShininess");

    //Initialize illuminationModel
    locIlluminationModel = glGetUniformLocation(program,"illuminationModel");
    illuminationModel = 0;
    glUniform1i(locIlluminationModel,illuminationModel);

    //Initializes matrixes
    matModel = glm::mat4(1.0f);
    matProjection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farplane);
    cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f); //punto de referencia
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    matView = glm::lookAt(cameraPosition, lookAt, upVector);

    //Load the matrixes into the shader
    glUniformMatrix4fv(locModel, 1, GL_TRUE,glm::value_ptr(matModel));
    glUniformMatrix4fv(locView,1,GL_TRUE,glm::value_ptr(matView));
    glUniformMatrix4fv(locProjection,1,GL_TRUE,glm::value_ptr(matProjection));

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
    locNormals = glGetAttribLocation(program,"vNormal");

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
        objFilePath = "/home/rafa/CLionProjects/proyecto/resources";
        objFileName = "suzanne.obj";
        loadObjFile();
    }
    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(Vec4);
    size_t iSize = indices.size()*sizeof(unsigned int);
    size_t nSize = normals.size()*sizeof(glm::vec3);

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );
    glBufferData(GL_ARRAY_BUFFER, vSize + nSize, NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize,vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize,normals.data());

    //Set the pointers of locNormal to the right places
    glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
    glEnableVertexAttribArray(locNormals);

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

    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(matProjection));
    glUniform3fv(locCameraPosition,1,glm::value_ptr(cameraPosition));
    glUniform1f(locMaterialShininess,materialShininess);
    glUniform3fv(locCameraPosition, 1, glm::value_ptr(cameraPosition));
    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPos));
    glUniform3fv(locLightColor, 1, glm::value_ptr(lightColor));
    glUniform3fv(locAmbientColor, 1, glm::value_ptr(ambientColor));
    glUniform3fv(locMaterialAmbient, 1, glm::value_ptr(materialAmbient));
    glUniform3fv(locMaterialDiffuse, 1, glm::value_ptr(materialDiffuse));
    glUniform3fv(locMaterialSpecular, 1, glm::value_ptr(materialSpecular));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();
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
    glm::mat4 tempModel = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    //matModel = glm::translate(matModel, glm::vec3(x, y, z));
    matModel = tempModel * matModel;
}

/**
 * @brief Scale the vertices
 * @param x Scale in x-axis
 * @param y Scale in y-axis
 * @param z Scale in z-axis
 */
void GeometryRender::scale(float x, float y, float z) {
    matModel = glm::scale(matModel, glm::vec3(x, y, z));
}

/**
 * @brief rotate in axis direction
 * @param a degrees of rotation, cna be + or -
 * @param axis axis to rotate around, 1->x 2->y 3->z
 */
void GeometryRender::rotate(float a,int axis) {
    if(axis == 1)
        matModel = glm::rotate(matModel, glm::radians(a), glm::vec3(1, 0, 0));
    else if(axis == 2)
        matModel = glm::rotate(matModel, glm::radians(a), glm::vec3(0, 1, 0));
    else if(axis == 3)
        matModel = glm::rotate(matModel, glm::radians(a), glm::vec3(0, 0, 1));
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


     cameraRight = glm::vec3(matView[0][0], matView[1][0], matView[2][0]);
     cameraUp = glm::vec3(matView[0][1], matView[1][1], matView[2][1]);
     cameraFront = -glm::vec3(matView[0][2], matView[1][2], matView[2][2]);


    switch(key){
        case GLFW_KEY_UP:
            rotate(10,2);
            break;

        case GLFW_KEY_DOWN:
            rotate(-10,2);
            break;

        case GLFW_KEY_LEFT:
            rotate(10,1);
            break;

        case GLFW_KEY_RIGHT:
            rotate(-10,1);
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
            rotate(10,3);
            break;

        case GLFW_KEY_KP_6:
            rotate(-10,3);
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window,GLFW_TRUE);
            break;

        case GLFW_KEY_O:
            std::cin >> objFileName;
            loadObjFile();
            break;

        case GLFW_KEY_W:
            // Move forward along the camera's negative z-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(0,0,-0.1f,1);
            lookAt = glm::inverse(matView) * glm::vec4(0,0,-1-0.1f,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            break;

        case GLFW_KEY_S:
            // Move backward along the camera's positive z-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(0,0,+0.1f,1);
            lookAt = glm::inverse(matView) * glm::vec4(0,0,-1+0.1f,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
            break;

        case GLFW_KEY_A:
            // Move left along the camera's negative x-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(-0.1f,0, 0,1);
            lookAt = glm::inverse(matView) * glm::vec4(-0.1f,0,-1,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
            break;

        case GLFW_KEY_D:
            // Move right along the camera's positive x-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(0.1f, 0, 0,1);
            lookAt = glm::inverse(matView) * glm::vec4(+0.1f,0,-1,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
            break;

        case GLFW_KEY_E:
            // Move up along the camera's positive y-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(0, 0.1f, 0,1);
            lookAt = glm::inverse(matView) * glm::vec4(0,0.1f,-1,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
            break;

        case GLFW_KEY_Q:
            // Move down along the camera's negative y-axis
            cameraPosition = glm::inverse(matView) * glm::vec4(0, -0.1f, 0,1);
            lookAt = glm::inverse(matView) * glm::vec4(0,-0.1f,-1,1);
            matView=glm::lookAt(cameraPosition,lookAt,upVector);
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
            break;

        case GLFW_KEY_LEFT_SHIFT:
            if(action != GLFW_PRESS) return;
            if(mouse) {
                mouse = false;
                std::cout << "Mouse control OFF" << std::endl;
            }else {
                mouse = true;
                double x;
                double y;
                glfwGetCursorPos(window, &x, &y);
                posX = x;
                posY = y;
                std::cout << "Mouse control ON" << std::endl;
            }
            break;
    }
}

/**
 * @brief Load an OBJ file into the vertex array
 * @param fileName name of the file
 * @post FILE IS LOCATED IN THE RESOURCES FOLDER
 */
void GeometryRender::loadObjFile() {
    std::string nombre = objFilePath+"/"+objFileName;
    std::ifstream file(nombre);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << nombre << std::endl;
    }

    vertices.clear();
    indices.clear();

    std::vector<glm::vec3> normalesArchivo;
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
            if(normals.size() < vertices.size()){
                for(int i = normals.size(); i  < vertices.size(); i++){
                    normals.push_back(glm::vec3(1.0f));
                }
            }
            // Lee los índices de una cara y los agrega al vector de índices
            std::string values = line.substr(2,line.size()-2); //Coge los datos eliminando el prefijo
            //std::cout << values << endl;
            std::stringstream ss(values);
            std::vector<string> v;
            std::string token;
            while(getline(ss,token,' ')){
                v.push_back(token);
                //std::cout << token << std::endl;
            }
            /*
            for(int i = 0; i < v.size(); i++){
                std::cout << v[i] << " || ";
            }

            std::cout << "->"<< endl;
            */
            if(v.size() == 3) {
                for (int i = 0; i < v.size(); i++) {
                    std::stringstream s1(v[i]);
                    getline(s1, v[i], '/'); //Cuando llega a la primera barra para para coger el indice
                    int index = stoi(v[i]);
                    indices.push_back(index - 1);
                    if(!normalesArchivo.empty()){
                        size_t pos = v[i].find_last_of('/');
                        std::string normalVertice = v[i].substr(pos+1);
                        normals[index-1] = normalesArchivo[stoi(normalVertice)];
                    }

                }
            }else if(v.size() == 4){
                for (int i = 0; i < 3; i++) {
                    std::stringstream s1(v[i]);
                    getline(s1, v[i], '/'); //Cuando llega a la primera barra para para coger el indice
                    int index = stoi(v[i]);
                    indices.push_back(index - 1);
                    if(!normalesArchivo.empty()){
                        size_t pos = v[i].find_last_of('/');
                        std::string normalVertice = v[i].substr(pos+1);
                        normals[index-1] = normalesArchivo[stoi(normalVertice)];
                    }
                }
                for (int i = 2; i < 5; i++) {
                    std::stringstream s1(v[i%4]);
                    getline(s1, v[i%4], '/'); //Cuando llega a la primera barra para para coger el indice
                    int index = stoi(v[i%4]);
                    indices.push_back(index - 1);
                    if(!normalesArchivo.empty()){
                        size_t pos = v[i%4].find_last_of('/');
                        std::string normalVertice = v[i%4].substr(pos+1);
                        normals[index-1] = normalesArchivo[stoi(normalVertice)];
                    }
                }
            }

        } else if (type == "vn"){
            glm::vec3 vertex(0, 0, 0);
            iss >> vertex.x >> vertex.y >> vertex.z;
            normalesArchivo.push_back(vertex);
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

    //Calcula las normales
    if(normalesArchivo.empty()){
        std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));

        // Iterar a través de los índices en grupos de 3 (triángulos)
        for (size_t i = 0; i < indices.size(); i += 3) {
            // Obtener los índices de los vértices del triángulo actual
            unsigned int i1 = indices[i];
            unsigned int i2 = indices[i + 1];
            unsigned int i3 = indices[i + 2];

            // Obtener los vértices del triángulo actual
            glm::vec3 v1 = {vertices[i1].values[0],vertices[i1].values[1],vertices[i1].values[2]};
            glm::vec3 v2 = {vertices[i2].values[0],vertices[i2].values[1],vertices[i2].values[2]};
            glm::vec3 v3 = {vertices[i3].values[0],vertices[i3].values[1],vertices[i3].values[2]};

            // Calcular el vector normal del triángulo actual
            glm::vec3 normalTriangulo = glm::cross(v2 - v1, v3 - v1);

            // Sumar la normal calculada a los vértices correspondientes
            normals[i1] += normalTriangulo;
            normals[i2] += normalTriangulo;
            normals[i3] += normalTriangulo;
        }

        // Normalizar los vectores de normales
        for (size_t i = 0; i < normals.size(); ++i) {
            normals[i] = glm::normalize(normals[i]);
        }

    }

    //Reset matrixes
    matModel = glm::mat4 (1.0f);
    cameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f); //punto de referencia
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    matView = glm::lookAt(cameraPosition, lookAt, upVector);

    loadGeometry();
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


/**
 * @brief Calcula la translacion necesaria para llevar una figura al origen
 * @param vertices vector de vertices de la figura
 * @return
 */
std::vector<float> GeometryRender::getOrigin(const std::vector<Vec4>& vertices){
    // Calcular el centro de masa de la figura
    std::vector<float> center= {0.0f, 0.0f, 0.0f};
    for (const auto& vertex : vertices) {
        center[0] += vertex.values[0];
        center[1] += vertex.values[1];
        center[2] += vertex.values[2];
    }
    for(int i = 0; i < center.size(); i++){
        center[i] /= static_cast<float>(vertices.size());
    }
    return center;
}

void GeometryRender::DrawGui() {
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");

        static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
        static ImGuiFileDialog fileDialog;
        static ImGuiFileDialog textureDialog;

        ImGui::Begin("3D Studio");

        if (ImGui::CollapsingHeader("OBJ File")) {
            ImGui::Text("OBJ file: %s", objFileName.c_str());
            if (ImGui::Button("Open File"))
                fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");

            if (fileDialog.Display("ChooseFileDlgKey")) {
                if (fileDialog.IsOk()) {
                    objFileName = fileDialog.GetCurrentFileName();
                    objFilePath = fileDialog.GetCurrentPath();
                    loadObjFile();
                } else {
                    std::cerr <<  "Error opening obj file ->" << objFileName << std::endl;
                }
                fileDialog.Close();
            }
        }

        if (ImGui::CollapsingHeader("Light")) {
            ImGui::Text("Light source position");
            ImGui::PushItemWidth(100);
            ImGui::InputFloat3("Light Pos", glm::value_ptr(lightPos), "%.1f");
            ImGui::PopItemWidth();

            ImGui::Text("Light source intensity:");
            ImGui::ColorEdit3("Light", glm::value_ptr(lightColor));
            ImGui::Text("Ambient light intensity:");
            ImGui::ColorEdit3("Ambient", glm::value_ptr(ambientColor));
        }

        if (ImGui::CollapsingHeader("Object Material")) {
            ImGui::Text("Ambient coefficient:");
            ImGui::ColorEdit3("Ambient color", glm::value_ptr(materialAmbient));

            ImGui::Text("Diffuse coefficient:");
            ImGui::ColorEdit3("Diffuse color", glm::value_ptr(materialDiffuse));

            ImGui::Text("Specular coefficient:");
            ImGui::ColorEdit3("Specular color", glm::value_ptr(materialSpecular));

            ImGui::SliderFloat("Shininess", &materialShininess, 1.0f, 1000.0f, "%1.0f", flags);
        }

        if (ImGui::CollapsingHeader("Object Texture")) {
            ImGui::Checkbox("Show texture", &textureShow);
            ImGui::Text("Texture file: %s", textureFileName.c_str());
            if (ImGui::Button("Open Texture File"))
                textureDialog.OpenDialog("ChooseFileDlgKey", "Choose Texture File",
                                         ".jpg,.bmp,.dds,.hdr,.pic,.png,.psd,.tga", ".");

            if (textureDialog.Display("ChooseFileDlgKey")) {
                if (textureDialog.IsOk()) {
                    textureFileName = textureDialog.GetCurrentFileName();
                    textureFilePath = textureDialog.GetCurrentPath();
                    cout << "Texture file: " << textureFileName << endl << "Path: " << textureFilePath << endl;
                } else {
                    // Return a message to the user if the file could not be opened
                }
                // close
                textureDialog.Close();
            }
        }

    if (ImGui::CollapsingHeader("Projection")) {
        const char* items[] = {"Perspective", "Parallel" };
        static int proj_current_idx = 0;
        if (ImGui::Combo("projektion", &proj_current_idx, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)));
        if (proj_current_idx == 0) {
            ImGui::SliderFloat("Field of view",&fov, 20.0f, 160.0f, "%1.0f", flags);
            ImGui::SliderFloat("Far",&farplane, 1.0f, 1000.0f, "%1.0f", flags);

            aspectRatio = (float) width()/height();
            matProjection = glm::perspective(glm::radians(fov), aspectRatio, farplane, nearPlane);

            if(debug) {
                std::cout << "Projection Matrix:" << std::endl;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        std::cout << matProjection[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << "View Matrix:" << std::endl;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        std::cout << matView[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << "Model Matrix:" << std::endl;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        std::cout << matModel[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << aspectRatio << endl;
                std::cout << "----------------------------------" << std::endl;

            }
        }
        if (proj_current_idx == 1) {
            ImGui::SliderFloat("Top",&top, 1.0f, 100.0f, "%.1f", flags);
            ImGui::SliderFloat("Far",&farplane, 1.0f, 1000.0f, "%1.0f", flags);
            ImGui::SliderFloat("Oblique scale",&obliqueScale, 0.0f, 1.0f, "%.1f", flags);
            ImGui::SliderAngle("Oblique angle",&obliqueAngleRad, 15, 75, "%1.0f", flags);

            aspectRatio = (float) width()/height();
            right = top * aspectRatio;
            left = -right;
            bottom = -top;

            matProjection = glm::ortho(left, right, bottom, top, farplane, nearPlane);

            // Apply oblique projection if needed
            if (obliqueScale > 0.0f) {
                glm::mat4 H = glm::mat4(1.0f);
                H[2][0] = obliqueScale * cos(obliqueAngleRad);
                H[2][1] = obliqueScale * sin(obliqueAngleRad);
                matProjection = matProjection * H;
            }
            glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(matProjection));

        }
    }

    ImGui::End();
    }

void GeometryRender::cursorPositionCallBack(GLFWwindow *window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window,xpos,ypos);
    if(mouse) {
        computeCameraMouse(xpos,ypos);
        posX = xpos;
        posY = ypos;
    }
}

void GeometryRender::computeCameraMouse(float x, float y) {

    double deltaX = x - posX;
    double deltaY = posY - y;
    yaw += deltaX * cameraSpeed;
    pitch += deltaY * cameraSpeed;
   /* float rotX = cameraSpeed * (float)(y - (height() / 2)) / height();
    float rotY = cameraSpeed * (float)(x - (width() / 2)) / width();
    glm::vec3 newOrientation = glm::rotate(lookAt, glm::radians(-rotX), glm::normalize(glm::cross(lookAt, upVector)));*/

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    lookAt = cameraPosition + glm::normalize(direction);

    //Debug text
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Yaw: " << yaw << " Pitch: " << pitch << std::endl;
    std::cout << "X: " << cos(glm::radians(yaw)) << " " << cos(glm::radians(pitch)) << std::endl;
    std::cout << "Z: " << sin(glm::radians(yaw)) << " " << cos(glm::radians(pitch)) << std::endl;
    std::cout << "Camera position: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
    std::cout << "Direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
    std::cout << "Up vector: " << upVector.x << " " << upVector.y << " " << upVector.z << std::endl;
    std::cout << "Look at: " << lookAt.x << " " << lookAt.y << " " << lookAt.z << std::endl;
    std::cout << "Matrices: " << std::endl;
    matView = glm::lookAt(cameraPosition,lookAt,upVector);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            std::cout << matView[j][i] << " ";
        }
        std::cout << std::endl;
    }

}