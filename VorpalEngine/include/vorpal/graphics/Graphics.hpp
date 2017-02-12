/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_VIDEO_GRAPHICS_HPP
#define VORPAL_VIDEO_GRAPHICS_HPP

#include <vector>

#include <vorpal/graphics/GraphicsProcessingUnit.hpp>
#include <vorpal/system/Window.hpp>

namespace vp
{
namespace graphics
{
//class VulkanRenderer;
class VulkanSubsystem;
class VulkanContext;
class VulkanDevice;

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
   // VulkanRenderer* m_pRenderer;
    VulkanDevice* m_pVkDevice;
    std::vector<GPU> m_videoCards;
};
}
}

#endif // VORPAL_VIDEO_GRAPHICS_HPP
