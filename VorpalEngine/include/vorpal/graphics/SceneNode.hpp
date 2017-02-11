/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_SCENENODE_HPP
#define VORPAL_GRAPHICS_SCENENODE_HPP

#include <list>

namespace vp
{
    namespace graphics
    {
        class SceneNode
        {
        private:
            std::list<SceneNode> m_nodes;
        };
    }
}

#endif // VORPAL_GRAPHICS_SCENENODE_HPP
