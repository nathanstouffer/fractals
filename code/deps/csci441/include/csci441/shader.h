#ifndef _CSCI441_SHADER_H_
#define _CSCI441_SHADER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class Shader {
    // adapted from shader loading code presented in
    // https://learnopengl.com/Getting-started/Hello-Triangle
private:
    GLuint _id;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        // create the shaders
        GLuint vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
        GLuint fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);

        // create the program
        _id = createShaderProgram(vertexShader, fragmentShader);

        // cleanup the shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint id() const {
        return _id;
    }

    void use() {
        glUseProgram(id());
    }

protected:

    GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM:COMPILATION_FAILED\n"
                << infoLog << std::endl;
        }

        return program;
    }

    std::string shaderTypeName(GLenum shaderType) {
        switch(shaderType) {
            case GL_VERTEX_SHADER: return "VERTEX";
            case GL_FRAGMENT_SHADER: return "FRAGMENT";
            default: return "UNKNOWN";
        }
    }

    GLuint createShader(const std::string& fileName, GLenum shaderType) {
        std::ifstream stream(fileName);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();

        std::string source = buffer.str();
        std::cout << "Source:" << std::endl;
        std::cout << source << std::endl;

        GLuint shader = glCreateShader(shaderType);
        const char* src_ptr = source.c_str();
        glShaderSource(shader, 1, &src_ptr, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::" << shaderTypeName(shaderType)
                <<"::COMPILATION_FAILED\n"
                << infoLog << std::endl;
        }
        return shader;
    }
};

#endif
