/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Renderer.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/video/Camera.hpp>

namespace unicorn
{
namespace video
{
Renderer::Renderer(system::Manager& manager, system::Window* window)
    : m_isInitialized(false), m_systemManager(manager), m_pWindow(window), m_camera(new Camera({0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f})), m_backgroundColor({{0.0f, 0.0f, 0.0f, 0.0f}})
{
    m_camera->SetPerspective(45.0f, static_cast<float>(window->GetSize().first) / window->GetSize().second, 0.1f, 100.0f);
}

Renderer::~Renderer()
{
}

std::shared_ptr<Camera> Renderer::GetCamera() const
{
    return m_camera;
}

void Renderer::SetBackgroundColor(const glm::vec3& backgroundColor)
{
    m_backgroundColor.at(0) = backgroundColor.r;
    m_backgroundColor.at(1) = backgroundColor.g;
    m_backgroundColor.at(2) = backgroundColor.b;
    m_backgroundColor.at(3) = 1.0f;
}
}
}
