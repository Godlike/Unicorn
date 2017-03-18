/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/asset/SimpleStorage.hpp>

#include <unicorn/utility/asset/Content.hpp>
#include <unicorn/utility/asset/FileReader.hpp>

#include <unicorn/utility/Logger.hpp>

namespace vp
{
namespace utility
{
namespace asset
{
Handler SimpleStorage::Get(const KeyType& key)
{
    HandlerMap::const_iterator it = m_entries.find(key);

    if (m_entries.cend() != it)
    {
        LOG_DEBUG("Requested asset \"%s\" -> exists", key.c_str());

        return it->second;
    }
    else
    {
        LOG_DEBUG("Requested asset \"%s\" -> process", key.c_str());

        return ProcessHandlerCreation(key);
    }
}

Handler SimpleStorage::ProcessHandlerCreation(const KeyType& key)
{
    LOG_DEBUG("Processing asset \"%s\" -> creating handler", key.c_str());

    Handler handler = CreateHandler(key);

    LOG_DEBUG("Processing asset \"%s\" -> publishing entry", key.c_str());

    m_entries.insert(std::make_pair(key, handler));

    return handler;
}

Handler SimpleStorage::CreateHandler(const KeyType& key)
{
    Content* content = nullptr;

    LOG_DEBUG("Reading file \"%s\"", key.c_str());

    FileReader file(key.c_str());

    if (file.IsGood())
    {
        content = new Content(file.MoveContent());
    }
    else
    {
        LOG_WARNING("Failed to read file \"%s\"", key.c_str());
    }

    return Handler(key, content);
}
}
}
}
