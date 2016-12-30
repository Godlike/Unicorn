// 73s studio

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <functional>

namespace vp {

template<typename T>
class VkDeleter {
public:
  VkDeleter() : VkDeleter([](T, VkAllocationCallbacks *) {}) {}

  VkDeleter(std::function<void(T, VkAllocationCallbacks *)> deletef) {
      this->deleter = [=](T obj) { deletef(obj, nullptr); };
  }

  VkDeleter(const VkDeleter<VkInstance> &instance,
            std::function<void(VkInstance, T, VkAllocationCallbacks *)> deletef) {
      this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
  }

  VkDeleter(const VkDeleter<VkDevice> &device,
            std::function<void(VkDevice, T, VkAllocationCallbacks *)> deletef) {
      this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
  }

  ~VkDeleter() {
      cleanup();
  }

  const T *operator&() const {
      return &object;
  }

  T *replace() {
      cleanup();
      return &object;
  }

  operator T() const {
      return object;
  }

  void operator=(T rhs) {
      if (rhs != object) {
          cleanup();
          object = rhs;
      }
  }

  template<typename V>
  bool operator==(V rhs) {
      return object == T(rhs);
  }

private:
  T object{VK_NULL_HANDLE};
  std::function<void(T)> deleter;

  void cleanup() {
      if (object != VK_NULL_HANDLE) {
          deleter(object);
      }
      object = VK_NULL_HANDLE;
  }
};
}

