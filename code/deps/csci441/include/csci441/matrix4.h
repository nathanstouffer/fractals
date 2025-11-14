#ifndef _CSCI441_MATRIX4_H_
#define _CSCI441_MATRIX4_H_

#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/glm.hpp>

#include "vector4.h"

class Matrix4 {
private:
    unsigned int idx(unsigned int r, unsigned int c) const {
        return r + c*4;
    }

public:
    float values[16];

    Matrix4() {
        set_to_identity();
    };

    Matrix4(const glm::mat4& m) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                values[idx(i,j)] = m[j][i];
            }
        }
    };

    float operator()(int row, int col) const {
        return values[idx(row, col)];
    }

    void set(int row, int col, float value) {
        values[idx(row, col)] = value;
    }

    void scale(float x, float y, float z) {
        set_to_identity();
        values[idx(0,0)] = x;
        values[idx(1,1)] = y;
        values[idx(2,2)] = z;
    }

    void rotate_x(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(1,1)] = std::cos(theta_radians);
        values[idx(1,2)] = -std::sin(theta_radians);
        values[idx(2,1)] = std::sin(theta_radians);
        values[idx(2,2)] = std::cos(theta_radians);
    }

    void rotate_y(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(0,0)] = std::cos(theta_radians);
        values[idx(0,2)] = std::sin(theta_radians);
        values[idx(2,0)] = -std::sin(theta_radians);
        values[idx(2,2)] = std::cos(theta_radians);
    }

    void rotate_z(float theta) {
        double theta_radians = theta*M_PI/180.0;

        set_to_identity();
        values[idx(0,0)] = std::cos(theta_radians);
        values[idx(0,1)] = -std::sin(theta_radians);
        values[idx(1,0)] = std::sin(theta_radians);
        values[idx(1,1)] = std::cos(theta_radians);
    }

    void translate(float x, float y, float z) {
        set_to_identity();
        values[idx(0,3)] = x;
        values[idx(1,3)] = y;
        values[idx(2,3)] = z;
    }

    void look_at(const Vector4& eye, const Vector4& target, const Vector4& up) {
        Vector4 gaze = target-eye;
        Vector4 w = gaze.normalized().scale(-1);
        Vector4 u = up.cross(w).normalized();
        Vector4 v = w.cross(u);

        Matrix4 camera;
        Vector4* basis[3] = {&u, &v, &w};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                camera.values[idx(i,j)] = basis[i]->coord(j);
            }
        }

        Matrix4 translate;
        translate.translate(-eye.x(), -eye.y(), -eye.z());

        set_to_identity();
        mult(values, camera.values, translate.values);
    }

    void ortho(float l, float r, float b, float t, float n, float f) {
        set_to_identity();
        values[idx(0,0)] = 2 / (r-l);
        values[idx(0,3)] = -(r+l) / (r-l);

        values[idx(1,1)] = 2 / (t-b);
        values[idx(1,3)] = -(t+b) / (t-b);

        values[idx(2,2)] = -2 / (f-n);
        values[idx(2,3)] = -(f+n) / (f-n);
    }

    void perspective(float min_fov, float aspect_ratio, float n, float f) {
        set_to_identity();

        double theta = min_fov*M_PI/180.0;

        values[idx(0,0)] = 1 / (aspect_ratio*tan(theta));
        values[idx(1,1)] = 1 / (tan(theta));
        values[idx(2,2)] = -(f+n) / (f-n);
        values[idx(2,3)] = (-2*f*n) / (f-n);
        values[idx(3,2)] = -1;
        values[idx(3,3)] = 0;
    }

    void perspective(float l, float r, float b, float t, float n, float f) {
        set_to_identity();
        values[idx(0,0)] = 2*n / (r-l);
        values[idx(0,2)] = (r+l) / (r-l);

        values[idx(1,1)] = 2*n / (t-b);
        values[idx(1,2)] = (t+b) / (t-b);

        values[idx(2,2)] = -(f+n) / (f-n);
        values[idx(2,3)] = (-2*f*n) / (f-n);

        values[idx(3,2)] = -1;
        values[idx(3,3)] = 0;
    }

    Matrix4 operator*(const Matrix4& m) const {
        Matrix4 ret;
        mult(ret.values, values, m.values);
        return ret;
    }

    Vector4 operator*(const Vector4& v) const {
        Vector4 ret;
        for (int i = 0; i < 4; ++i) {
            ret.values[i] = dot(values, i, v.values, 0);
        }
        return ret;
    }

    std::string to_string()  const {
        std::ostringstream os;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                os << values[idx(row, col)] << " ";
            }
            os << std::endl;
        }
        return os.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix4& m) {
        os << m.to_string();
        return os;
    }

private:

    double dot(const float* m1, unsigned int row, const float* m2, int col) const {
        double dot = 0;
        for (int i = 0; i < 4; ++i) {
            dot += m1[idx(row,i)]*m2[idx(i,col)];
        }
        return dot;
    }

    void mult(float* target, const float* const m1, const float* const m2) const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                target[idx(i,j)] = dot(m1, i, m2, j);
            }
        }
    }

    void set_to_identity() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                values[idx(i,j)] = i==j;
            }
        }
    }
};

#endif
