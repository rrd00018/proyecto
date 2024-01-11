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
    glEnable(GL_CULL_FACE);
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

    //Initialize the texture
    textureShow = false;
    textureFileName = "";
    textureFilePath = "";


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

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");
    locNormals = glGetAttribLocation(program,"vNormal");
    locTextureCoordinates = glGetAttribLocation(program,"textureCoordinates");
    locTextureShow = glGetAttribLocation(program,"textureShow");

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
        objFileName = "sphere_large.obj";
        loadObjFile();
    }

    if(locTexture == 0) {
        if (textureFilePath == "") textureFilePath = "/home/rafa/CLionProjects/proyecto/resources";
        if (textureFileName == "")
            textureFileName = "bricks.bmp";
        locTexture = loadTexture(textureFilePath + "/" + textureFileName);

    }

    glUseProgram(program);
    glBindVertexArray(vao);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(glm::vec4);
    size_t nSize = normals.size()*sizeof(glm::vec4);
    size_t tSize = textureCoordinates.size()*sizeof(glm::vec2);

    //Buffer de vertices-> Contiene posicion, normal, textura
    glBufferData(GL_ARRAY_BUFFER, vSize + nSize + tSize, NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize,vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize,normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, nSize+vSize, tSize,textureCoordinates.data());

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    //Set the pointers of locNormal to the right places
    glVertexAttribPointer(locNormals, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
    glEnableVertexAttribArray(locNormals);

    //Set the pointers of locTextureCoordinates to the right place
    glVertexAttribPointer(locTextureCoordinates, 2, GL_FLOAT,GL_FALSE,0, BUFFER_OFFSET(vSize+nSize));
    glEnableVertexAttribArray(locTextureCoordinates);

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

    //Matrixes to shader
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(matView));
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(matProjection));

    //Lightning things to shader
    glUniform1f(locMaterialShininess,materialShininess);
    glUniform3fv(locCameraPosition, 1, glm::value_ptr(cameraPosition));
    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPos));
    glUniform3fv(locLightColor, 1, glm::value_ptr(lightColor));
    glUniform3fv(locAmbientColor, 1, glm::value_ptr(ambientColor));
    glUniform3fv(locMaterialAmbient, 1, glm::value_ptr(materialAmbient));
    glUniform3fv(locMaterialDiffuse, 1, glm::value_ptr(materialDiffuse));
    glUniform3fv(locMaterialSpecular, 1, glm::value_ptr(materialSpecular));
    glUniform1i(locIlluminationModel,illuminationModel);

    //Texture things to shader
    glUniform1i(glGetUniformLocation(program, "myTexture"), 0);
    glUniform2fv(locTextureCoordinates,1,glm::value_ptr(textureCoordinates[0]));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(illuminationModel == 2)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    if(textureShow)
        activateTexture();

    // Call OpenGL to draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // Not to be called in release...
    debugShader();

    deactivateTexture();

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
    std::vector<glm::vec4> verticesFinal;
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << nombre << std::endl;
    }

    vertices.clear();
    normals.clear();
    textureCoordinates.clear();

    std::vector<glm::vec4> normalsFinal;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        // Lee la primera palabra de la línea para determinar el tipo de entrada
        std::string type;
        iss >> type;

        // Procesa la entrada basada en su tipo
        if (type == "v") {
            // Lee las coordenadas de un vértice y crea un objeto Vec4 para representarlo
            glm::vec4 vertex(0, 0, 0, 1);
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (type == "f") {
            // Lee los índices de una cara y los agrega al vector de índices
            std::string values = line.substr(2,line.size()-2); //Coge los datos eliminando el prefijo
            std::stringstream ss(values);
            std::vector<string> v;
            std::string token;
            while(getline(ss,token,' ')){
                v.push_back(token);
            }

            if(v.size() == 3) {
                for (size_t i = 0; i < 3; i++) {
                    std::stringstream s1(v[i]);
                    std::string index;
                    getline(s1, index, '/');
                    int vertexIndex = stoi(index) -1;
                    //Add to the vertex vector the vetex specified by the index (duplicate vertexs)
                    verticesFinal.push_back(vertices[vertexIndex]);
                    //If normals have been read from the file
                    if(!normals.empty()){
                        size_t pos = v[i].find_last_of('/')+1;
                        int normalVertexIndex = stoi(v[i].substr(pos)) - 1;
                        //Adds to the normals vector the normal corresponding to that vertex
                        normalsFinal.push_back(normals[normalVertexIndex]);
                    }
                }
            }else if(v.size() == 4){
                for (int i = 0; i < 3; i++) {
                    std::stringstream s1(v[i]);
                    std::string index;
                    getline(s1, index, '/');
                    int vertexIndex = stoi(index)-1;
                    verticesFinal.push_back(vertices[vertexIndex]);
                    if(!normals.empty()){
                        size_t pos = v[i].find_last_of('/')+1;
                        int normalVertexIndex = stoi(v[i].substr(pos)) - 1;
                        //Adds to the normals vector the normal corresponding to that vertex
                        normalsFinal.push_back(normals[normalVertexIndex]);
                    }
                }
                for (int i = 2; i < 5; i++) {
                    std::stringstream s1(v[i%4]);
                    std::string index;
                    getline(s1, index, '/');
                    int vertexIndex = stoi(index)-1;
                    verticesFinal.push_back(vertices[vertexIndex]);
                    if(!normals.empty()){
                        size_t pos = v[i%4].find_last_of('/') + 1;
                        int normalVertexIndex = stoi(v[i%4].substr(pos)) -1;
                        normalsFinal.push_back(normals[normalVertexIndex]);
                    }
                }
            }

        } else if (type == "vn"){
            glm::vec4 normal(0, 0, 0,0);
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "vt"){
            /*glm::vec2 texture(0, 0);
            iss >> texture.x >> texture.y;
            textureCoordinates.push_back(texture);*/
        }
    }
    file.close();

    vertices = verticesFinal;

    //Calculate scalate factor as 1/maxDimension
    float maxDimension = std::max(std::max(verticesDimension(vertices, 0), verticesDimension(vertices, 1)), verticesDimension(vertices, 2));
    float scaleFactor = 1.0f / maxDimension;

    //Apply the scalate factor to each coordinate of vertexs
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x *= scaleFactor;
        vertices[i].y *= scaleFactor;
        vertices[i].z *= scaleFactor;
    }

    //Normal Calculation
    if(normals.empty()){
        normals = std::vector<glm::vec4>(vertices.size(), glm::vec4(0.0f));
        //Iterate through the triangles
        for (size_t i = 0; i < vertices.size(); i += 3) {
            glm::vec3 v0 = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
            glm::vec3 v1 = glm::vec3(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
            glm::vec3 v2 = glm::vec3(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);

            glm::vec3 eje1 = v1 - v0;
            glm::vec3 eje2 = v2 - v0;

            glm::vec3 normal = glm::cross(eje1,eje2);

            for(int j = 0; j < 3; j++){
                normals[i+j] = glm::vec4(normal,0); //w = 0 as is a vector
            }
        }
        // Normalize the normals
        for (size_t i = 0; i < normals.size(); i++) {
            normals[i] = glm::vec4(glm::normalize(normals[i]));
        }
    }else{
        normals = normalsFinal;
    }



    //Texture coordinates calculation
    for(size_t i = 0; i < vertices.size(); i++){
        float s = glm::acos(vertices[i].x/(1/sqrt(3)))/M_PI; //Assume radius is 1/sqrt(3) so sphere is large enough to contain a cube of 2x2 (NDC)
        float t = (glm::atan(vertices[i].z/vertices[i].y)/M_PI)+0.5;
        textureCoordinates.push_back(glm::vec2(s,t));
    }

    //Reset matrixes
    matModel = glm::mat4 (1.0f);
    cameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    matView = glm::lookAt(cameraPosition, lookAt, upVector);
/*
    std::cout << "Imprimiendo coordenadas de las normales: " << normals.size() << std::endl;
    for(size_t i = 0; i < normals.size(); i++){
        std::cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Imprimiendo coordenadas de los vertices: " << vertices.size() << std::endl;
    for(size_t i = 0; i < vertices.size(); i++){
        std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }
*/
    loadGeometry();
}

/**
 * @brief used to calculate the maximum dimension of the vertices in a specific dimension
 * @param vertices vector of vertices
 * @param dimension 0-> x 1->y 2->z
 * @return float value of the dimension
 */
float GeometryRender::verticesDimension(const std::vector<glm::vec4>& vertices, int dimension) {
    float maxCoord = std::numeric_limits<float>::lowest();
    float minCoord = std::numeric_limits<float>::max();

    for (const auto& vertex : vertices) {
        if (vertex[dimension] > maxCoord) {
            maxCoord = vertex[dimension];
        }
        if (vertex[dimension] < minCoord) {
            minCoord = vertex[dimension];
        }
    }

    return maxCoord - minCoord;
}


/**
 * @brief Calcula la translacion necesaria para llevar una figura al origen
 * @param vertices vector de vertices de la figura
 * @return
 */
glm::vec4 GeometryRender::getOrigin(const std::vector<glm::vec4>& vertices){
    // Calcular el centro de masa de la figura
    glm::vec4 center (0.0f, 0.0f, 0.0f, 0.0f);
    for (const auto& vertex : vertices) {
        center[0] += vertex.x;
        center[1] += vertex.y;
        center[2] += vertex.z;
    }
    for(size_t i = 0; i < 4; i++){
        center[i] /= static_cast<float>(vertices.size());
    }
    return center;
}

/**
 * @brief configures the gui usage
 */
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
                loadTexture(textureFilePath+"/"+textureFileName);
            } else {
                std::cerr << "Texture file can't be opened" << std::endl;
            }
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

    if(ImGui::CollapsingHeader("Shading")){
        const char* items[] = {"Phong", "Gouraud", "Wireframe" };
        static int proj_current_idx = 0;
        if (ImGui::Combo("Shading model", &proj_current_idx, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)));
        if(proj_current_idx == 0){
            illuminationModel = 0;
            std::cout << illuminationModel << " Phong model activated" << std::endl;
        }
        if(proj_current_idx == 1){
            illuminationModel = 1;
            std::cout << illuminationModel << " Gouraud model activated" << std::endl;
        }
        if(proj_current_idx == 2){
            illuminationModel = 2;
            std::cout << illuminationModel << " Wireframe model activated" << std::endl;
        }
    }

ImGui::End();
}

/**
 * @brief manages the cursor callback
 * @param window
 * @param xpos x position of the mouse
 * @param ypos y position of the mouse
 */
void GeometryRender::cursorPositionCallBack(GLFWwindow *window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window,xpos,ypos);
    if(mouse) {
        computeCameraMouse(xpos,ypos);
        posX = xpos;
        posY = ypos;
    }
}


/**
 * @brief Computes the rotation movement of the camera with the mouse
 * @param x current x position of the mouse
 * @param y current y position of the mouse
 */
void GeometryRender::computeCameraMouse(float x, float y) {

    double deltaX = x - posX;
    double deltaY = y - posY;
    yaw += deltaX * cameraSpeed;
    pitch += deltaY * cameraSpeed;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    lookAt = cameraPosition + glm::normalize(direction);

    matView = glm::lookAt(cameraPosition, lookAt, upVector);
}

/**
 * @brief load the texture
 * @param filePath path to the file
 * @return GLuint with the id of the texture
 */
GLuint GeometryRender::loadTexture(const std::string& filePath){
    int width, height, numChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum colorNotation;
    if(numChannels == 3)
        colorNotation = GL_RGB;
    else colorNotation = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, colorNotation, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture parameters (optional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}

/**
 * Binds the texture to be used
 */
void GeometryRender::activateTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, locTexture);
}

/**
 * Unbind the texture
 */
void GeometryRender::deactivateTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}