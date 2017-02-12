/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GRAPHICS_HPP
#define UNICORN_VIDEO_GRAPHICS_HPP

#include <vector>

#include <unicorn/system/Window.hpp>

namespace uc
{
namespace graphics
{
class VulkanRenderer;
class VulkanSubsystem;
class VulkanContext;
class VulkanDevice;
class GraphicsSubsystem;

/** @brief Abstract graphics renderer
 *
 *  Initializes graphics system and proxies render calls to the renderer
 */
class Graphics
{
public:
    Graphics();
    ~Graphics();

    Graphics(const Graphics& other) = delete;
    Graphics(const Graphics&& other) = delete;
    Graphics& operator=(const Graphics& other) = delete;
    Graphics& operator=(const Graphics&& other) = delete;

    bool Init(const system::Window& renderWindow);
    void Deinit();
    void Render();
    void WindowSizeChanged();

private:
    bool m_isInitialized;
    GraphicsSubsystem* m_pGraphicsSubsystem;
    VulkanRenderer* m_pRenderer;
    VulkanDevice* m_pVkDevice;
};
}
}

#endif // UNICORN_VIDEO_GRAPHICS_HPP
