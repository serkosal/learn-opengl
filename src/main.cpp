#include "window.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "third_party/glad/glad.h"

int main()
{
    Window window(1280, 720, "Learn openGL");
    Shader shader_basic("basic");
    Texture texture("brick_wall_texture.jpg");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] {
    /*    POSITION       TEXTURE    */
        -.5f, -.5f,     .0f, .0f,
        -.5f,  .5f,     .0f, 1.f,
         .5f,  .5f,     1.f, 1.f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW );

    //position coords
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);

    //texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2 * sizeof(float)) );
    glEnableVertexAttribArray(1);

    shader_basic.use();
    texture.bind();
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