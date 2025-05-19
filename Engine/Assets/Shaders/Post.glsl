// VERTEX
#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUVs;

out vec2 vUVs;

void main() {
    vUVs = aUVs;
    gl_Position = vec4(aPosition, 0.0, 1.0);
}


// FRAGMENT
#version 330 core

in vec2 vUVs;
out vec4 oColor;

uniform sampler2D uScreenTexture;

void main() {
    // Vignette
    float dist = distance(vUVs, vec2(0.5));
    float vignette = smoothstep(0.75, 0.3, dist);

    // Chromatic aberration
    float offset = 0.003;
    vec3 col;
    col.r = texture(uScreenTexture, vUVs + vec2(offset, 0.0)).r;
    col.g = texture(uScreenTexture, vUVs).g;
    col.b = texture(uScreenTexture, vUVs - vec2(offset, 0.0)).b;

    // Subtle scanline effect
    float scanline = 0.96 + 0.04 * sin(vUVs.y * 800.0);
    col *= scanline;

    // Apply vignette
    col *= vignette;

    oColor = vec4(col, 1.0);
}
