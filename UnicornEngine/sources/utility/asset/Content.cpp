/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/asset/Content.hpp>

uc::utility::asset::Content::Content(std::vector<uint8_t>&& buffer)
    : m_buffer(buffer)
{
}
