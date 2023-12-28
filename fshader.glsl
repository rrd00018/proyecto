#version 330 core

in vec3 Normal;
in vec3 FragPos; // Posición del fragmento en eye space

out vec4 FragColor;

const vec3 modelColor = vec3(1.0,1.0,1.0);
const float ambientStrength = 0.1;

uniform vec3 cameraPosition;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

void main() {
    // Calcula el vector de luz y la dirección de vista
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(cameraPosition - FragPos);

    // Calcula la componente difusa
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialDiffuse;

    // Calcula la componente especular
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = materialSpecular * spec * lightColor;

    // Calcula la componente ambiental
    vec3 ambient = ambientColor * materialAmbient;

    // Combina todas las componentes para obtener el color final
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
