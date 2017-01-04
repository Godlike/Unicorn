// 73s studio

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vorpal/video/Renderer.hpp>
#include <vorpal/VorpalEngine.hpp>
#include <iostream>

#define error_log(x) std::cout << x << std::endl;

void vp::video::Renderer::deinit()
{
    vkDestroyInstance(_vkInstance, nullptr);
    if (_pWindow)
    {
        glfwDestroyWindow(_pWindow);
    }
}

void vp::video::Renderer::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (glfwVulkanSupported())
    {
      error_log("Vulkan not supported");
    }
    _pWindow = glfwCreateWindow(VorpalEngine::instance()->applicationWidth(),
                                VorpalEngine::instance()->applicationHeight(),
                                VorpalEngine::instance()->applicationName().c_str(),
                                nullptr, nullptr);
    _createInstance();
}

void vp::video::Renderer::update()
{
    while ( !glfwWindowShouldClose(_pWindow) )
    {
        glfwPollEvents();
    }
}

vp::video::Renderer::Renderer()  : _pWindow(nullptr)
{

}
void vp::video::Renderer::_createInstance()
{
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = VorpalEngine::instance()->applicationName().c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); //TODO: remove hardcode
  appInfo.pEngineName = VorpalEngine::instance()->vorpalEngineName().c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0); //TODO: remove hardcode
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  unsigned int glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS) {
    error_log("failed to create instance!");
  }
}
