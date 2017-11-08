/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_SHARED_MACROS_HPP
#define UNICORN_UTILITY_SHARED_MACROS_HPP

/**
 *  @brief CMake will define UnicornRender_EXPORTS when building the library.
 */

#if defined(_WIN32) && defined(UNICORN_SHARED)
#ifdef UnicornRender_EXPORTS
#define UNICORN_EXPORT __declspec(dllexport)
#else
#define UNICORN_EXPORT __declspec(dllimport)
#endif
#else
#define UNICORN_EXPORT
#endif

#endif // UNICORN_UTILITY_SHARED_MACROS_HPP
