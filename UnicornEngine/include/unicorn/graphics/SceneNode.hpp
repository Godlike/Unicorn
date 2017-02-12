/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_SCENENODE_HPP
#define UNICORN_GRAPHICS_SCENENODE_HPP

#include <list>

namespace uc
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

#endif // UNICORN_GRAPHICS_SCENENODE_HPP
