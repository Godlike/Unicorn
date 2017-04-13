/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_ENGINE_HPP
#define UNICORN_ENGINE_HPP

#include <unicorn/utility/SharedMacros.hpp>

namespace unicorn
{

namespace video
{
class Graphics;
}

namespace system
{
class Manager;
class WindowProfiler;
class MonitorProfiler;
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

private:
    //! Flag describing if engine was initialized
    bool m_isInitialized;

    //! Pointer to the window and monitor managing manager
    system::Manager* m_pSystemManager;

    //! Pointer to the window profiler
    system::WindowProfiler* m_pWindowProfiler;

    //! Pointer to the monitor profiler
    system::MonitorProfiler* m_pMonitorProfiler;

    //! Pointer to the graphics system
    video::Graphics* m_pGraphics;
};
}

#endif // UNICORN_ENGINE_HPP
