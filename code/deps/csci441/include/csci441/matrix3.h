#ifndef _CSCI441_MATRIX3_H_
#define _CSCI441_MATRIX3_H_

#include <sstream>

#include <csci441/matrix4.h>

class Matrix3 {
private:
    unsigned int idx(unsigned int r, unsigned int c) const {
        return r + c*3;
    }

public:
    float values[9];

    Matrix3() {
        set_to_identity();
    };

    std::string to_string()  const {
        std::ostringstream os;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                os << values[idx(row, col)] << " ";
            }
            os << std::endl;
        }
        return os.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix3& m) {
        os << m.to_string();
        return os;
    }

    void set_to_identity() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                values[idx(i,j)] = i==j;
            }
        }
    }

    void inverse_transpose(const Matrix4& m) {
        set_to_identity();

        values[idx(0,0)] = m(1,1)*m(2,2) - m(1,2)*m(2,1);
        values[idx(0,1)] = m(1,2)*m(2,0) - m(1,0)*m(2,2);
        values[idx(0,2)] = m(1,0)*m(2,1) - m(1,1)*m(2,0);

        values[idx(1,0)] = m(0,2)*m(2,1) - m(0,1)*m(2,2);
        values[idx(1,1)] = m(0,0)*m(2,2) - m(0,2)*m(2,0);
        values[idx(1,2)] = m(0,1)*m(2,0) - m(0,0)*m(2,1);

        values[idx(2,0)] = m(0,1)*m(1,2) - m(0,2)*m(1,1);
        values[idx(2,1)] = m(0,2)*m(1,0) - m(0,0)*m(1,2);
        values[idx(2,2)] = m(0,0)*m(1,1) - m(0,1)*m(1,0);
    }
};

#endif

