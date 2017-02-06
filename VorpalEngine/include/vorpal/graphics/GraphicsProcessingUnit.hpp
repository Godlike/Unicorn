/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_GPU_HPP
#define VORPAL_GRAPHICS_GPU_HPP

#include <string>

namespace vp
{
namespace graphics
{
/**
 * @brief The GraphicsProcessingUnit is GPUs found in system.
 */
struct GraphicsProcessingUnit
{
    std::string gpuName;
    uint32_t memory;
};
using GPU = GraphicsProcessingUnit;
}
}

#endif // VORPAL_GRAPHICS_GPU_HPP
