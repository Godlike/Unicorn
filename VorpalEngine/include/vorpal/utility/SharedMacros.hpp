/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_SHARED_MACROS_HPP
#define VORPAL_UTILITY_SHARED_MACROS_HPP

/**
 *  @brief CMake will define VorpalEngine_EXPORTS when building the library.
 */

#if defined(_WIN32) && defined(VORPAL_SHARED)
#ifdef VorpalEngine_EXPORTS
#define VORPAL_EXPORT __declspec(dllexport)
#else
#define VORPAL_EXPORT __declspec(dllimport)
#endif
#else
#define VORPAL_EXPORT
#endif

#endif  // VORPAL_UTILITY_SHARED_MACROS_HPP
