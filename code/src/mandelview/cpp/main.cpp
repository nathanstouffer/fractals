#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>
#include <csci441/matrix4.h>

#define SCALE 0.025

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

Matrix4 processTrans(GLFWwindow *window, float scale)
{
    Matrix4 update, tmp;
    const float TRANS = 0.01/scale;

    // TRANSLATE
    if (isPressed(window, GLFW_KEY_W)) { tmp.translate(0, -TRANS, 0); update = update * tmp; }
    if (isPressed(window, GLFW_KEY_A)) { tmp.translate(TRANS, 0, 0);  update = update * tmp; }
    if (isPressed(window, GLFW_KEY_S)) { tmp.translate(0, TRANS, 0);  update = update * tmp; }
    if (isPressed(window, GLFW_KEY_D)) { tmp.translate(-TRANS, 0, 0); update = update * tmp; }

    return update;
}

Matrix4 processZoom(GLFWwindow *window)
{
    Matrix4 update, tmp;
    const float scale = SCALE;

    // SCALE
    if (isPressed(window, '-')) { tmp.scale(1-scale, 1-scale, 1); update = update * tmp; }
    if (isPressed(window, '=')) { tmp.scale(1+scale, 1+scale, 1); update = update * tmp; }

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
    window = glfwCreateWindow(1920, 1080, "fractal", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // init glad
    if (!gladLoadGL())
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* init the triangle drawing */
    // define the vertex coordinates of the triangle
    float triangle[] =
    {
         // points    // color
         10.0f,  10.0f, 0.05, 0.05, 0.05,
         10.0f, -10.0f, 0.05, 0.05, 0.05,
        -10.0f,  10.0f, 0.05, 0.05, 0.05,

         10.0f, -10.0f, 0.05, 0.05, 0.05,
        -10.0f, -10.0f, 0.05, 0.05, 0.05,
        -10.0f,  10.0f, 0.05, 0.05, 0.05
    };

    // create and bind the vertex buffer object and copy the data to the buffer
    GLuint VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // create and bind the vertex array object and describe data layout
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the shaders
    Shader shader("assets/shaders/mandelbrot_vert.glsl", "assets/shaders/mandelbrot_frag.glsl");

    Matrix4 view, undistort, trans, zoom;
    undistort.scale(1/1.7777777777777777, 1, 1);
    trans.translate(0.5, 0, 0);

    bool prev_enter = false;
    float scale = 1;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // process input
        processInput(window);
        zoom = zoom * processZoom(window);
        trans = trans * processTrans(window, zoom.values[0]);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader
        shader.use();

        view = zoom * trans * undistort;

        // test if enter has been clicked
        if (isPressed(window, GLFW_KEY_ENTER) && prev_enter == false)
        {
            float x = -trans.values[12]*1.77777777777777;
            float y = -trans.values[13];
            float width = (3.5)/zoom.values[0];
            std::cout << "\n\n----- info -----" << std::endl;
            std::cout << "center ------- " << "x: " << x << " y: " << y << std::endl;
            std::cout << "zoom  -------- " << zoom.values[0] << std::endl;
            std::cout << "width -------- " << width << std::endl;
        }
        prev_enter = isPressed(window, GLFW_KEY_ENTER);

        // set shader variables
        unsigned int viewLoc = glGetUniformLocation(shader.id(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.values);

        // draw our triangles
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle));

        /* Swap front and back * buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
