/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_RENDER_HPP
#define UNICORN_RENDER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/system/Timer.hpp>

#include <wink/signal.hpp>
#include <map>

namespace unicorn
{

namespace video
{
class Graphics;
}

namespace system
{
namespace input
{
class Gamepad;
}

class Input;
class Manager;
}

/** @brief  Render main class */
class UnicornRender
{
public:
    /** @brief Bit mask class describing profiling settings */
    struct ProfilingMask
    {
        typedef uint16_t MaskType;

        static const MaskType None      = 0;

        static const MaskType Window    = 1 << 0;
        static const MaskType Monitor   = 1 << 1;
        static const MaskType Mouse     = 1 << 2;
        static const MaskType Key       = 1 << 3;
        static const MaskType Gamepad   = 1 << 4;
    };

    /** @brief  Constructs an empty render */
    UNICORN_EXPORT UnicornRender();

    /** @brief  Destructs render
     *
     *  Also calls Deinit() to release all resources
     *
     *  @sa Deinit()
     */
    UNICORN_EXPORT ~UnicornRender();

    UnicornRender(const UnicornRender& other) = delete;
    UnicornRender& operator=(const UnicornRender& other) = delete;

    UnicornRender(UnicornRender&& other) = delete;
    UnicornRender& operator=(UnicornRender&& other) = delete;

    /** @brief  Initializes the render
     *
     *  Initializes storage and @p m_pGraphics
     *
     *  @param  profilingMask   bit mask describing which profilers shall be activated
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    UNICORN_EXPORT bool Init(ProfilingMask::MaskType profilingMask = ProfilingMask::None);

    /** @brief  Render's main loop */
    UNICORN_EXPORT void Run();

    /** @brief  Deinitializes the render
     *
     *  Deinitializes @ref m_pGraphics
     */
    UNICORN_EXPORT void Deinit();

    /** @brief  Returns pointer to the graphics system */
    UNICORN_EXPORT video::Graphics* GetGraphics() const { return m_pGraphics; }

    /** @brief  Returns pointer to the input system */
    system::Input* GetInput() const { return m_pInput; }

    /** @brief  Event triggered after input processing but before rendering
     *
     *  Event is emitted with the following signature:
     *  -# render pointer
     */
    wink::signal< wink::slot<void(UnicornRender*)> > LogicFrame;

private:
    //! Flag describing if render was initialized
    bool m_isInitialized;

    //! Pointer to the window and monitor managing manager
    system::Manager* m_pSystemManager;

    //! Pointer to the graphics system
    video::Graphics* m_pGraphics;

    //! Pointer to the input system
    system::Input* m_pInput;
};
}

#endif // UNICORN_RENDER_HPP
