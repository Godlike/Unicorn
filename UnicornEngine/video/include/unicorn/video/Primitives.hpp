/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PRIMITIVES_HPP
#define UNICORN_VIDEO_PRIMITIVES_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Mesh.hpp>

namespace unicorn
{
namespace video
{

class Triangle : public Mesh
{
        
};

class Cube : public Mesh
{
    Cube();
};

class Quad : public Mesh
{

};

}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
