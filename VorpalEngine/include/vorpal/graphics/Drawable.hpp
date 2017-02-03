/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_DRAWABLE_HPP
#define VORPAL_GRAPHICS_DRAWABLE_HPP

namespace vp
{
namespace graphics
{
class Drawable
{
	virtual Drawable();
    virtual void Draw() = 0;
};
}
}

#endif // VORPAL_GRAPHICS_DRAWABLE_HPP
