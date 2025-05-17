// VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormals;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;
out vec3 vNormals;
out vec3 vFragPos;

void main()
{
    vUVs = aUVs;
    vNormals = mat3(transpose(inverse(uTransform))) * aNormals;
    vFragPos = vec3(uTransform * vec4(aPos, 1.0));
    gl_Position = uProjection * uView * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

// FRAGMENT
#version 330 core

// -- Light structures --
struct DirectionalLight {
    vec4 Color;

    vec3 Direction;
    
    float Intensity;
};

struct PointLight {
    vec3 Position;
    vec3 Specular;

    vec4 Color;

    float Constant;
    float Linear;
    float Quadratic;
    float Radius;
    float Intensity;
};

struct SpotLight {
  vec3 Position;
  vec3 Direction;
  vec3 Diffuse;
  vec3 Specular;

  vec4 Color;

  float CutOff;
  float OuterCutOff;
  float Constant;
  float Quadratic;
  float Linear;
};

#define CE_LIGHT_COUNT 100

uniform DirectionalLight DirLight;
uniform PointLight uPointLights[CE_LIGHT_COUNT];
uniform int uPointLightCount;
uniform SpotLight uSpotLights[CE_LIGHT_COUNT];
uniform int uSpotLightCount;
// ----------------------

in vec2 vUVs;
in vec3 vNormals;
in vec3 vFragPos;

uniform vec4 uColor;
uniform vec3 uCameraPosition;
uniform sampler2D uColorTexture;

out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 diffuse = diff * light.Color.rgb * light.Intensity;
    return diffuse;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // ambient
    vec3 ambient = light.Color.rgb;
    
    // diffuse 
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff;  
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0); // todo: shininess
    vec3 specular = light.Specular * spec;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    attenuation *= light.Intensity;

    vec3 diffuse = light.Color.rgb * diff * light.Intensity;
    vec3 specular = light.Specular * spec;

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

void main()
{
    vec3 normal = normalize(vNormals);
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);

    vec3 dirResult = CalcDirLight(DirLight, normal, viewDirection);

    vec3 pointLightResult = vec3(0);
    for(int i = 0; i < uPointLightCount; i++) {
        pointLightResult += CalculatePointLight(uPointLights[i], normal, vFragPos, viewDirection);
    }

    vec3 spotLightResult = vec3(0);
    for (int i = 0; i < uSpotLightCount; i++) {
        spotLightResult += CalculateSpotLight(uSpotLights[i], normal, vFragPos, viewDirection);
    }
    FragColor = uColor * texture2D(uColorTexture, vUVs);
    FragColor += vec4(dirResult, 1.0);
    FragColor += vec4(pointLightResult, 1.0);
    FragColor += vec4(spotLightResult, 1.0);
}

