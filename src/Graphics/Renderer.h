#pragma once

#include "Window.h"
#include "Util/Math.h"

namespace engine {
    class Renderer {
    public:
        Renderer(Window* window);

        void initialize();
        void clear();
        void drawTriangleMesh(unsigned int program, unsigned int vao, unsigned int indexCount, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projectionMatrix);
        void present();

    private:
        Window* window;
    };
}