#include "window.hpp"
#include "shader.hpp"

#include "third_party/glad/glad.h"

int main()
{
    Window window(1280, 720, "Learn openGL");
    Shader shader_basic("basic");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] {
        -.5f, -.5f,
        -.5f,  .5f,
         .5f,  .5f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, &vertices, GL_STATIC_DRAW );

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);

    shader_basic.use();
    while(window.is_open())
    {
        auto elapsedTime = window.getElapsedTime();
        
        //processing input
        window.poll_events();

        window.clear_src();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.blit_scr();
    }


    return 0;
}