#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <vorpal/core/Utility.hpp>

namespace vp {

namespace video {
class Graphics;
}

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
