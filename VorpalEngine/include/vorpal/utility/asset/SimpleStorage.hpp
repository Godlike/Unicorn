#ifndef VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP
#define VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP

#include <vorpal/utility/asset/Handler.hpp>
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
                typedef std::string KeyType;

                /** @brief  Gets an asset identified by @p key
                 *
                 *  If the asset is not available, prepares and returns it
                 *
                 *  @param  key asset identifier
                 *
                 *  @return Handler shared object
                 */
                Handler Get(const KeyType& key);

            private:
                friend class utility::templates::Singleton<SimpleStorage>;

                typedef std::unordered_map<KeyType, Handler> HandlerMap;

                Handler ProcessHandlerCreation(const KeyType& key);

                Handler CreateHandler(const KeyType& key);

                SimpleStorage() = default;

                SimpleStorage(const SimpleStorage& other) = delete;
                SimpleStorage& operator=(const SimpleStorage& other) = delete;

                SimpleStorage(SimpleStorage&& other) = delete;
                SimpleStorage& operator=(SimpleStorage&& other) = delete;

                ~SimpleStorage() = default;

                HandlerMap m_entries;
            };
        }
    }
}

#endif // VORPAL_UTILITY_ASSET_SIMPLE_STORAGE_HPP
