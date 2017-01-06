#ifndef VORPAL_UTILITY_LOGGER_HPP
#define VORPAL_UTILITY_LOGGER_HPP

#include <loguru.hpp>

#define LOG_ERROR(...)    LOG_F(ERROR, __VA_ARGS__)
#define LOG_WARNING(...)  LOG_F(WARNING, __VA_ARGS__)
#define LOG_INFO(...)     LOG_F(INFO, __VA_ARGS__)
#define LOG_DEBUG(...)    DLOG_F(INFO, __VA_ARGS__)

#endif // VORPAL_UTILITY_LOGGER_HPP
