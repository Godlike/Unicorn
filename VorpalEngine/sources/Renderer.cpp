#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vorpal/video/Renderer.hpp>
#include <vorpal/core/Logger.hpp>
#include <vorpal/core/Settings.hpp>
#include <cstring>

VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                      const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugReportCallbackEXT *pCallback) {
  auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pCallback);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void vp::video::Renderer::deinit() {
  if (m_vkInstance != VK_NULL_HANDLE)
    vkDestroyInstance(m_vkInstance, nullptr);
  if (m_pWindow) {
    glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
  }
}

bool vp::video::Renderer::init() {
  LOG_INFO("Vulkan render initialization started.");
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
  if (!createInstance() && !setupDebugCallback())
    return false;
  LOG_INFO("Vulkan render initialized correctly.");
  return true;
}

void vp::video::Renderer::render() {
  while (!glfwWindowShouldClose(m_pWindow)) {
    glfwPollEvents();
  }
}

vp::video::Renderer::Renderer() : m_vkInstance(nullptr), m_pWindow(nullptr), m_validationLayers({
                                                                                                    "VK_LAYER_LUNARG_standard_validation"}) {
}

bool vp::video::Renderer::createInstance() {
  if (m_enableValidationLayers && !checkValidationLayerSupport()) {
    LOG_ERROR("Vulkan validation layers requested, but not available!");
    return false;
  }
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName =
      core::Settings::instance()->applicationName().c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: remove hardcode
  appInfo.pEngineName = core::Settings::instance()->vorpalEngineName().c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0); // TODO: remove hardcode
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  auto extensions = getRequiredExtensions();

  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (m_enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
    createInfo.ppEnabledLayerNames = m_validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
    LOG_ERROR("Failed to create instance!");
    return false;
  }
  return true;
}

bool vp::video::Renderer::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : m_validationLayers) {
    bool layerFound = false;
    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      LOG_ERROR("Can't find required Vulkan layers: ");
      for (auto &requiredLayer : m_validationLayers)
        LOG_ERROR(requiredLayer);
      return false;
    }
  }
  return true;
}
std::vector<const char *> vp::video::Renderer::getRequiredExtensions() {
  std::vector<const char *> extensions;

  unsigned int glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  for (unsigned int i = 0; i < glfwExtensionCount; i++) {
    extensions.push_back(glfwExtensions[i]);
  }

  if (m_enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  return extensions;
}
bool vp::video::Renderer::setupDebugCallback() {
  if (!m_enableValidationLayers) return true;
  VkDebugReportCallbackCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  return !(CreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_vulkanCallback) != VK_SUCCESS);
}
VkBool32 vp::video::Renderer::debugCallback(VkDebugReportFlagsEXT flags,
                                            VkDebugReportObjectTypeEXT objType,
                                            uint64_t obj,
                                            size_t location,
                                            int32_t code,
                                            const char *layerPrefix,
                                            const char *msg,
                                            void *userData) {
  LOG_ERROR("Validation layer : ", msg);
  return VK_FALSE;
}
