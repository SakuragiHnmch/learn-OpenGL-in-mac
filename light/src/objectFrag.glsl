#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * lightColor;
//
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor;
//
//    float specularStrength = 0.5;
//    vec3 viewDir = normalize(viewPos- FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    vec3 specular = specularStrength * spec * lightColor;
//
//    vec3 result = (ambient + diffuse + specular) * objectColor;

    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = lightColor * (diff * material.diffuse);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(norm, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    //the objectColor is include in the Material struct, for example, previous ambient * objectColor =  material.ambient
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}