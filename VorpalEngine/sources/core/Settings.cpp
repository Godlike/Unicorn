/*
* Copyright (C) 2017 by 73s studio
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <vorpal/core/Settings.hpp>

#include <vorpal/utility/Logger.hpp>

namespace vp
{
namespace core
{

Settings::Settings()
    : m_width(640)
    , m_height(480)
    , m_applicationName("SAMPLE NAME")
    , m_vorpalEngineName("Vorpal Engine")
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
