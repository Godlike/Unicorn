/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_HPP
#define UNICORN_SYSTEM_INPUT_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <wink/event_queue.hpp>

#include <map>

namespace unicorn
{
namespace system
{

namespace input
{
class Gamepad;
}

class Manager;

/** @brief  Input gateway system
 *
 *  Initializes and controls all supported inputs
 */
class Input
{
public:
    /** @brief  Constructs the input system */
    Input(system::Manager& manager);

    Input(const Input& other) = delete;
    Input& operator=(const Input& other) = delete;

    Input(Input&& other) = delete;
    Input& operator=(Input&& other) = delete;

    /** @brief  Destructs the input system */
    ~Input();

    /** @brief  Returns clipboard data
     *
     *  @note   If clipboard contents can't be represented as UTF-8 string
     *          the result will be empty
     *
     *  @return clipboard string or empty string if some problem occured
     */
    UNICORN_EXPORT std::string GetClipboard() const;

    /** @brief  Sets clipboard data
     *
     *  @param  data    new clipboard data
     */
    UNICORN_EXPORT void SetClipboard(const std::string& data) const;

    /** @brief  Returns a map of connected gamepads */
    UNICORN_EXPORT const std::map<uint32_t, input::Gamepad*>& GetGamepads() const;

    /** @brief  Process all inputs */
    void Process();

    /** @brief  Event queue triggered every time new bath of gamepads is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created gamepad
     */
    wink::event_queue<input::Gamepad*>& GamepadCreated;

private:
    //! Reference to window and monitor managing manager
    system::Manager& m_systemManager;
};

}
}

#endif // UNICORN_SYSTEM_INPUT_HPP
