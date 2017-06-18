/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Window.hpp>

#include <unicorn/system/adapter/Helper.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace system
{

Window::Window(uint32_t id, int32_t width, int32_t height,
    const std::string& name, Monitor* pMonitor, Window* pSharedWindow)
    : m_id( id )
    , m_size( width, height )
    , m_position( 0, 0 )
    , m_name( name )
    , m_pMonitor( pMonitor )
    , m_pSharedWindow( pSharedWindow )
    , m_focus( false )
    , m_modifiers( input::Modifier::None )
    , m_handle( nullptr )
{
    m_handle = WINDOW_MANAGER_ADAPTER::CreateWindow(m_size.first
        , m_size.second
        , m_name.c_str()
        , m_pMonitor ? m_pMonitor->GetHandle() : nullptr
        , m_pSharedWindow ? m_pSharedWindow->GetHandle() : nullptr
    );

    WINDOW_MANAGER_ADAPTER::GetWindowPosition(m_handle, &m_position.first, &m_position.second);

    WINDOW_MANAGER_ADAPTER::WindowPositionChanged.connect(this, &Window::OnWindowPositionChanged);
    WINDOW_MANAGER_ADAPTER::WindowSizeChanged.connect(this, &Window::OnWindowSizeChanged);
    WINDOW_MANAGER_ADAPTER::WindowClose.connect(this, &Window::OnWindowClose);
    WINDOW_MANAGER_ADAPTER::WindowContentRefresh.connect(this, &Window::OnWindowContentRefresh);
    WINDOW_MANAGER_ADAPTER::WindowFocused.connect(this, &Window::OnWindowFocused);
    WINDOW_MANAGER_ADAPTER::WindowMinimized.connect(this, &Window::OnWindowMinimized);
    WINDOW_MANAGER_ADAPTER::WindowMaximized.connect(this, &Window::OnWindowMaximized);
    WINDOW_MANAGER_ADAPTER::WindowFramebufferResized.connect(this, &Window::OnWindowFramebufferResized);

    WINDOW_MANAGER_ADAPTER::WindowMouseButton.connect(this, &Window::OnWindowMouseButton);
    WINDOW_MANAGER_ADAPTER::WindowMousePosition.connect(this, &Window::OnWindowMousePosition);
    WINDOW_MANAGER_ADAPTER::WindowMouseEnter.connect(this, &Window::OnWindowMouseEnter);
    WINDOW_MANAGER_ADAPTER::WindowScroll.connect(this, &Window::OnWindowScroll);
    WINDOW_MANAGER_ADAPTER::WindowKeyboard.connect(this, &Window::OnWindowKeyboard);
    WINDOW_MANAGER_ADAPTER::WindowUnicode.connect(this, &Window::OnWindowUnicode);
    WINDOW_MANAGER_ADAPTER::WindowFileDrop.connect(this, &Window::OnWindowFileDrop);
}

Window::~Window()
{
    Destroyed.emit(this);

    Destroyed.clear();
    PositionChanged.clear();
    SizeChanged.clear();
    Close.clear();
    ContentRefresh.clear();
    Focused.clear();
    Minimized.clear();
    Maximized.clear();
    CursorShapeChanged.clear();
    MouseModeChanged.clear();
    StickyMouseChanged.clear();
    StickyKeysChanged.clear();
    FramebufferResized.clear();

    MouseButton.clear();
    MousePosition.clear();
    MouseEnter.clear();
    Scroll.clear();
    Keyboard.clear();
    Unicode.clear();
    FileDrop.clear();

    WINDOW_MANAGER_ADAPTER::DestroyWindow(m_handle);
}

bool Window::ShouldClose() const
{
    return WINDOW_MANAGER_ADAPTER::WindowShouldClose(m_handle);
}

void Window::SetShouldClose(bool flag) const
{
    WINDOW_MANAGER_ADAPTER::SetWindowShouldClose(m_handle, flag);
}

void Window::SetName(const std::string& name)
{
    WINDOW_MANAGER_ADAPTER::SetWindowName(m_handle, name.c_str());
}

void Window::SetSize(std::pair<int32_t, int32_t> size)
{
    WINDOW_MANAGER_ADAPTER::SetWindowSize(m_handle, size.first, size.second);
}

void Window::SetSizeLimits(std::pair<int32_t, int32_t> minSize, std::pair<int32_t, int32_t> maxSize)
{
    WINDOW_MANAGER_ADAPTER::SetWindowSizeLimits(m_handle, minSize, maxSize);
}

void Window::SetAspectRatio(std::pair<int32_t, int32_t> ratio)
{
    WINDOW_MANAGER_ADAPTER::SetWindowAspectRatio(m_handle, ratio);
}

void Window::SetPosition(std::pair<int32_t, int32_t> position)
{
    WINDOW_MANAGER_ADAPTER::SetWindowPosition(m_handle, position.first, position.second);
}

void Window::Minimize()
{
    WINDOW_MANAGER_ADAPTER::MinimizeWindow(m_handle);
}

void Window::Restore()
{
    WINDOW_MANAGER_ADAPTER::RestoreWindow(m_handle);
}

void Window::Maximize()
{
    WINDOW_MANAGER_ADAPTER::MaximizeWindow(m_handle);
}

void Window::Focus()
{
    WINDOW_MANAGER_ADAPTER::FocusWindow(m_handle);
}

void Window::Hide()
{
    WINDOW_MANAGER_ADAPTER::HideWindow(m_handle);
}

void Window::Show()
{
    WINDOW_MANAGER_ADAPTER::ShowWindow(m_handle);
}

void Window::SetCursorShape(CursorShape shape)
{
    WINDOW_MANAGER_ADAPTER::SetWindowCursorShape(m_handle, shape);

    CursorShapeChanged.emit(this, shape);
}

MouseMode Window::GetMouseMode() const
{
    return WINDOW_MANAGER_ADAPTER::GetWindowMouseMode(m_handle);
}

void Window::SetMouseMode(MouseMode mode)
{
    WINDOW_MANAGER_ADAPTER::SetWindowMouseMode(m_handle, mode);

    MouseModeChanged.emit(this, mode);
}

bool Window::GetStickyMouse() const
{
    return WINDOW_MANAGER_ADAPTER::GetWindowStickyMouse(m_handle);
}

void Window::SetStickyMouse(bool flag)
{
    WINDOW_MANAGER_ADAPTER::SetWindowStickyMouse(m_handle, flag);

    StickyMouseChanged.emit(this, flag);
}

bool Window::GetStickyKeys() const
{
    return WINDOW_MANAGER_ADAPTER::GetWindowStickyKeys(m_handle);
}

void Window::SetStickyKeys(bool flag)
{
    WINDOW_MANAGER_ADAPTER::SetWindowStickyKeys(m_handle, flag);

    StickyKeysChanged.emit(this, flag);
}

void Window::ClearInputEvents()
{
    m_skipMouseButtons.clear();
    m_skipKeys.clear();

    MouseButton.clear();
    Keyboard.clear();
}

void Window::UpdateInputModifiers()
{
    m_modifiers = input::Modifier::None;

    if (input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::LeftControl) ||
        input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::RightControl))
    {
        m_modifiers |= input::Modifier::Ctrl;
    }

    if (input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::LeftAlt) ||
        input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::RightAlt))
    {
        m_modifiers |= input::Modifier::Alt;
    }

    if (input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::LeftShift) ||
        input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::RightShift))
    {
        m_modifiers |= input::Modifier::Shift;
    }

    if (input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::LeftSuper) ||
        input::Action::Release != WINDOW_MANAGER_ADAPTER::GetWindowKey(m_handle, input::Key::RightSuper))
    {
        m_modifiers |= input::Modifier::Super;
    }
}

void Window::TriggerMouseButtonEvents()
{
    for (auto const& button : m_pressedMouseButtons)
    {
        if (!m_skipMouseButtons.count(button))
        {
            MouseButton.push({this, button, input::Action::Repeat, m_modifiers});
        }
    }

    MouseButton.emit();
}

void Window::TriggerKeyboardEvents()
{
    for (auto const& key : m_pressedKeys)
    {
        if (!m_skipKeys.count(key))
        {
            Keyboard.push({this, key, WINDOW_MANAGER_ADAPTER::GetKeyScancode(key), input::Action::Repeat, m_modifiers});
        }
    }

    Keyboard.emit();
}

void Window::OnWindowPositionChanged(void* handle, std::pair<int32_t, int32_t> position)
{
    if (handle == m_handle)
    {
        m_position = position;

        PositionChanged.emit(this, m_position);
    }
}

void Window::OnWindowSizeChanged(void* handle, std::pair<int32_t, int32_t> size)
{
    if (handle == m_handle)
    {
        m_size = size;

        SizeChanged.emit(this, m_size);
    }
}

void Window::OnWindowClose(void* handle)
{
    if (handle == m_handle)
    {
        Close.emit(this);
    }
}

void Window::OnWindowContentRefresh(void* handle)
{
    if (handle == m_handle)
    {
        ContentRefresh.emit(this);
    }
}

void Window::OnWindowFocused(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        m_focus = flag;

        if (!m_focus)
        {
            m_pressedKeys.clear();
            m_pressedMouseButtons.clear();
        }

        Focused.emit(this, flag);
    }
}

void Window::OnWindowMinimized(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        Minimized.emit(this, flag);
    }
}

void Window::OnWindowMaximized(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        Maximized.emit(this, flag);
    }
}

void Window::OnWindowFramebufferResized(void* handle, std::pair<int32_t, int32_t> size)
{
    if (handle == m_handle)
    {
        FramebufferResized.emit(this, size);
    }
}

void Window::OnWindowMouseButton(void* handle, input::MouseButton button, input::Action action, input::Modifier::Mask modifiers)
{
    if (handle == m_handle)
    {
        m_skipMouseButtons.insert(button);

        switch (action)
        {
            case input::Action::Press:
            {
                m_pressedMouseButtons.insert(button);
                break;
            }
            case input::Action::Release:
            {
                m_pressedMouseButtons.erase(button);
                break;
            }
            default:
            {
                break;
            }
        }

        MouseButton.push({this, button, action, modifiers});
    }
}

void Window::OnWindowMousePosition(void* handle, std::pair<double, double> coords)
{
    if (handle == m_handle)
    {
        MousePosition.emit(this, coords);
    }
}

void Window::OnWindowMouseEnter(void* handle, bool entered)
{
    if (handle == m_handle)
    {
        MouseEnter.emit(this, entered);
    }
}

void Window::OnWindowScroll(void* handle, std::pair<double, double> coords)
{
    if (handle == m_handle)
    {
        Scroll.emit(this, coords);
    }
}

void Window::OnWindowKeyboard(void* handle, input::Key key, uint32_t scancode, input::Action action, input::Modifier::Mask modifiers)
{
    if (handle == m_handle)
    {
        m_skipKeys.insert(key);

        switch (action)
        {
            case input::Action::Press:
            {
                m_pressedKeys.insert(key);
                break;
            }
            case input::Action::Release:
            {
                m_pressedKeys.erase(key);

                break;
            }
            default:
            {
                break;
            }
        }

        Keyboard.push({this, key, scancode, action, modifiers});
    }
}

void Window::OnWindowUnicode(void* handle, uint32_t unicode, input::Modifier::Mask modifiers)
{
    if (handle == m_handle)
    {
        Unicode.emit(this, unicode, modifiers);
    }
}

void Window::OnWindowFileDrop(void* handle, std::vector<std::string> paths)
{
    if (handle == m_handle)
    {
        FileDrop.emit(this, paths);
    }
}

}
}
