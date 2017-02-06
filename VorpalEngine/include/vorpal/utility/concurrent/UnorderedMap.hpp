/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP
#define VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP

#include <mutex>
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
template <
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>>
class UnorderedMap
{
public:
    //! Alias to mutex type
    typedef std::shared_timed_mutex MutexType;

    //! Alias to unique lock type
    typedef std::unique_lock<MutexType> UniqueLockType;

    //! Alias to underlying container
    typedef std::unordered_map<Key, T, Hash, KeyEqual, Allocator> underlying_map;

    //! Alias to underlying container's key type
    typedef typename underlying_map::key_type key_type;

    //! Alias to underlying container's mapped type
    typedef typename underlying_map::mapped_type mapped_type;

    //! Alias to underlying container's value type
    typedef typename underlying_map::value_type value_type;

    //! Alias to underlying container's size type
    typedef typename underlying_map::size_type size_type;

    //! Alias to underlying container's difference type
    typedef typename underlying_map::difference_type difference_type;

    //! Alias to underlying container's hasher
    typedef typename underlying_map::hasher hasher;

    //! Alias to underlying container's key equal
    typedef typename underlying_map::key_equal key_equal;

    //! Alias to underlying container's allocator type
    typedef typename underlying_map::allocator_type allocator_type;

    //! Alias to underlying container's reference
    typedef typename underlying_map::reference reference;

    //! Alias to underlying container's const reference
    typedef typename underlying_map::const_reference const_reference;

    //! Alias to underlying container's pointer
    typedef typename underlying_map::pointer pointer;

    //! Alias to underlying container's const pointer
    typedef typename underlying_map::const_pointer const_pointer;

    //! Alias to underlying container's iterator
    typedef typename underlying_map::iterator iterator;

    //! Alias to underlying container's const iterator
    typedef typename underlying_map::const_iterator const_iterator;

    UnorderedMap() = default;

    UnorderedMap(const UnorderedMap& other) = delete;
    UnorderedMap& operator=(const UnorderedMap& other) = delete;

    UnorderedMap(UnorderedMap&& other) = delete;
    UnorderedMap& operator=(UnorderedMap&& other) = delete;

    /** @brief  Destroys an object clearing the contents */
    ~UnorderedMap() { clear(); }

    /** @brief  Returns a unique lock for contents
     *
     *  @return lock to contents
     */
    UniqueLockType GetUniqueLock() { return UniqueLockType(m_mutex); }

    /** @brief  Returns a reference to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return reference to underlying container
     *
     *  @sa GetUniqueLock()
     */
    underlying_map& GetUnderlyingMap() { return m_data; }

    /** @brief  Returns a begin iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    iterator begin() { return m_data.begin(); }

    /** @brief  Returns a const begin iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return const iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    const_iterator begin() const { return m_data.begin(); }

    /** @brief  Returns a const begin iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return const iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    const_iterator cbegin() const { return m_data.cbegin(); }

    /** @brief  Returns a end iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    iterator end() { return m_data.end(); }

    /** @brief  Returns a const end iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return const iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    const_iterator end() const { return m_data.end(); }

    /** @brief  Returns a const end iterator to underlying container
     *
     *  @attention  Shall be used only when holding a lock
     *
     *  @return const iterator to underlying container
     *
     *  @sa GetUniqueLock()
     */
    const_iterator cend() const { return m_data.cend(); }

    /** @brief  Checks if underlying container is empty
     *
     *  @return @c true if empty, @c false otherwise
     */
    bool empty() const
    {
        SharedLockType lock(m_mutex);
        return m_data.empty();
    }

    /** @brief  Returns the number of objects in underlying container
     *
     *  @return number of objects
     */
    size_type size() const
    {
        SharedLockType lock(m_mutex);
        return m_data.size();
    }

    /** @brief  Inserts value
     *
     *  @param  value   value to be inserted
     *
     *  @return iterator to the inserted element (or to the element that
     *          prevented insertion) and a flag describing if insertion
     *          took place
     */
    std::pair<iterator, bool> insert(const value_type& value)
    {
        LockType lock(m_mutex);
        return m_data.insert(value);
    }

    /** @brief  Inserts value
     *
     *  @param  value   value to be inserted
     *
     *  @return iterator to the inserted element (or to the element that
     *          prevented insertion) and a flag describing if insertion
     *          took place
     */
    template <class P>
    std::pair<iterator, bool> insert(P&& value)
    {
        LockType lock(m_mutex);
        return m_data.insert(value);
    }

    /** @brief  Inserts value
     *
     *  @param  hint    suggestion to where the search should start
     *  @param  value   value to be inserted
     *
     *  @return iterator to the inserted element (or to the element that
     *          prevented insertion) and a flag describing if insertion
     *          took place
     */
    iterator insert(const_iterator hint, const value_type& value)
    {
        LockType lock(m_mutex);
        return m_data.insert(hint, value);
    }

    /** @brief  Inserts value
     *
     *  @param  hint    suggestion to where the search should start
     *  @param  value   value to be inserted
     *
     *  @return iterator to the inserted element (or to the element that
     *          prevented insertion) and a flag describing if insertion
     *          took place
     */
    template <class P>
    iterator insert(const_iterator hint, P&& value)
    {
        LockType lock(m_mutex);
        return m_data.insert(hint, value);
    }

    /** @brief  Inserts elements from range [first; last)
     *
     *  @param  first   iterator to the first element to be inserted
     *  @param  last    end-iterator for insertion (not included in the range)
     */
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        LockType lock(m_mutex);
        return m_data.insert(first, last);
    }

    /** @brief  Inserts elements from initializer list
     *
     *  @param  ilist   initializer list
     */
    void insert(std::initializer_list<value_type> ilist)
    {
        LockType lock(m_mutex);
        return m_data.insert(ilist);
    }

    /** @brief  Removes an element
     *
     *  @param  pos const iterator to the element to be removed
     *
     *  @return iterator following the last removed element
     */
    iterator erase(const_iterator pos)
    {
        LockType lock(m_mutex);
        return m_data.erase(pos);
    }

    /** @brief  Removes an elements from range [first; last)
     *
     *  @param  first   iterator to the first element to be removed
     *  @param  last    end-iterator for removal (not included in range)
     *
     *  @return iterator following the last removed element
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        LockType lock(m_mutex);
        return m_data.erase(first, last);
    }

    /** @brief  Removes elements identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return number of removed elements
     */
    size_type erase(const key_type& key)
    {
        LockType lock(m_mutex);
        return m_data.erase(key);
    }

    /** @brief  Clears underlying container */
    void clear()
    {
        LockType lock(m_mutex);
        m_data.clear();
    }

    /** @brief  Boundary-safe access of container
     *
     *  If no element identified by @p key is found, exception is thrown.
     *
     *  @param  key element identifier
     *
     *  @return reference to element
     */
    T& at(const Key& key)
    {
        SharedLockType lock(m_mutex);
        return m_data.at(key);
    }

    /** @brief  Boundary-safe access of container
     *
     *  If no element identified by @p key is found, exception is thrown.
     *
     *  @param  key element identifier
     *
     *  @return const reference to element
     */
    const T& at(const Key& key) const
    {
        SharedLockType lock(m_mutex);
        return m_data.at(key);
    }

    /** @brief  Container access
     *
     *  Creates an element if none were identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return reference to element
     */
    T& operator[](const Key& key)
    {
        LockType lock(m_mutex);
        return m_data[key];
    }

    /** @brief  Container access
     *
     *  Creates an element if none were identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return reference to element
     */
    T& operator[](Key&& key)
    {
        LockType lock(m_mutex);
        return m_data[key];
    }

    /** @brief  Returns the number of elements identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return number of elements identified by @p key
     */
    size_type count(const Key& key) const
    {
        SharedLockType lock(m_mutex);
        return m_data.count(key);
    }

    /** @brief  Searches for an element identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return iterator to found element or an end-iterator if none were found
     */
    iterator find(const Key& key)
    {
        SharedLockType lock(m_mutex);
        return m_data.find(key);
    }

    /** @brief  Searches for an element identified by @p key
     *
     *  @param  key element identifier
     *
     *  @return iterator to found element or an end-iterator if none were found
     */
    const_iterator find(const Key& key) const
    {
        SharedLockType lock(m_mutex);
        return m_data.find(key);
    }

    /** @brief  Checks if underlying container has an element identified by @p key
     *
     *  Basically performs find() and compares it with end() while holding a mutex once
     *
     *  @param  key element identifier
     *
     *  @return pair of iterator to element and a flag if it was found or not
     *
     *  @sa find(), end()
     */
    std::pair<iterator, bool> contains(const Key& key)
    {
        SharedLockType lock(m_mutex);
        iterator result = m_data.find(key);
        return std::make_pair(result, m_data.end() != result);
    }

    /** @brief  Swaps the contents of maps
     *
     *  @param  other   map to swap the contents with
     */
    void swap(UnorderedMap& other)
    {
        UniqueLockType lhs(m_mutex, std::defer_lock);
        UniqueLockType rhs(other.m_mutex, std::defer_lock);
        std::lock(lhs, rhs);

        m_data.swap(other.m_data);
    }

private:
    //! Alias to the regular lock type
    typedef std::lock_guard<MutexType> LockType;

    //! Alias to the shared lock type
    typedef std::shared_lock<MutexType> SharedLockType;

    //! Underlying container
    underlying_map m_data;

    //! Protection for @ref m_data
    mutable MutexType m_mutex;
};
}
}
}

#endif // VORPAL_UTILITY_CONCURRENT_UNORDERED_MAP_HPP
