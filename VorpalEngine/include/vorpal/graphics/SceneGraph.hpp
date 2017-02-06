/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_SCENEGRAPH_HPP
#define VORPAL_GRAPHICS_SCENEGRAPH_HPP

#include <list>
#include <vorpal/graphics/Texture.hpp>

namespace vp
{
namespace graphics
{
class SceneGraph
{
public:
    std::list<Texture> sceneGraph;
};
}
}

#endif // VORPAL_GRAPHICS_SCENEGRAPH_HPP
