// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vUVs;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    vUVs = aPosition;
}

// FRAGMENT
#version 330 core

in vec3 vUVs;
out vec4 FragColor;

uniform samplerCube uSkybox;

void main() {
    FragColor = texture(uSkybox, normalize(vUVs));
}