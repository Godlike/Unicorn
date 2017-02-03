/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_SCENEGRAPH_HPP
#define VORPAL_GRAPHICS_SCENEGRAPH_HPP

#include <vector>
#include <vorpal/graphics/Drawable.hpp>

namespace vp
{
namespace graphics
{
class SceneGraph
{
public:
    std::vector<vp::graphics::Drawable> graph;
};
}
}

#endif // VORPAL_GRAPHICS_SCENEGRAPH_HPP
