#ifndef VORPAL_RENDERER_HPP
#define VORPAL_RENDERER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct GLFWwindow;

namespace vp {
namespace video {
/**
 * @brief The Renderer class abstract low level Vulkan initialization and updating.
 */
class Renderer {
 public:
  Renderer();
  Renderer(const Renderer &other) = delete;
  Renderer(const Renderer &&other) = delete;
  Renderer &operator=(const Renderer &other) = delete;
  Renderer &operator=(const Renderer &&other) = delete;

  bool init();
  void deinit();
  void render();
 private:
  GLFWwindow *m_pWindow;
  VkInstance m_vkInstance;
  const std::vector<const char *> m_validationLayers;
  VkDebugReportCallbackEXT m_vulkanCallback;
#ifdef NDEBUG
  const bool m_enableValidationLayers = false;
#else
  const bool m_enableValidationLayers = true;
#endif

  bool createInstance();
  bool checkValidationLayerSupport();
  std::vector<const char *> getRequiredExtensions();
  bool setupDebugCallback();
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,
                                                      VkDebugReportObjectTypeEXT objType,
                                                      uint64_t obj,
                                                      size_t location,
                                                      int32_t code,
                                                      const char *layerPrefix,
                                                      const char *msg,
                                                      void *userData);
};
}
}

#endif // VORPAL_RENDERER_HPP
