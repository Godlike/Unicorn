/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_ASSET_FILE_READER_HPP
#define VORPAL_UTILITY_ASSET_FILE_READER_HPP

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
                struct Flags
                {
                    static const uint8_t uninitialized   = 0b00000000;

                    static const uint8_t accessed        = 0b00000001;
                    static const uint8_t eof             = 0b00000010;
                    static const uint8_t ok              = accessed | eof;

                    static const uint8_t error           = 0b00000100;
                };

                FileReader(const char* path);
                ~FileReader() = default;

                /** @brief  Checks if file was successfully read */
                bool IsGood();
                const std::string& GetPath() const { return m_path; }
                const std::vector<uint8_t>& GetContent();

                std::vector<uint8_t> MoveContent();

            private:
                void LazyAccess();

                uint8_t m_flags;
                std::string m_path;
                std::vector<uint8_t> m_buffer;
            };
        }
    }
}

#endif // VORPAL_UTILITY_ASSET_FILE_READER_HPP
