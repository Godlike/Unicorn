/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_INPUT_GAMEPAD_HPP
#define UNICORN_SYSTEM_INPUT_GAMEPAD_HPP

#include <wink/signal.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace unicorn
{
namespace system
{
namespace input
{

/** @brief  Provides interface to gamepad */
class Gamepad
{
public:
    /** @brief  Constructs a gamepad with provided id */
    explicit Gamepad(void* handle);

    Gamepad(const Gamepad& other) = delete;
    Gamepad& operator=(const Gamepad& other) = delete;

    Gamepad(Gamepad&& other) = delete;
    Gamepad& operator=(Gamepad&& other) = delete;

    /** @brief  Destructs a gamepad object */
    ~Gamepad();

    /** @brief  Returns the id of the gamepad */
    uint32_t GetId() const { return m_id; }

    /** @brief  Returns the name of the gamepad */
    const std::string& GetName() const { return m_name; }

    /** @brief  Returns the current state of all axes of the gamepad */
    const std::vector<float>& GetAxes() const { return m_axes; }

    /** @brief  Returns the current state of all axes of the gamepad */
    const std::vector<bool>& GetButtons() const { return m_buttons; }

    /** @brief  Refresh axes and buttons */
    void UpdateData();

    /** @brief  Event triggered after the gamepad information has been updated
     *
     *  Event is emitted with the following signature:
     *  -# gamepad pointer
     */
    wink::signal< wink::slot<void(Gamepad*)> > Updated;

    /** @brief  Event triggered from destructor before the gamepad is destroyed
     *
     *  Event is emitted with the following signature:
     *  -# gamepad pointer
     */
    wink::signal< wink::slot<void(Gamepad*)> > Destroyed;

private:
    //! id
    uint32_t m_id;

    //! Name
    std::string m_name;

    //! Vector of axis values
    std::vector<float> m_axes;

    //! Vector of button states
    std::vector<bool> m_buttons;

    //! Handle provided by adapter
    void* m_handle;
};

}
}
}

#endif /* UNICORN_SYSTEM_INPUT_GAMEPAD_HPP */
