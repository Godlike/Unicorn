#ifndef VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP
#define VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP

#include <shared_mutex>
#include <unordered_map>

namespace vp
{
    namespace utility
    {
        namespace concurrent
        {
            /** @brief  Simple mutex-protected std::unordered_map for concurrent access
             *
             *  In future it would make sense to use some high performance concurrent
             *  hash map. For example from the Junction library.
             */
            template<
                class Key,
                class T,
                class Hash = std::hash<Key>,
                class KeyEqual = std::equal_to<Key>,
                class Allocator = std::allocator< std::pair<const Key, T> >
            > class UnorderedMap
            {
            public:
                typedef std::shared_timed_mutex MutexType;
                typedef std::unique_lock<MutexType> UniqueLockType;
                typedef std::unordered_map<Key, T, Hash, KeyEqual, Allocator> underlying_map;

                typedef typename underlying_map::key_type key_type;
                typedef typename underlying_map::mapped_type mapped_type;
                typedef typename underlying_map::value_type value_type;
                typedef typename underlying_map::size_type size_type;
                typedef typename underlying_map::difference_type difference_type;
                typedef typename underlying_map::hasher hasher;
                typedef typename underlying_map::key_equal key_equal;
                typedef typename underlying_map::allocator_type allocator_type;
                typedef typename underlying_map::reference reference;
                typedef typename underlying_map::const_reference const_reference;
                typedef typename underlying_map::pointer pointer;
                typedef typename underlying_map::const_pointer const_pointer;
                typedef typename underlying_map::iterator iterator;
                typedef typename underlying_map::const_iterator const_iterator;

                UnorderedMap() = default;

                UnorderedMap(const UnorderedMap& other) = delete;
                UnorderedMap& operator=(const UnorderedMap& other) = delete;

                UnorderedMap(UnorderedMap&& other) = delete;
                UnorderedMap& operator=(UnorderedMap&& other) = delete;

                ~UnorderedMap() { clear(); }

                UniqueLockType GetUniqueLock() { return UniqueLockType( m_mutex ); }
                underlying_map& GetUnderlyingMap() { return m_data; }

                iterator begin() { return m_data.begin(); }
                const_iterator begin() const { return m_data.begin(); }
                const_iterator cbegin() const { return m_data.cbegin(); }

                iterator end() { return m_data.end(); }
                const_iterator end() const { return m_data.end(); }
                const_iterator cend() const { return m_data.cend(); }

                bool empty() const { SharedLockType lock(m_mutex); return m_data.empty(); }
                size_type size() const { SharedLockType lock(m_mutex); return m_data.size(); }

                std::pair<iterator, bool> insert(const value_type& value) { LockType lock(m_mutex); return m_data.insert(value); }
                template <class P> std::pair<iterator, bool> insert(P&& value) { LockType lock(m_mutex); return m_data.insert(value); }
                iterator insert(const_iterator hint, const value_type& value) { LockType lock(m_mutex); return m_data.insert(hint, value); }
                template <class P> iterator insert(const_iterator hint, P&& value) { LockType lock(m_mutex); return m_data.insert(hint, value); }
                template <class InputIt> void insert(InputIt first, InputIt last) { LockType lock(m_mutex); return m_data.insert(first, last); }
                void insert(std::initializer_list<value_type> ilist) { LockType lock(m_mutex); return m_data.insert(ilist); }

                iterator erase(const_iterator pos) { LockType lock(m_mutex); return m_data.erase(pos); }
                iterator erase(const_iterator first, const_iterator last) { LockType lock(m_mutex); return m_data.erase(first, last); }
                size_type erase(const key_type& key) { LockType lock(m_mutex); return m_data.erase(key); }

                void clear() { LockType lock(m_mutex); m_data.clear(); }

                T& at(const Key& key) { SharedLockType lock(m_mutex); return m_data.at(key); }
                const T& at(const Key& key) const { SharedLockType lock(m_mutex); return m_data.at(key); }

                T& operator[](const Key& key) { LockType lock(m_mutex); return m_data[key]; }
                T& operator[](Key&& key) { LockType lock(m_mutex); return m_data[key]; }

                size_type count(const Key& key) const { SharedLockType lock(m_mutex); return m_data.count(key); }

                iterator find(const Key& key) { SharedLockType lock(m_mutex); return m_data.find(key); }
                const_iterator find(const Key& key) const { SharedLockType lock(m_mutex); return m_data.find(key); }

                std::pair<iterator, bool> contains(const Key& key)
                {
                    SharedLockType lock(m_mutex);
                    iterator result = m_data.find(key);
                    return std::make_pair(result, m_data.end() != result);
                }

                void swap(UnorderedMap& other)
                {
                    UniqueLockType lhs(m_mutex, std::defer_lock);
                    UniqueLockType rhs(other.m_mutex, std::defer_lock);
                    std::lock(lhs, rhs);

                    m_data.swap(other.m_data);
                }

            private:
                typedef std::lock_guard<MutexType> LockType;
                typedef std::shared_lock<MutexType> SharedLockType;

                mutable MutexType m_mutex;
                underlying_map m_data;
            };
        }
    }
}

#endif // VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP
