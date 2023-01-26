#include "window.hpp"

int main()
{
    Window window(1280, 720, "Learn openGL");

    while(window.is_open())
    {
        auto elapsedTime = window.getElapsedTime();
        
        //processing input
        window.poll_events();

        window.clear_src();

        window.blit_scr();
    }


    return 0;
}