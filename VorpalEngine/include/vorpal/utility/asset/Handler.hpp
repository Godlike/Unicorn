#ifndef VORPAL_UTILITY_ASSET_HANDLER_HPP
#define VORPAL_UTILITY_ASSET_HANDLER_HPP

#include <vorpal/utility/asset/Content.hpp>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>

namespace vp
{
    namespace utility
    {
        namespace asset
        {
            /** @brief  Provide access to shared asset content
             *
             *  Holds track of how long current object is the last reference to shared data
             */
            class Handler
            {
            public:
                /** @brief  Constructs an empty handler
                 *
                 *  Created object does not have any data associated with it
                 */
                Handler();

                /** @brief  Constructs a shared data object around @p pContent
                 *
                 *  Initializes all required shared data
                 */
                Handler(const std::string& name, Content* pContent);

                /** @brief  Constructs a shared object from @p other
                 *
                 *  Inherits and subscribes to shared data of @p other
                 */
                Handler(const Handler& other);

                /** @brief  Switches to shared object from @p other
                 *
                 *  Unsubscribes from current shared data and then inherits
                 *  and subscribes to shared data of @p other
                 */
                Handler& operator=(const Handler& other);

                /** @brief  Moves shared object from @p other
                 *
                 *  Inherits and subscribes to shared data of @p other, after that
                 *  @p other is unsubscribed and reset
                 */
                Handler(Handler&& other);

                /** @brief  Moves shared object from @p other
                 *
                 *  Unsubscribes from current shared data, then inherits and
                 *  subscribes to shared data of @p other, after that @p other
                 *  is unsubscribed and reset
                 */
                Handler& operator=(Handler&& other);

                /** @brief  Destructor
                 *
                 *  Unsubscribes from current shared data
                 */
                ~Handler();

                /** @brief  Compares two Handler objects
                 *
                 *  @return @c true if @p other points to the same shared data,
                 *          @c false otherwise
                 */
                bool operator==(const Handler& other) const;

                /** @brief  Compares two Handler objects
                 *
                 *  @return @c false if @p other points to the same shared data,
                 *          @c true otherwise
                 *
                 *  @sa operator==()
                 */
                bool operator!=(const Handler& other) const;

                /** @brief  Checks if shared data may be accessed */
                bool IsValid() const { return nullptr != m_pContent;}

                const std::string& GetName() const { return *m_pName; }
                const Content& GetContent() const { return *m_pContent; }

                /** @brief  Returns the duration since this object is the last refernce
                 *
                 *  @return duration since this object is the last reference (in ms),
                 *          @c 0ms is returned if this object is not the last reference
                 */
                std::chrono::milliseconds GetLastReferenceDuration() const;

            private:
                void Unsubscribe();
                void Subscribe();

                std::string* m_pName;
                Content* m_pContent;
                std::atomic<uint32_t>* m_pRefCounter;
                std::atomic<uint64_t>* m_pLastReferenceTimestamp;
            };
        }
    }
}

#endif // VORPAL_UTILITY_ASSET_HANDLER_HPP
