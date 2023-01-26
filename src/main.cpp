#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "third_party/glad/glad.h"

int main()
{
    Window window(1280, 720, "Learn openGL");
    Shader shader_basic("basic");
    Texture texture("brick_wall_texture.jpg");


    Mesh cube(std::vector<Vertex>{
        {{ -0.5f, -0.5f, -0.5f},  {  0.0f,  0.0f, -1.0f}, { 0.f, 0.f } }, // 0
        {{  0.5f, -0.5f, -0.5f},  {  0.0f,  0.0f, -1.0f}, { 1.f, 0.f } }, // 1
        {{  0.5f,  0.5f, -0.5f},  {  0.0f,  0.0f, -1.0f}, { 1.f, 1.f } }, // 2
        {{ -0.5f,  0.5f, -0.5f},  {  0.0f,  0.0f, -1.0f}, { 0.f, 1.f } }, // 3 

        {{ -0.5f, -0.5f,  0.5f},  {  0.0f,  0.0f,  1.0f}, { 0.f, 0.f } }, // 4
        {{  0.5f, -0.5f,  0.5f},  {  0.0f,  0.0f,  1.0f}, { 1.f, 0.f } }, // 5
        {{  0.5f,  0.5f,  0.5f},  {  0.0f,  0.0f,  1.0f}, { 1.f, 1.f } }, // 6
        {{ -0.5f,  0.5f,  0.5f},  {  0.0f,  0.0f,  1.0f}, { 0.f, 1.f } }, // 7

        {{ -0.5f,  0.5f,  0.5f},  { -1.0f,  0.0f,  0.0f}, { 0.f, 0.f } }, // 8
        {{ -0.5f,  0.5f, -0.5f},  { -1.0f,  0.0f,  0.0f}, { 1.f, 0.f } }, // 9
        {{ -0.5f, -0.5f, -0.5f},  { -1.0f,  0.0f,  0.0f}, { 1.f, 1.f } }, //10
        {{ -0.5f, -0.5f,  0.5f},  { -1.0f,  0.0f,  0.0f}, { 0.f, 1.f } }, //11

        {{  0.5f,  0.5f,  0.5f},  {  1.0f,  0.0f,  0.0f}, { 0.f, 0.f } }, //12
        {{  0.5f,  0.5f, -0.5f},  {  1.0f,  0.0f,  0.0f}, { 1.f, 0.f } }, //13
        {{  0.5f, -0.5f, -0.5f},  {  1.0f,  0.0f,  0.0f}, { 1.f, 1.f } }, //14
        {{  0.5f, -0.5f,  0.5f},  {  1.0f,  0.0f,  0.0f}, { 0.f, 1.f } }, //15

        {{ -0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}, { 0.f, 0.f } }, //16
        {{  0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}, { 1.f, 0.f } }, //17
        {{  0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}, { 1.f, 1.f } }, //18
        {{ -0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}, { 0.f, 1.f } }, //19

        {{ -0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}, { 0.f, 0.f } }, //20
        {{  0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}, { 1.f, 0.f } }, //21
        {{  0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}, { 1.f, 1.f } }, //22
        {{ -0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}, { 0.f, 1.f } }  //23
    },
    std::vector<GLuint>{
         0,  1,  2,     0,  2,  3,
         4,  5,  6,     4,  6,  7,
         8,  9, 10,     8, 10, 11,
        12, 13, 14,    12, 14, 15,
        16, 17, 18,    16, 18, 19,
        20, 21, 22,    20, 22, 23 
    });

    shader_basic.use();
    texture.bind();
    while(window.is_open())
    {
        auto elapsedTime = window.getElapsedTime();
        
        //processing input
        window.poll_events();

        window.clear_src();

        cube.draw();

        window.blit_scr();
    }


    return 0;
}