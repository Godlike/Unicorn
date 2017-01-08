#include <vorpal/utility/asset/FileReader.hpp>

#include <algorithm>
#include <fstream>

namespace vp
{
namespace utility
{
namespace asset
{

FileReader::FileReader(const char* path)
    : m_flags(Flags::uninitialized)
    , m_path(path)
{

}

bool FileReader::IsGood()
{
    LazyAccess();

    return Flags::ok == m_flags;
}

const std::vector<uint8_t>& FileReader::GetContent()
{
    LazyAccess();

    return m_buffer;
}

std::vector<uint8_t> FileReader::MoveContent()
{
    LazyAccess();

    m_flags = Flags::uninitialized;

    return std::move(m_buffer);
}

void FileReader::LazyAccess()
{
    if (Flags::uninitialized == m_flags)
    {
        std::ifstream in(m_path, std::ios::in | std::ios::binary);

        if (in)
        {
            m_flags = Flags::accessed;

            //! This would work for file sizes up to size_t
            in.seekg(0, std::ios_base::end);
            size_t size = static_cast<size_t>(std::min<std::ifstream::pos_type>(in.tellg(), SIZE_MAX));
            in.seekg(0, std::ios_base::beg);

            m_buffer.clear();
            m_buffer.reserve(size);

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

}
}
}
