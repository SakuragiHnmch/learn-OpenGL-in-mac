#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir) {
    // 手动执行透视除法（在进行透视投影时有效）
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 标准化设备空间坐标范围是[-1, 1], 我们需要的是用于采样的纹理坐标，将NDC坐标范围缩小一半，再向x轴正向平移0.5个单位，可得到范围[0, 1]的坐标
    projCoords = 0.5 * projCoords + 0.5;
    // 在当前片段，获取光照视角下的深度值，也即最深可以被光照照射到的深度值，凡是超过这个深度值，说明不能被光照照射到，处于阴影之中
    float closestDepth = texture(shadowMap, projCoords.xy).r; // opengl会自动将深度值转化为灰度值？
    // 当前片段，摄像机视角的深度值
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fs_in.Normal);
    vec3 light = normalize(lightDir);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float isInShadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return isInShadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfVec = normalize(lightDir + reflectDir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 32);
    vec3 specular = spec * lightColor;

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir);

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}