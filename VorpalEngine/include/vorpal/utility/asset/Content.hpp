/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_ASSET_CONTENT_HPP
#define VORPAL_UTILITY_ASSET_CONTENT_HPP

#include <cstdint>
#include <vector>

namespace vp
{
namespace utility
{
namespace asset
{
/** @brief  Represents asset content access
 *
 *  Holds binary buffer filled with asset content
 */
class Content
{
public:
    /** @brief  Constructs an object
     *
     *  @param  buffer  buffer with asset contents
     */
    Content(std::vector<uint8_t>&& buffer);

    Content(const Content& other) = delete;
    Content& operator=(const Content& other) = delete;

    Content(Content&& other) = delete;
    Content& operator=(Content&& other) = delete;

    ~Content() = default;

    /** @brief  Returns stored buffer */
    const std::vector<uint8_t>& GetBuffer() const { return m_buffer; }

    /** @brief  Returns the size of stored buffer */
    uintmax_t GetSize() const { return m_buffer.size(); }

private:
    //! Buffer with asset contents
    std::vector<uint8_t> m_buffer;
};
}
}
}

#endif // VORPAL_UTILITY_ASSET_CONTENT_HPP
