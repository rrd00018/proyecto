#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

//PASANDO EL LIGHT DIR Y VIEW DIR AL VERTEX SHADER

out vec4 FragColor;

uniform sampler2D myTexture;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform vec3 cameraPosition;
uniform vec3 lightPos;
uniform float materialShininess;
uniform int illuminationModel;


void main()
{
    // Diferentes modelos de iluminación
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 textureColor =  vec3(texture(myTexture,TextCoords));
    FragColor = vec4(diffuse * textureColor,1);
}
