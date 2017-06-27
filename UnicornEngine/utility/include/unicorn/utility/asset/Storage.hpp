/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_ASSET_STORAGE_HPP
#define UNICORN_UTILITY_ASSET_STORAGE_HPP

#include <unicorn/utility/asset/Handler.hpp>
#include <unicorn/utility/concurrent/UnorderedMap.hpp>
#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/utility/templates/Singleton.hpp>

#include <condition_variable>
#include <cstdint>
#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace unicorn
{
namespace utility
{
namespace asset
{
/** @brief  Controls thread safe asset access
 *
 *  Provides both sync and async methods to request assets
 *
 *  @todo   cleaning up idle Handler object
 */
class Storage : public templates::Singleton<Storage>
{
public:
    //! Alias to a key type used by underlying container
    typedef std::string KeyType;

    /** @brief  Initializes worker threads
     *
     *  If upon the calling of this method there already were
     *  some worker threads, they will be stopped and after that
     *  given number of threads will be created
     *
     *  @param  count   number of worker threads
     */
    UNICORN_EXPORT void InitializeWorkers(uint16_t count);

    /** @brief  Synchronously gets an asset identified by @p key
     *
     *  If asset is available, returns it. Otherwise:
     *  - if the asset was not yet requested, prepares and returns it
     *  - if the asset task is in the worker queue, tries to snatch it:
     *  -- if task was snatched, prepares and returns the asset
     *  -- if task was not snatched, waits until worker provides the asset
     *
     *  @param  key asset identifier
     *
     *  @return Handler shared object
     *
     *  @sa ProcessHandlerCreation()
     */
    UNICORN_EXPORT Handler Get(const KeyType& key);

    /** @brief  Asynchronously gets an asset identified by @p key
     *
     *  If asset is avaiable, returns its shared future. Otherwise
     *  - if the asset was not yet requested, prepares asset loading task
     *      and pushes it to the worker queue
     *  - if the asset task is in the worker queue, notifies it of @p priority;
     *      (the biggest of priorities is taken)
     *
     *  @param  key         asset identifier
     *  @param  priority    asset loading priority
     *
     *  @return shared future object on a Handler
     *
     *  @sa ProcessAsyncHandlerCreation
     */
    UNICORN_EXPORT std::shared_future<Handler> GetAsync(const KeyType& key, uint32_t priority = 100);

private:
    friend class utility::templates::Singleton<Storage>;

    //! Alias to underlying container
    typedef concurrent::UnorderedMap<KeyType, std::shared_future<Handler>> ConcurrentHandlerMap;

    //! Alias to a set of requested asset identifiers
    typedef std::set<KeyType> RequestsSet;

    //! Alias to a shared mutex type
    typedef std::shared_timed_mutex SharedMutexType;

    //! Alias to a mutex type
    typedef std::mutex MutexType;

    /** @brief  Describes asset loading task */
    struct Task
    {
        //! Asset identifier
        KeyType key;

        //! Task loading priority
        uint32_t priority;

        //! Promise to create a Handler for the asset
        std::promise<Handler> promise;

        /** @brief  Constructs a default-initialized object */
        Task();

        Task(const Task& other) = delete;
        Task& operator=(const Task& other) = delete;

        /** @brief  Moves a Task from @p other
         *
         *  @param  other   task object
         */
        Task(Task&& other);

        /** @brief  Moves a Task from @p other
         *
         *  @param  other   task object
         *
         *  @return a reference to @c this object
         */
        Task& operator=(Task&& other);

        ~Task() = default;

        /** @brief  Comparator struct for Task ordering */
        struct ComparePriority
        {
            /** @brief  Compare operator
             *
             *  @param  lhs left side of comparison
             *  @param  rhs right side of comparison
             *
             *  @return @c true if priority of @p lhs is higher
             *          than of @p rhs, @c false otherwise
             */
            bool operator()(const Task& lhs, const Task& rhs) const
            {
                return lhs.priority < rhs.priority;
            }
        };
    };

    /** @brief  Describes worker queue */
    class TaskQueue : public std::priority_queue<Task, std::vector<Task>, Task::ComparePriority>
    {
    public:
        /** @brief  Steals a Task for given @p key
         *
         *  Searches for a Task with @p key identifier and removes it
         *  from queue.
         *
         *  @param  key asset identifier
         *
         *  @return stolen Task object or default-initialized one if the Task
         *          was not found in the queue
         */
        Task Snatch(const KeyType& key);

        /** @brief  Removes top element from the queue and returns it
         *
         *  @return popped Task object
         */
        Task PopTop();

        /** @brief  Updates loading priority for a Task
         *
         *  If @p priority is smaller than that of a Task, nothing happens.
         *
         *  @param  key         asset loading
         *  @param  priority    loading priority
         */
        void UpdatePriority(const KeyType& key, uint32_t priority);
    };

    /** @brief  Indicates worker threads to stop and joins them */
    void StopWorkers();

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

    /** @brief  Creates and stores an async request for Handler
     *
     *  Checks if there is an entry for @p key in @ref m_requests :
     *  - if there is none, creates it and also queues a Task for @p key
     *  - if there is a request for @p key, calls UpdatePriority() on it
     *
     *  @param  key         asset identifier
     *  @param  priority    loading priority
     *
     *  @return a future for a Handler
     *
     *  @sa CreateHandler()
     */
    std::shared_future<Handler> ProcessAsyncHandlerCreation(const KeyType& key, uint32_t priority);

    /** @brief  Creates a Handler for given @p key
     *
     *  Prepares asset contents from @p key and creates a Handler object.
     *
     *  @param  key asset identifier
     *
     *  @return created Handler
     */
    Handler CreateHandler(const KeyType& key);

    /** @brief  A routine for a worker thread */
    void WorkerThread();

    /** @brief  Constructs a storage object */
    UNICORN_EXPORT Storage();

    Storage(const Storage& other) = delete;
    Storage& operator=(const Storage& other) = delete;

    Storage(Storage&& other) = delete;
    Storage& operator=(Storage&& other) = delete;

    /** @brief  Destructs a storage object
     *
     *  Also calls StopWorkers() to ensure that worker threads are
     *  destroyed.
     */
    UNICORN_EXPORT ~Storage();

    //! Container of asset handlers
    ConcurrentHandlerMap m_entries;

    //! A set of requests for Handlers
    RequestsSet m_requests;

    //! Protection for @ref m_requests
    SharedMutexType m_requestsMutex;

    //! A vector of worker threads
    std::vector<std::thread> m_workers;

    //! Asset loading task queue
    TaskQueue m_orders;

    //! Protection for @ref m_orders
    MutexType m_ordersMutex;

    /** @brief  Flag indicating if worker threads should stop execution
     *
     * Is guarded by @ref m_ordersMutex
     */
    bool m_workersStopFlag;

    //! Indicates when @p m_orders queue was changed by Storage
    std::condition_variable m_ordersCV;
};
}
}
}

#endif // UNICORN_UTILITY_ASSET_STORAGE_HPP
