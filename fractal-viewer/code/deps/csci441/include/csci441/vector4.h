#ifndef _CSCI441_VECTOR4_H_
#define _CSCI441_VECTOR4_H_

#include <sstream>

class Vector4 {
    const static int X=0;
    const static int Y=1;
    const static int Z=2;
    const static int W=3;
    const static int N_COORDS = 4;

    float det(float a, float b, float c, float d) const {
        return  a*d - b*c;
    }

public:
    float values[N_COORDS];

    Vector4(float x=0, float y=0, float z=0, float w=1) : values{x, y, z, w} { }

    Vector4 operator-(const Vector4& v) const {
        return Vector4(
                values[X] - v.values[X],
                values[Y] - v.values[Y],
                values[Z] - v.values[Z],
                values[W] - v.values[W]);
    }

    float x() const { return values[X]; }
    float y() const { return values[Y]; }
    float z() const { return values[Z]; }
    float w() const { return values[W]; }
    float coord(int idx) { return values[idx % N_COORDS]; }

    Vector4 scale(double s) const {
        return Vector4(
                s*values[X],
                s*values[Y],
                s*values[Z]);
    }

    Vector4 normalized() const {
        double inv_norm = 1/norm();
        return scale(inv_norm);
    }

    Vector4 cross(const Vector4& v) const {
        return Vector4(
                 det(values[Y], values[Z], v.values[Y], v.values[Z]),
                -det(values[X], values[Z], v.values[X], v.values[Z]),
                 det(values[X], values[Y], v.values[X], v.values[Y]),
                 0);
    }

    double norm() const {
        double n_sq = 0;
        for (int i = 0; i < N_COORDS; ++i) {
            n_sq += values[i]*values[i];
        }
        return sqrt(n_sq);
    }

    std::string to_string()  const {
        std::ostringstream os;
        for (int i = 0; i < N_COORDS; ++i) {
            os << values[i] << " ";
        }
        return os.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v) {
        os << v.to_string();
        return os;
    }
};

#endif
