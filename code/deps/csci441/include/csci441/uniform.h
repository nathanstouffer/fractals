#ifndef _CSCI441_UNIFORM_H_
#define _CSCI441_UNIFORM_H_

#include <csci441/matrix4.h>
#include <csci441/matrix3.h>
#include <csci441/vector4.h>

class Uniform {
public:
    static void set(GLuint shader, const std::string &name, const Matrix4& m) {
        GLuint loc = location(shader, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m.values);
    }

    static void set(GLuint shader, const std::string &name, const Matrix3& m) {
        GLuint loc = location(shader, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, m.values);
    }

    static void set(GLuint shader, const std::string &name, const Vector4& v) {
        GLuint loc = location(shader, name);
        glUniform3f(loc, v.x(), v.y(), v.z());
    }

    static void set(GLuint shader, const std::string &name, const int v) {
        GLuint loc = location(shader, name);
        glUniform1i(loc, v);
    }

    static void set(GLuint shader, const std::string &name, const float v) {
        GLuint loc = location(shader, name);
        glUniform1f(loc, v);
    }

private:

    static GLuint location(GLuint shader, const std::string &name) {
        return glGetUniformLocation(shader, name.c_str());
    }
};

#endif


