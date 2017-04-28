/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_ENGINE_HPP
#define UNICORN_ENGINE_HPP

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

/** @brief  Engine main class */
class UnicornEngine
{
public:
    /** @brief  Constructs an empty engine */
    UNICORN_EXPORT UnicornEngine();

    /** @brief  Destructs an engine
     *
     *  Also calls Deinit() to release all resources
     *
     *  @sa Deinit()
     */
    UNICORN_EXPORT ~UnicornEngine();

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
    UNICORN_EXPORT bool Init();

    /** @brief  Engine's main loop */
    UNICORN_EXPORT void Run();

    /** @brief  Deinitializes the engine
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
     *  -# engine pointer
     */
    wink::signal< wink::slot<void(UnicornEngine*)> > LogicFrame;

private:
    //! Flag describing if engine was initialized
    bool m_isInitialized;

    //! Pointer to the window and monitor managing manager
    system::Manager* m_pSystemManager;

    //! Pointer to the graphics system
    video::Graphics* m_pGraphics;

    //! Pointer to the input system
    system::Input* m_pInput;
};
}

#endif // UNICORN_ENGINE_HPP
