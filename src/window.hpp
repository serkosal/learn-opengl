//this file encapsulates: glfw initialization, window creation and managing processes

#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "third_party/glad/glad.h"
#include "third_party/GLFW/glfw3.h"
#include "third_party/glm/glm.hpp"

#include <string_view>
#include <stdexcept>
#include <utility>

class Window
{
private:
    GLFWwindow* _window = nullptr;
    double _oldTime;

    std::pair<double, double> mouse_old;
public:
    enum class key;
    
    Window() = delete;

    Window(int width, int height, std::string_view title, bool isFullscreen=false);

    Window(const Window& other) = delete;
    Window operator=(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window operator=(Window&& other) = delete;

    ~Window()
    {
        glfwTerminate();
    }

    bool is_open() const { return !glfwWindowShouldClose(_window); }
    double elapsed_time()
    {
        auto newTime = glfwGetTime();
        auto elapsedTime = _oldTime - newTime;
        _oldTime = newTime;

        return elapsedTime;
    }
    void clear_src(const glm::vec3& color = glm::vec3(1.f)) const
    {
        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void poll_events() const
    {
        glfwPollEvents();
    }
    void blit_scr() const
    {
        glfwSwapBuffers(_window);
    }

    std::pair<int, int> get_size() const
    {   
        std::pair<int, int> size;
        glfwGetWindowSize(_window, &size.first, &size.second);
        return size;
    }
    
    float aspect_ratio() const
    {
        auto[width, height] = get_size();
        return width / float(height);
    }

    const GLFWwindow* const get_ptr() const { return _window; }

    void close() const
    {
        glfwSetWindowShouldClose(_window, true);
    }

    bool is_key_pressed(Window::key key)
    {
        return glfwGetKey(_window, int(key));
    }

    std::pair<double, double> get_mouse_pos() const
    {
        std::pair<double, double> pos;
        glfwGetCursorPos(_window, &pos.first, &pos.second);
        
        return pos;
    }

    std::pair<double, double> get_mouse_offset()
    {
        auto[x, y] = get_mouse_pos();

        std::pair offset(mouse_old.first - x, mouse_old.second - y);
        
        mouse_old = std::pair(x, y);

        return offset;
    }
};

//string_view is used for avoid unnecessary copying of memory
inline Window::Window(int width, int height, std::string_view title, bool isFullscreen)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (isFullscreen)
        _window = glfwCreateWindow(
            width, height, title.begin(), glfwGetPrimaryMonitor(), NULL
        );
    else
        _window = glfwCreateWindow(
            width, height, title.begin(), NULL, NULL
        );

    if (!_window)
        throw std::runtime_error("Couldn't create GLFW _window!");

    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, 
        [](GLFWwindow* window, int width, int height) -> void
        {
            glViewport(0, 0, width, height);
        }
    );

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    auto [new_width, new_height] = get_size();
    glViewport(0, 0, new_width, new_height);

    //glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    glEnable(GL_DEPTH_TEST);

    _oldTime = glfwGetTime();

    mouse_old = get_mouse_pos();
}

enum class Window::key
{
space       = GLFW_KEY_SPACE,
apostrophe  = GLFW_KEY_APOSTROPHE,
comma       = GLFW_KEY_COMMA,    
minus,    
period,   
slash,

_0 = GLFW_KEY_0,
_1, _2, _3, _4, _5, _6, _7, _8, 
_9,

semicolon   = GLFW_KEY_SEMICOLON,
equal       = GLFW_KEY_EQUAL,    

a = GLFW_KEY_A,
b, c, d, e, f, g, h, i, j, k, l, m,
n, o, p, q, r, s, t, u, v, w, x, y, 
z,

escape = GLFW_KEY_ESCAPE,      
enter,
tab,
backspace,
insert,
del,    
right,
left,        
down,
up,         
page_up,
page_down,
home,    
end,

caps_lock = GLFW_KEY_CAPS_LOCK,    
scroll_lock,
num_lock,     
print_screen,
pause,

f1 = GLFW_KEY_F1,        
f2, f3, f4, f5, f6, f7, f8, f9, f10, f11,
f12,

num_0 = GLFW_KEY_KP_0,    
num_1, num_2, num_3, num_4,
num_5, num_6, num_7, num_8,
num_9,

num_decimal,    
num_divide,    
num_multiply,
num_subtract,
num_add,
num_enter,       
num_equal,

left_shift = GLFW_KEY_LEFT_SHIFT,
left_ctrl,
left_alt,     
left_super,

right_shift,
right_ctrl,
right_alt,    
right_super,
};

#endif
