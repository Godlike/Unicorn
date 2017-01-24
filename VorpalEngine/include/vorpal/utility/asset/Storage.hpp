/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_ASSET_STORAGE_HPP
#define VORPAL_UTILITY_ASSET_STORAGE_HPP

#include <vorpal/utility/asset/Handler.hpp>
#include <vorpal/utility/concurrent/UnorderedMap.hpp>
#include <vorpal/utility/SharedMacros.hpp>
#include <vorpal/utility/templates/Singleton.hpp>

#include <condition_variable>
#include <cstdint>
#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace vp
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
    typedef std::string KeyType;

    /** @brief  Initializes worker threads
     *
     *  If upon the calling of this method there already were
     *  some worker threads, they will be stopped and after that
     *  given number of threads will be created
     *
     *  @param  count   number of worker threads
     */
    VORPAL_EXPORT void InitializeWorkers(uint16_t count);

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
     */
    VORPAL_EXPORT Handler Get(const KeyType& key);

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
     */
    VORPAL_EXPORT std::shared_future<Handler> GetAsync(
        const KeyType& key, uint32_t priority = 100);

   private:
    friend class utility::templates::Singleton<Storage>;

    typedef concurrent::UnorderedMap<KeyType, std::shared_future<Handler>>
        ConcurrentHandlerMap;
    typedef std::set<KeyType> RequestsSet;
    typedef std::shared_timed_mutex SharedMutexType;
    typedef std::mutex MutexType;

    /** @brief  Describes asset loading task */
    struct Task
    {
        KeyType key;
        uint32_t priority;
        std::promise<Handler> promise;

        Task();

        Task(const Task& other) = delete;
        Task& operator=(const Task& other) = delete;

        Task(Task&& other);
        Task& operator=(Task&& other);

        ~Task() = default;

        struct ComparePriority
        {
            bool operator()(const Task& lhs, const Task& rhs) const
            {
                return lhs.priority < rhs.priority;
            }
        };
    };

    /** @brief  Describes worker queue */
    class TaskQueue : public std::priority_queue<Task,
                          std::vector<Task>,
                          Task::ComparePriority>
    {
       public:
        Task Snatch(const KeyType& key);
        Task PopTop();
        void UpdatePriority(const KeyType& key, uint32_t priority);
    };

    void StopWorkers();

    Handler ProcessHandlerCreation(const KeyType& key);
    std::shared_future<Handler> ProcessAsyncHandlerCreation(
        const KeyType& key, uint32_t priority);

    Handler CreateHandler(const KeyType& key);

    void WorkerThread();

    VORPAL_EXPORT Storage();

    Storage(const Storage& other) = delete;
    Storage& operator=(const Storage& other) = delete;

    Storage(Storage&& other) = delete;
    Storage& operator=(Storage&& other) = delete;

    VORPAL_EXPORT ~Storage();

    ConcurrentHandlerMap m_entries;

    RequestsSet m_requests;
    SharedMutexType m_requestsMutex;

    std::vector<std::thread> m_workers;

    TaskQueue m_orders;
    MutexType m_ordersMutex;
    bool m_workersStopFlag;  //! is guarded by m_ordersMutex
    std::condition_variable m_ordersCV;
};
}
}
}

#endif  // VORPAL_UTILITY_ASSET_STORAGE_HPP
