/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_ASSET_FILE_READER_HPP
#define UNICORN_UTILITY_ASSET_FILE_READER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace vp
{
namespace utility
{
namespace asset
{
/** @brief  Provides lazy access to file content
 *
 *  File contents are loaded only when needed
 */
class FileReader
{
public:
    /** @brief  Bit flags describing the state of FileReader */
    struct Flags
    {
        //! File was not accessed
        static const uint8_t uninitialized = 0b00000000;

        //! File was accessed
        static const uint8_t accessed = 0b00000001;

        //! File was read to the end
        static const uint8_t eof = 0b00000010;

        //! Bit mask describing a good state
        static const uint8_t ok = accessed | eof;

        //! Error encountered while reading file
        static const uint8_t error = 0b00000100;
    };

    /** @brief  Constructs an object
     *
     *  @param  path    path to be used for file access
     */
    FileReader(const char* path);
    ~FileReader() = default;

    /** @brief  Checks if file was successfully read
     *
     *  Reads the file if it was not yet read
     *
     *  @return @c true if @ref m_flags is Flags::ok
     */
    bool IsGood();

    /** @brief  Returns the path used by FileReader */
    const std::string& GetPath() const { return m_path; }

    /** @brief  Returns file contents
     *
     *  Reads the file if it was not yet read
     *
     *  @return const reference to a buffer with file contents
     */
    const std::vector<uint8_t>& GetContent();

    /** @brief  Returns moved file contents
     *
     *  Sets @ref m_flags value to Flags::uninitialized
     *
     *  @return moved buffer with file contents
     */
    std::vector<uint8_t> MoveContent();

private:
    /** @brief  Represents lazy access file reading routine
     *
     *  Checks if @ref m_flags is Flags::uninitialized and loads
     *  file contents if it is.
     */
    void LazyAccess();

    //! Describes FileReader state
    uint8_t m_flags;

    //! Path to the file
    std::string m_path;

    //! Buffer with file contents
    std::vector<uint8_t> m_buffer;
};
}
}
}

#endif // UNICORN_UTILITY_ASSET_FILE_READER_HPP
