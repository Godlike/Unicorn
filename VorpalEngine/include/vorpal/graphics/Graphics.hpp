/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_VIDEO_GRAPHICS_HPP
#define VORPAL_VIDEO_GRAPHICS_HPP

namespace vp
{
namespace graphics
{
class VulkanRenderer;

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

    bool Init();
    void Deinit();
    void Render();

private:
    bool m_isInitialized;
    VulkanRenderer* m_pRenderer;
};
}
}

#endif // VORPAL_VIDEO_GRAPHICS_HPP
