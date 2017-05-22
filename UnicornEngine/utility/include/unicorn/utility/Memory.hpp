/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_MEMORY_HPP
#define UNICORN_UTILITY_MEMORY_HPP

namespace unicorn
{
namespace utility
{
void* AlignedAlloc(size_t size, size_t alignment);
void AlignedFree(void* data);
}
}

#endif // UNICORN_UTILITY_MEMORY_HPP
