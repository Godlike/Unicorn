/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/GammaRamp.hpp>

namespace unicorn
{
namespace system
{

GammaRamp::GammaRamp()
    : size( 0 )
    , red( nullptr )
    , green( nullptr )
    , blue( nullptr )
{

}

GammaRamp::GammaRamp(const GammaRamp& other)
    : size( other.size )
{
    bool greenUsesRed = (other.green == other.red);
    bool blueUsesRed = (other.blue == other.red);
    bool blueUsesGreen = (other.blue == other.green);

    red = new uint16_t[size];
    green = !greenUsesRed ? new uint16_t[size] : red;
    blue = (!blueUsesRed && !blueUsesGreen) ? new uint16_t[size] : (blueUsesRed ? red : green);

    for (uint32_t i = 0; i < size; ++i)
    {
        red[i] = other.red[i];
        green[i] = other.green[i];
        blue[i] = other.blue[i];
    }
}

GammaRamp& GammaRamp::operator=(const GammaRamp& other)
{
    if (blue)
    {
        delete[] blue;
    }

    if (green != blue)
    {
        delete[] green;
    }

    if (red != blue && red != green)
    {
        delete[] red;
    }

    size = other.size;

    bool greenUsesRed = (other.green == other.red);
    bool blueUsesRed = (other.blue == other.red);
    bool blueUsesGreen = (other.blue == other.green);

    red = new uint16_t[size];
    green = !greenUsesRed ? new uint16_t[size] : red;
    blue = (!blueUsesRed && !blueUsesGreen) ? new uint16_t[size] : (blueUsesRed ? red : green);

    for (uint32_t i = 0; i < size; ++i)
    {
        red[i] = other.red[i];
        green[i] = other.green[i];
        blue[i] = other.blue[i];
    }

    return *this;
}

GammaRamp::GammaRamp(GammaRamp&& other)
    : size( other.size )
    , red( other.red )
    , green( other.green )
    , blue( other.blue )
{
    other.red = nullptr;
    other.green = nullptr;
    other.blue = nullptr;
}

GammaRamp& GammaRamp::operator=(GammaRamp&& other)
{
    size = other.size;
    red = other.red;
    green = other.green;
    blue = other.blue;

    other.red = nullptr;
    other.green = nullptr;
    other.blue = nullptr;

    return *this;
}

GammaRamp::~GammaRamp()
{
    if (blue)
    {
        delete[] blue;
    }

    if (green != blue)
    {
        delete[] green;
    }

    if (red != blue && red != green)
    {
        delete[] red;
    }
}

}
}
