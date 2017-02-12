/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_TEMPLATES_SINGLETON_HPP
#define UNICORN_UTILITY_TEMPLATES_SINGLETON_HPP

namespace uc
{
namespace utility
{
namespace templates
{
/** @brief  Template class describing singleton implementation
 *
 *  To use singleton semantics the object shall be a child
 *  of this class. If necessary the child object shall friend
 *  this class in order to allow usage of non-public
 *  constructor and destructor.
 */
template <class C>
class Singleton
{
public:
    static C& Instance();
    static void Destroy();

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    static C* s_instance;

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;

    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;
};
}
}
}

#include <unicorn/utility/templates/Singleton.imp>

#endif // UNICORN_UTILITY_TEMPLATES_SINGLETON_HPP
