/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <vorpal/system/Settings.hpp>

#include <vorpal/utility/Logger.hpp>

namespace vp
{
namespace system
{
Settings::Settings()
    : m_width(640)
    , m_height(480)
    , m_applicationName("SAMPLE NAME")
    , m_vorpalEngineName("Vorpal Engine")
    , m_anisatropicFiltering(16)
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
