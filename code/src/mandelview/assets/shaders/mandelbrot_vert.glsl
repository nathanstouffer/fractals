#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 myColor;
out vec2 myFragPos;

uniform mat4 view;

void main() {
    gl_Position = view * vec4(aPos, 0.0, 1.0);
    myColor = aColor;
    myFragPos = vec2(aPos);
}
