#version 330 core
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;

out vec2 v_pos;

uniform vec4 u_bounds;

void main()
{
    gl_Position = vec4(a_pos, 0.0, 1.0);
    v_pos = mix(u_bounds.xy, u_bounds.zw, a_uv);
}
