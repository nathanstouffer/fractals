#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>
#include <csci441/matrix4.h>    // TODO (stouff) remove this include

#include <stf/stf.hpp>

namespace mandelview
{

    static constexpr double c_zoom_delta = 0.025;

    static stfi::vec2 s_dimensions = stfi::vec2(960, 540);

    void resize_callback(GLFWwindow* window, int width, int height)
    {
        s_dimensions = stfi::vec2(width, height);
        glViewport(0, 0, width, height);
    }

    bool is_pressed(GLFWwindow* window, int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    void process_input(GLFWwindow* window)
    {
        if (is_pressed(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    stfd::vec2 process_translation(GLFWwindow* window, stfd::vec2 const& magnitude)
    {
        stfd::vec2 delta = stfd::vec2();

        if (is_pressed(window, GLFW_KEY_W)) { delta += stfd::vec2(0, magnitude.y); }
        if (is_pressed(window, GLFW_KEY_A)) { delta -= stfd::vec2(magnitude.x, 0); }
        if (is_pressed(window, GLFW_KEY_S)) { delta -= stfd::vec2(0, magnitude.y); }
        if (is_pressed(window, GLFW_KEY_D)) { delta += stfd::vec2(magnitude.x, 0); }

        return delta;
    }

    double process_zoom(GLFWwindow* window)
    {
        double update = 1.0;
        const float delta = c_zoom_delta;

        if (is_pressed(window, '-')) { update += delta; }
        if (is_pressed(window, '=')) { update -= delta; }

        return update;
    }

    int main()
    {
        /* Initialize the library */
        GLFWwindow* window;
        if (!glfwInit())
        {
            return -1;
        }

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(s_dimensions.x, s_dimensions.y, "mandelview", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // tell glfw what to do on resize
        glfwSetFramebufferSizeCallback(window, resize_callback);

        // init glad
        if (!gladLoadGL())
        {
            std::cout << "Failed to initialize OpenGL context" << std::endl;
            glfwTerminate();
            return -1;
        }

        /* init the triangle drawing */
        // define the vertex coordinates of the triangle
        float triangles[] =
        {
            // points       // uv
            1.0f,  1.0f,    1.f, 1.f,
            1.0f, -1.0f,    1.f, 0.f,
            -1.0f,  1.0f,   0.f, 1.f,

            1.0f, -1.0f,    1.f, 0.f,
            -1.0f, -1.0f,   0.f, 0.f,
            -1.0f,  1.0f,   0.f, 1.f,
        };

        int r = 255;
        int g = 0;
        int b = 0;

        // create and bind the vertex buffer object and copy the data to the buffer
        GLuint VBO[1];
        glGenBuffers(1, VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

        // create and bind the vertex array object and describe data layout
        GLuint VAO[1];
        glGenVertexArrays(1, VAO);
        glBindVertexArray(VAO[0]);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0*sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 4*sizeof(float), (void*)(2*sizeof(float)));
        glEnableVertexAttribArray(1);

        stfd::aabb2 bounds = { stfd::vec2(-4, -1.5), stfd::vec2(1.33, 1.5) };

        // create the shaders
        Shader shader("assets/shaders/mandelbrot_vert.glsl", "assets/shaders/mandelbrot_frag.glsl");

        stfi::vec2 dimensions = s_dimensions;
        bool prev_enter = false;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // process input
            process_input(window);

            // process resize change
            if (dimensions != s_dimensions)
            {
                stfd::vec2 center = bounds.center();
                stfd::vec2 scale = s_dimensions.as<double>() / dimensions.as<double>();
                stfd::vec2 half_diag = 0.5 * scale * bounds.diagonal();
                bounds = stfd::aabb2(center - half_diag, center + half_diag);
                dimensions = s_dimensions;
            }

            // compute zoom in/out
            {
                double zoom = process_zoom(window);
                if (zoom != stfd::constants::one)
                {
                    stfd::vec2 center = bounds.center();
                    bounds.translate(-center).scale(zoom).translate(center);
                }
            }

            // compute translation
            {
                stfd::vec2 diagonal = bounds.diagonal();
                stfd::vec2 delta = process_translation(window, diagonal / s_dimensions.as<double>());
                bounds.translate(delta);
            }

            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // use the shader
            shader.use();

            // test if enter has been clicked
            if (is_pressed(window, GLFW_KEY_ENTER) && prev_enter == false)
            {
                std::cout << "\n\n----------------------------- info -----------------------------" << std::endl;
                std::cout << "--bounds " << bounds.min.x << " " << bounds.min.y << " " << bounds.max.x << " " << bounds.max.y << std::endl;
                std::cout << "center: " << bounds.center().x << " " << bounds.center().y << "  ";
                std::cout << "width: " << bounds.diagonal().x << "  height: " << bounds.diagonal().y << std::endl;
            }
            prev_enter = is_pressed(window, GLFW_KEY_ENTER);

            stff::vec4 bounds_vec = stfd::vec4(bounds.min, bounds.max).as<float>();
            unsigned int bounds_loc = glGetUniformLocation(shader.id(), "u_bounds");
            glUniform4fv(bounds_loc, 1, bounds_vec.values);

            stff::vec3 color = stff::vec3(r, g, b) / 255.f;
            unsigned int color_loc = glGetUniformLocation(shader.id(), "u_color");
            glUniform3fv(color_loc, 1, color.values);

            // draw our triangles
            glBindVertexArray(VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(triangles));

            /* Swap front and back * buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

}

int main()
{
    return mandelview::main();
}