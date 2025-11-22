#version 330 core

in vec2 v_pos;
in vec2 v_uv;

out vec4 frag_color;

uniform vec3 u_color;

float modulus(vec2 num) { return sqrt(num.x * num.x + num.y * num.y); }
vec2 square(vec2 num) { return vec2(num.x * num.x - num.y * num.y, 2 * num.x * num.y); }

void main()
{
    vec3 bckgrnd = vec3(0.0, 0.0, 0.0);
    vec3 result  = u_color;
    vec2 num     = v_pos.xy;
    // mandelbrot computations
    // quick check to decrease computation time
    if (modulus(num) < 0.2) { result = bckgrnd; }
    else if (num.x < 0 && modulus(num) < 0.6)
    {
        result = bckgrnd;
    }
    else
    {
        // TODO (stouff) add antialiasing
        vec2 z = vec2(0.0, 0.0);                                            // start the 0-orbit
        int cap = 1000;                                                     // set an iteration cap
        int i;
        for (i = 0; i < cap && modulus(z) <= 2.0; i++)                      // iterate 0 on z_n+1 = z_n^2 + num
        {
            z = square(z) + num;
        }
        if (modulus(z) <= 2) { result = bckgrnd; }                          // if orbit has not diverged to infinity, return the background color
        else                                                                // otherwise, compute the scaled color
        {
            float div = cap/i;
            float r = u_color.x + (1.0 / div) * (1 - u_color.x);
            float g = u_color.y + (1.0 / div) * (1 - u_color.y);
            float b = u_color.z + (1.0 / div) * (1 - u_color.z);
            result = vec3(r, g, b);
        }
    }

    frag_color = vec4(result, 1.0f);
    frag_color = vec4(v_uv, 0.f, 1.0f);
}
