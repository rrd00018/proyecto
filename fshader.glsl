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
uniform int textureShow;


void main()
{

    float ambientStrength = 0.1;
    vec3 ambient = materialAmbient * ambientColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialDiffuse;

    // specular
    vec3 specular = vec3(0,0,0);
    if(diff > 0){
        float specularStrength = 0.5;
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        specular = specularStrength * spec * materialSpecular * lightColor;
    }
    vec3 textureColor =  vec3(texture(myTexture,TextCoords));
    vec3 result = ambient + diffuse + specular;

    if(textureShow == 1){
        FragColor = vec4(result * textureColor, 1);
    }else{
        FragColor = vec4(result, 1);
    }
}
