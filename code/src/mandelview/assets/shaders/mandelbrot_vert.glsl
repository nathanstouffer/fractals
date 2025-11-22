#version 330 core
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec3 a_color;

out vec3 v_color;
out vec2 v_frag_pos;

uniform mat4 view;

void main()
{
    gl_Position = view * vec4(a_pos, 0.0, 1.0);
    v_color = a_color;
    v_frag_pos = vec2(a_pos);
}
