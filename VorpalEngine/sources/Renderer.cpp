#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vorpal/video/Renderer.hpp>
#include <vorpal/core/Logger.hpp>
#include <vorpal/core/Settings.hpp>

void vp::video::Renderer::deinit() {
  vkDestroyInstance(m_vkInstance, nullptr);
  if (m_pWindow) {
    glfwDestroyWindow(m_pWindow);
  }
}

bool vp::video::Renderer::init() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  if (!glfwVulkanSupported()) {
    LOG_ERROR("Vulkan not supported!");
    return false;
  }
  m_pWindow = glfwCreateWindow(
      core::Settings::instance()->applicationWidth(),
      core::Settings::instance()->applicationHeight(),
      core::Settings::instance()->applicationName().c_str(), nullptr, nullptr);
  if(!m_createInstance())
    return false;
  LOG_INFO("Render initialized correctly.");
  return true;
}

void vp::video::Renderer::render() {
  while (!glfwWindowShouldClose(m_pWindow)) {
    glfwPollEvents();
  }
}

vp::video::Renderer::Renderer() : m_pWindow(nullptr) {}

bool vp::video::Renderer::m_createInstance() {
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName =
      core::Settings::instance()->applicationName().c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: remove
                                                         // hardcode
  appInfo.pEngineName = core::Settings::instance()->vorpalEngineName().c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0); // TODO: remove hardcode
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  unsigned int glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
    LOG_ERROR("Failed to create instance!");
    return false;
  }
  return true;
}
