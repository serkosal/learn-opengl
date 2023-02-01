#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

#include "third_party/glm/gtc/matrix_transform.hpp"

int main()
{
    Window window(1980, 1080, "Learn openGL", true);

    Shader shader_basic("basic");

    //Model backpack("backpack/backpack.obj", true);
    Model backpack("survival_guitar_backpack/scene.gltf");
    backpack._trans = glm::rotate(
            backpack._trans,
            glm::radians(-90.f),
            glm::vec3(1.f, 0.f, 0.f)
    );

    Camera camera;


    camera.isCaptured = false;
    camera._speed = 5.f;
    bool isClicked = false;
    while(window.is_open())
    {
        auto elapsedTime = window.elapsed_time();
        
        // backpack._trans = glm::rotate(
        //     backpack._trans,
        //     float(glm::radians(22.5f * elapsedTime)),
        //     glm::vec3(0.f, 1.f, 0.f)
        // );

        //processing input
        window.poll_events();

        if (window.is_mouse_clicked())
            isClicked = true;

        if (!window.is_mouse_clicked() && isClicked)
        {
            if (camera.isCaptured = !camera.isCaptured)
                window.set_cursor_mode(Window::cursor_modes::disabled);
            else
                window.set_cursor_mode(Window::cursor_modes::normal);
            

            isClicked = false;
        }
        if (camera.isCaptured)
        {
            auto[cursor_xoffset, cursor_yoffset] = window.get_mouse_offset();
            camera.ProcessMouseMovement(-cursor_xoffset, cursor_yoffset);
        }

        //window closing
        if (window.is_key_pressed(Window::key::q))
            window.close();

        float velocity = elapsedTime * camera._speed;
        if (window.is_key_pressed(Window::key::w))
            camera.move(-camera.get_dir() * velocity);
        if (window.is_key_pressed(Window::key::a))
            camera.move( camera.get_right() * velocity);
        if (window.is_key_pressed(Window::key::s))
            camera.move( camera.get_dir() * velocity);
        if (window.is_key_pressed(Window::key::d))
            camera.move(-camera.get_right() * velocity);
        if (window.is_key_pressed(Window::key::space))
            camera.move(-camera.get_up() * velocity);
        if (window.is_key_pressed(Window::key::left_shift))
            camera.move(camera.get_up() * velocity);

        //rendering
        window.clear_src(glm::vec3(0.f, 0.f, 0.f));

        backpack.draw(camera, shader_basic, window.aspect_ratio());

        window.blit_scr();
    }


    return 0;
}