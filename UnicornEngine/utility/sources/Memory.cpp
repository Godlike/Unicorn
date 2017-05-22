/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Memory.hpp>
#include <stdlib.h>

namespace unicorn
{
namespace utility
{
void* AlignedAlloc(size_t size, size_t alignment)
{
    void* data = nullptr;
    #if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
    #else
    int res = posix_memalign(&data, alignment, size);
    if ( res != 0 )
    {
        data = nullptr;
    }
    #endif
    return data;
}

void AlignedFree(void* data)
{
    #if	defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(data);
    #else
    free(data);
    #endif
}
}
}
