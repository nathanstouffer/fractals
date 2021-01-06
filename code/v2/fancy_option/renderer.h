#ifndef _CSCI441_RENDERER_H_
#define _CSCI441_RENDERER_H_

#include <csci441/matrix3.h>

class Renderer {
    Matrix3 itModel_tmp;
public:

    void render(const Camera& camera, Model& m) {
        itModel_tmp.inverse_transpose(m.model);

        m.shader.use();
        Uniform::set(m.shader.id(), "model", m.model);
        Uniform::set(m.shader.id(), "projection", camera.projection);
        Uniform::set(m.shader.id(), "camera", camera.look_at());
        Uniform::set(m.shader.id(), "eye", camera.eye);
        Uniform::set(m.shader.id(), "itModel", itModel_tmp);

        // render the cube
        glBindVertexArray(m.vao);
        glDrawArrays(GL_TRIANGLES, 0, m.size);
    }
};

#endif
