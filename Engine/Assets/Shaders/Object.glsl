// VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in vec3 aTangent;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;
out vec3 vFragPos;
out vec3 vTangent;
out vec3 vBitangent;
out vec3 vNormal;

void main()
{
    vUVs = aUVs;
    vFragPos = vec3(uTransform * vec4(aPos, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(uTransform)));
    vNormal = normalize(normalMatrix * aNormals);
    vTangent = normalize(normalMatrix * aTangent);
    vBitangent = normalize(cross(vNormal, vTangent)); // Or pass from mesh

    gl_Position = uProjection * uView * vec4(vFragPos, 1.0);
}


// FRAGMENT
#version 330 core

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

#define CE_LIGHT_COUNT 2

uniform DirectionalLight DirLight;
uniform PointLight uPointLights[CE_LIGHT_COUNT];
uniform int uPointLightCount;
uniform SpotLight uSpotLights[CE_LIGHT_COUNT];
uniform int uSpotLightCount;

in vec2 vUVs;
in vec3 vFragPos;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vNormal;

uniform vec4 uColor;
uniform float uMetallic;
uniform float uRoughness;
uniform float uAO;

uniform vec3 uCameraPosition;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;

out vec4 FragColor;

const float PI = 3.14159265359;

// ----------------------- PBR Functions -----------------------
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// --------------------------------------------------------------

void main()
{
    // Normal Mapping
    vec3 sampledNormal = texture(uNormalTexture, vUVs).rgb * 2.0 - 1.0;
    mat3 TBN = mat3(normalize(vTangent), normalize(vBitangent), normalize(vNormal));
    vec3 N = normalize(TBN * sampledNormal);
    vec3 V = normalize(uCameraPosition - vFragPos);

    vec3 albedo = pow(texture(uColorTexture, vUVs).rgb * uColor.rgb, vec3(2.0));
    vec3 F0 = mix(vec3(0.04), albedo, uMetallic);

    vec3 Lo = vec3(0.0);

    // Point Lights
    for(int i = 0; i < uPointLightCount; i++) {
        vec3 L = normalize(uPointLights[i].Position - vFragPos);
        vec3 H = normalize(V + L);
        float distance = length(uPointLights[i].Position - vFragPos);
        float attenuation = 1.0 / (uPointLights[i].Constant + uPointLights[i].Linear * distance + uPointLights[i].Quadratic * (distance * distance));
        vec3 radiance = uPointLights[i].Color.rgb * attenuation * uPointLights[i].Intensity;

        float NDF = DistributionGGX(N, H, uRoughness);
        float G = GeometrySmith(N, V, L, uRoughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - uMetallic);

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // Spot Lights
    for (int i = 0; i < uSpotLightCount; ++i) {
        vec3 L = normalize(uSpotLights[i].Position - vFragPos);
        vec3 H = normalize(V + L);
        float distance = length(uSpotLights[i].Position - vFragPos);
        float attenuation = 1.0 / (uSpotLights[i].Constant + uSpotLights[i].Linear * distance + uSpotLights[i].Quadratic * (distance * distance));

        float theta = dot(L, normalize(-uSpotLights[i].Direction));
        float epsilon = uSpotLights[i].CutOff - uSpotLights[i].OuterCutOff;
        float intensity = clamp((theta - uSpotLights[i].OuterCutOff) / epsilon, 0.0, 1.0);

        vec3 radiance = uSpotLights[i].Color.rgb * attenuation * intensity;

        float NDF = DistributionGGX(N, H, uRoughness);
        float G = GeometrySmith(N, V, L, uRoughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - uMetallic);

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // Directional Light
    {
        vec3 L = normalize(-DirLight.Direction);
        vec3 H = normalize(V + L);
        vec3 radiance = DirLight.Color.rgb * DirLight.Intensity;

        float NDF = DistributionGGX(N, H, uRoughness);
        float G = GeometrySmith(N, V, L, uRoughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - uMetallic);

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // Ambient Lighting
    vec3 ambient = vec3(0.03) * albedo * uAO;

    vec3 color = ambient + Lo;

    // HDR tonemapping and gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

