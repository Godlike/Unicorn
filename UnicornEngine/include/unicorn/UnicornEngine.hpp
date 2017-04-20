/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_ENGINE_HPP
#define UNICORN_ENGINE_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <cstdio>
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

class Manager;

class GamepadProfiler;
class KeyProfiler;
class MonitorProfiler;
class MouseProfiler;
class WindowProfiler;
}

/** @brief  Engine main class */
class UNICORN_EXPORT UnicornEngine
{
public:
    /** @brief  Constructs an empty engine */
    UnicornEngine();

    /** @brief  Destructs an engine
     *
     *  Also calls Deinit() to release all resources
     *
     *  @sa Deinit()
     */
    ~UnicornEngine();

    UnicornEngine(const UnicornEngine& other) = delete;
    UnicornEngine& operator=(const UnicornEngine& other) = delete;

    UnicornEngine(UnicornEngine&& other) = delete;
    UnicornEngine& operator=(UnicornEngine&& other) = delete;

    /** @brief  Initializes the engine
     *
     *  Initializes storage and @p m_pGraphics
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    bool Init();

    /** @brief  Engine's main loop */
    void Run();

    /** @brief  Deinitializes the engine
     *
     *  Deinitializes @ref m_pGraphics
     */
    void Deinit();

    /** @brief  Returns pointer to the graphics system */
    video::Graphics* GetGraphics() const { return m_pGraphics; }

    /** @brief  Returns all connected gamepads
     *
     *  @todo   Create system::Input class to control input related stuff when
     *          there will be some extra functionality in addition to gamepads
     *
     *  @return a map of connected gamepads
     */
    const std::map<uint32_t, system::input::Gamepad*>& GetGamepads() const;

private:
    //! Flag describing if engine was initialized
    bool m_isInitialized;

    //! Pointer to the window and monitor managing manager
    system::Manager* m_pSystemManager;

    //! Pointer to the window profiler
    system::WindowProfiler* m_pWindowProfiler;

    //! Pointer to the monitor profiler
    system::MonitorProfiler* m_pMonitorProfiler;

    //! Pointer to the mouse profiler
    system::MouseProfiler* m_pMouseProfiler;

    //! Pointer to the key profiler
    system::KeyProfiler* m_pKeyProfiler;

    //! Pointer to the gamepad profiler
    system::GamepadProfiler* m_pGamepadProfiler;

    //! Pointer to the graphics system
    video::Graphics* m_pGraphics;
};
}

#endif // UNICORN_ENGINE_HPP
