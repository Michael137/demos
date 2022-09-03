#version 120

// 'varying' => can be shared between shaders
varying vec2 texCoord0;

uniform sampler2D diffuse;

void main() {
    gl_FragColor = texture2D(diffuse, texCoord0);
}

