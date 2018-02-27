/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_LOGGERS_HPP
#define UNICORN_LOGGERS_HPP

#include <mule/LoggerConfigBase.hpp>
#include <mule/templates/Singleton.hpp>

namespace unicorn
{

class Loggers
    : public mule::LoggerConfigBase
    , public mule::templates::Singleton<Loggers>
{
public:
    using Settings = mule::LoggerConfigBase::Settings;

    std::vector<std::string> GetDefaultLoggerNames() const;

private:
    friend mule::templates::Singleton<Loggers>;

    Loggers();
    ~Loggers() = default;

    virtual void InitializeLogger(uint32_t index, Settings const& settings) override;
};

}


#endif // UNICORN_LOGGERS_HPP
