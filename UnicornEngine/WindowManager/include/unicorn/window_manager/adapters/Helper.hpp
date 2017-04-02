/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_ADAPTERS_HELPER_HPP
#define UNICORN_WINDOW_MANAGER_ADAPTERS_HELPER_HPP

#define __EXEC_STRINGIFY(x) #x
#define __STRINGIFY(x)      __EXEC_STRINGIFY(x)
#define __SUBSTITUTE(x)     x

#define WINDOW_MANAGER_ADAPTER_CONFIG __STRINGIFY(__SUBSTITUTE(unicorn/window_manager/adapters/UNICORN_WINDOW_MANAGER)/config.hpp)

#include WINDOW_MANAGER_ADAPTER_CONFIG

#ifndef MONITOR_HANDLE
    #define MONITOR_HANDLE void*
#endif

#ifndef WINDOW_HANDLE
    #define WINDOW_HANDLE void*
#endif

#endif /* UNICORN_WINDOW_MANAGER_ADAPTERS_HELPER_HPP */
