/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP
#define VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP

#include <vorpal/utility/asset/Handler.hpp>
#include <vorpal/utility/SharedMacros.hpp>
#include <vorpal/utility/templates/Singleton.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>

namespace vp
{
namespace utility
{
namespace asset
{
/** @brief  Controls non-thread safe asset access
 *
 *  Provides synchronous method to request assets
 *
 *  @todo   cleaning up idle Handler object
 */
class SimpleStorage : public templates::Singleton<SimpleStorage>
{
public:
    //! Alias to a key type used by underlying container
    typedef std::string KeyType;

    /** @brief  Gets an asset identified by @p key
     *
     *  If the asset is not available, prepares and returns it
     *
     *  @param  key asset identifier
     *
     *  @return Handler shared object
     *
     *  @sa ProcessHandlerCreation()
     */
    VORPAL_EXPORT Handler Get(const KeyType& key);

private:
    friend class utility::templates::Singleton<SimpleStorage>;

    //! Alias to underlying container
    typedef std::unordered_map<KeyType, Handler> HandlerMap;

    /** @brief  Creates and stores a Handler
     *
     *  Creates handler via CreateHandler() call and inserts it
     *  to @ref m_entries
     *
     *  @param  key asset identifier
     *
     *  @return created Handler
     *
     *  @sa CreateHandler()
     */
    Handler ProcessHandlerCreation(const KeyType& key);

    /** @brief  Creates a Handler for given @p key
     *
     *  Prepares asset contents from @p key and creates a Handler object.
     *
     *  @param  key asset identifier
     *
     *  @return created Handler
     */
    Handler CreateHandler(const KeyType& key);

    SimpleStorage() = default;

    SimpleStorage(const SimpleStorage& other) = delete;
    SimpleStorage& operator=(const SimpleStorage& other) = delete;

    SimpleStorage(SimpleStorage&& other) = delete;
    SimpleStorage& operator=(SimpleStorage&& other) = delete;

    ~SimpleStorage() = default;

    //! Container of asset handlers
    HandlerMap m_entries;
};
}
}
}

#endif // VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP
