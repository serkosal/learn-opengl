//this file encapsulates: glfw initialization, window creation and managing processes

#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "third_party/glad/glad.h"
#include "third_party/GLFW/glfw3.h"

#include <string_view>
#include <stdexcept>

class Window
{
private:
    GLFWwindow* _window = nullptr;
    double _oldTime;
public:
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

    bool is_open() { return !glfwWindowShouldClose(_window); }
    double getElapsedTime()
    {
        auto newTime = glfwGetTime();
        auto elapsedTime = _oldTime - newTime;
        _oldTime = newTime;

        return elapsedTime;
    }
    void clear_src()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void poll_events()
    {
        glfwPollEvents();
    }
    void blit_scr()
    {
        glfwSwapBuffers(_window);
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
        [](GLFWwindow* _window, int width, int height) -> void
        {
            glViewport(0, 0, width, height);
        }
    );

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");


    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glEnable(GL_DEPTH_TEST);

    _oldTime = glfwGetTime();
}

#endif