/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/Settings.hpp>

#include <mule/Logger.hpp>

namespace unicorn
{

Settings::Settings()
    : m_width(640)
    , m_height(480)
    , m_applicationName("SAMPLE NAME")
    , m_unicornEngineName("Unicorn Render")
{
}

void Settings::Init(int /*argc*/, char** /*argv*/, const std::string& /*logFileName*/)
{
}

}
