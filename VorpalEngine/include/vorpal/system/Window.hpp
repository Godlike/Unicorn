/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_SYSTEM_WINDOW_HPP
#define VORPAL_SYSTEM_WINDOW_HPP

#include <GLFW/glfw3.h>

namespace vp
{
namespace graphics
{
class Graphics;
}
namespace system
{
/**
* @brief Main window of application.
*/
class Window
{
public:
    Window();
    ~Window();

    Window(const Window& other) = delete;
    Window(const Window&& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window& operator=(const Window&& other) = delete;

    bool Init();
    void Deinit();
    void SetGraphicsEngine(graphics::Graphics* graphicsEngine);
    void RetrieveEvents() const;
    static void onWindowResized(GLFWwindow* window, int width, int height);
    bool ShouldClose() const;
    void Close();

private:
    GLFWwindow* m_pWindow;
    graphics::Graphics* m_Graphics;
};
}
}

#endif
