#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;
in vec3 cameraPositionWorld;
in vec3 lightPositionWorld;
in vec3 viewDir;
in vec3 lightDir;
//PASANDO EL LIGHT DIR Y VIEW DIR AL VERTEX SHADER

out vec4 FragColor;

uniform sampler2D myTexture;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform int illuminationModel;

void main()
{
   /* // Diferentes modelos de iluminación
    vec3 ambient = ambientColor * materialAmbient;
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * materialDiffuse);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightColor * (spec * materialSpecular);

    vec3 result = ambient + diffuse + specular;

    if (illuminationModel == 0) { // Phong
        FragColor = vec4(result, 1.0);
    } else if (illuminationModel == 1) { // Otro modelo de iluminación
        // Implementar otro modelo de iluminación si es necesario
    } else if(illuminationModel == 2){
        FragColor = vec4(1,0,0,0);
    }

    // Puedes agregar mapeo de texturas si es necesario
    vec3 textureColor = texture(myTexture, TextCoords).xyz;
    FragColor = vec4(result,1.0);
*/

    vec3 n = normalize(Normal);
    vec3 l = normalize(lightDir);
    vec3 v = normalize(viewDir);
    vec3 r = reflect(-l,n);

    float cosTheta = dot( n,l );
    float cosAlpha = clamp( dot( v,r ), 0,1 );

    vec3 ambient = materialAmbient * ambientColor;
    vec3 diffuse = materialDiffuse * lightColor * cosTheta;
    vec3 specular = materialSpecular * lightColor * cosAlpha;


    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result,1.0);
}
