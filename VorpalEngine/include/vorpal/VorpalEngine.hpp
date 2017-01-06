#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <string>

#include <vorpal/core/Utility.hpp>
#include <vorpal/video/Graphics.hpp>

namespace vp {
class VORPAL_EXPORT VorpalEngine {
 public:
  VorpalEngine();
  ~VorpalEngine();

  VorpalEngine(const VorpalEngine &other) = delete;
  VorpalEngine(const VorpalEngine &&other) = delete;
  VorpalEngine &operator=(const VorpalEngine &other) = delete;
  VorpalEngine &operator=(const VorpalEngine &&other) = delete;

  bool init();
  void run();
  void deinit();
 private:
  video::Graphics *m_pGraphics;
};
}

#endif // VORPAL_ENGINE_HPP
