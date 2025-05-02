// VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;

void main()
{
  vUVs = aUVs;
    gl_Position = uProjection * uView * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

// FRAGMENT
#version 330 core

in vec2 vUVs;

uniform vec4 uColor;
uniform sampler2D uColorTexture;

out vec4 FragColor;

void main()
{
    FragColor = uColor * texture2D(uColorTexture, vUVs);
}

