/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/asset/FileReader.hpp>

#include <algorithm>
#include <fstream>

uc::utility::asset::FileReader::FileReader(const char* path)
    : m_flags(Flags::uninitialized)
    , m_path(path)
{
}

bool uc::utility::asset::FileReader::IsGood()
{
    LazyAccess();

    return Flags::ok == m_flags;
}

const std::vector<uint8_t>& uc::utility::asset::FileReader::GetContent()
{
    LazyAccess();

    return m_buffer;
}

std::vector<uint8_t> uc::utility::asset::FileReader::MoveContent()
{
    LazyAccess();

    m_flags = Flags::uninitialized;

    return std::move(m_buffer);
}

void uc::utility::asset::FileReader::LazyAccess()
{
    if (Flags::uninitialized == m_flags)
    {
        std::ifstream in(m_path, std::ios::in | std::ios::binary);

        if (in)
        {
            m_flags = Flags::accessed;

            //! This would work for file sizes up to size_t
            in.seekg(0, std::ios_base::end);
            size_t size = in.tellg();
            in.seekg(0, std::ios_base::beg);

            m_buffer.clear();
            m_buffer.resize(size);

            //! Safe to use reinterpret since char and unsigned char occupy same amount of storage
            if (in.read(reinterpret_cast<char*>(m_buffer.data()), size))
            {
                m_flags |= Flags::eof;
            }
        }
        else
        {
            m_flags = Flags::error;
            m_buffer.clear();
        }
    }
}
