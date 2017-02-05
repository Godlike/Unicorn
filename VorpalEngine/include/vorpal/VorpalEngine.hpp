/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <vorpal/utility/SharedMacros.hpp>

namespace vp
{
namespace video
{
class Graphics;
}

/** @brief  Engine main class */
class VORPAL_EXPORT VorpalEngine
{
public:
    /** @brief  Constructs an empty engine */
    VorpalEngine();

    /** @brief  Destructs an engine
     *
     *  Also calls Deinit() to release all resources
     *
     *  @sa Deinit()
     */
    ~VorpalEngine();

    VorpalEngine(const VorpalEngine& other) = delete;
    VorpalEngine& operator=(const VorpalEngine& other) = delete;

    VorpalEngine(VorpalEngine&& other) = delete;
    VorpalEngine& operator=(VorpalEngine&& other) = delete;

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

private:
    //! Flag describing if engine was initialized
    bool m_isInitialized;

    //! Pointer to graphics system
    video::Graphics* m_pGraphics;
};
}

#endif // VORPAL_ENGINE_HPP
