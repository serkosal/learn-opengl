#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

#include "third_party/glm/gtc/matrix_transform.hpp"

int main()
{
    Window window(1280, 720, "Learn openGL");

    Shader shader_basic;
    shader_basic.init("basic");

    Texture texture;
    texture.init("brick_wall_texture.jpg");

    Camera camera;

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

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));

    shader_basic.use();
    texture.bind();

    bool isW = false, isA = false, isS = false, isD = false;
    while(window.is_open())
    {
        auto elapsedTime = window.elapsed_time();
        
        //processing input
        window.poll_events();

        //window closing
        if (window.is_key_pressed(Window::key::q))
            window.close();

        //camera movement
        glm::vec3 move_offset(0.f);

        if (isW = !isW && window.is_key_pressed(Window::key::w))
            move_offset -= camera.get_dir();
        if (isA = !isA && window.is_key_pressed(Window::key::a))
            move_offset += camera.get_right();
        if (isS = !isS && window.is_key_pressed(Window::key::s))
            move_offset += camera.get_dir();
        if (isD = !isD && window.is_key_pressed(Window::key::d))
            move_offset -= camera.get_right();
        
        move_offset *= camera._speed * elapsedTime;

        camera.move(move_offset);

        auto[cursor_xoffset, cursor_yoffset] = window.get_mouse_offset();
        camera.ProcessMouseMovement(-cursor_xoffset, cursor_yoffset);

        //rendering
        window.clear_src();

        //model = glm::rotate(model, glm::radians(-45.f * float(elapsedTime)), glm::vec3(0.f, 1.f, 0.0));
        shader_basic.setMat4("model", model);
        shader_basic.setMat4("view", camera.get_view());
        shader_basic.setMat4("projection", camera.get_proj(window.aspect_ratio()));

        cube.draw();

        window.blit_scr();
    }


    return 0;
}