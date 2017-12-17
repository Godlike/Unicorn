/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_MEMORY_HPP
#define UNICORN_UTILITY_MEMORY_HPP

#include <stdlib.h>

namespace unicorn
{
namespace utility
{
/* @brief Allocates memory on a specified alignment boundary
 * @param[in] size size of the requested memory allocation
 * @param[in] alignment the alignment value, which must be an integer power of 2
 * @return a pointer to the memory block that was allocated or NULL if the operation failed, the pointer is a multiple of alignment
 */
void* AlignedAlloc(size_t size, size_t alignment);

/* @brief Frees a block of memory that was align allocated
*  @param[in] data a pointer to the memory block
*/
void AlignedFree(void* data);
}
}

#endif // UNICORN_UTILITY_MEMORY_HPP
