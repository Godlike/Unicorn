/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Settings.hpp>

namespace unicorn
{
namespace utility
{

Settings::Settings()
    : m_width(640)
    , m_height(480)
    , m_applicationName("SAMPLE NAME")
    , m_unicornEngineName("Unicorn Render")
    , m_profilingMask(Settings::ProfilingMask::None)
{
}

}
}
