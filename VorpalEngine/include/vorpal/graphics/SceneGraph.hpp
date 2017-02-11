/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_SCENEGRAPH_HPP
#define VORPAL_GRAPHICS_SCENEGRAPH_HPP

#include <list>
#include <vorpal/graphics/SceneNode.hpp>

namespace vp
{
namespace graphics
{
class SceneGraph
{
public:
    std::list<SceneNode> sceneGraph;
};
}
}

#endif // VORPAL_GRAPHICS_SCENEGRAPH_HPP
