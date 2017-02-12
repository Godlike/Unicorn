/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_OPENGL_GRAPHICS_SUBSYSTEM_HPP
#define UNICORN_GRAPHICS_OPENGL_GRAPHICS_SUBSYSTEM_HPP

#include <unicorn/graphics/GraphicsSubsystem.hpp>

namespace uc
{
    namespace graphics
    {
        class OpenGLGraphicsSubsystem : public GraphicsSubsystem
        {            
            virtual bool Init() override
            {
            }

            virtual bool Deinit() override
            {
            }

            virtual bool RecreateSwapChain() override
            {
            }
            
            virtual bool Render() override
            {
            }

        };
    }
}

#endif // UNICORN_GRAPHICS_OPENGL_GRAPHICS_SUBSYSTEM_HPP