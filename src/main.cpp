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

    Model backpack("backpack/backpack.obj");

    Camera camera;


    bool isW = false, isA = false, isS = false, isD = false;
    camera.isActive = false;
    camera.isCaptured = false;
    camera._speed = 5.f;
    while(window.is_open())
    {
        auto elapsedTime = window.elapsed_time();
        
        //processing input
        window.poll_events();

        //camera movement
        glm::vec3 move_offset(0.f);

        if (window.is_mouse_clicked() && !camera.isCaptured)
            camera.isCaptured = camera.isActive = true;
        else if (window.is_mouse_clicked() && camera.isCaptured)
            camera.isCaptured = camera.isActive = false;
        
        if (!window.is_mouse_clicked())
        {
            if(camera.isCaptured)
                window.set_cursor_mode(Window::cursor_modes::disabled);
            else
                window.set_cursor_mode(Window::cursor_modes::normal);
        }
        

        if (camera.isActive)
        {
            auto[cursor_xoffset, cursor_yoffset] = window.get_mouse_offset();
            camera.ProcessMouseMovement(-cursor_xoffset, cursor_yoffset);
        }

        //window closing
        if (window.is_key_pressed(Window::key::q))
            window.close();

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


        //rendering
        window.clear_src(glm::vec3(0.f, 0.f, 0.f));

        backpack.draw(shader_basic, camera.get_view(), camera.get_proj(window.aspect_ratio()));

        window.blit_scr();
    }


    return 0;
}