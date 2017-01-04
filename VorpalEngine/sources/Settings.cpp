#include <vorpal/core/Settings.hpp>
#include <loguru.hpp>

vp::core::Settings::Settings(int argc, char **argv) {
  loguru::init(argc, argv);
}
