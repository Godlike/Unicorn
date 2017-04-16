/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Settings.hpp>

#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace core
{
Settings::Settings()
    : m_width(640)
    , m_height(480)
    , m_applicationName("SAMPLE NAME")
    , m_unicornEngineName("Unicorn Engine")
{
}

void Settings::Init(int argc, char** argv, const std::string& logFileName)
{
    loguru::init(argc, argv);

    if (!logFileName.empty())
    {
        loguru::add_file(logFileName.c_str(), loguru::Append, loguru::Verbosity_INFO);
    }
}
}
}
