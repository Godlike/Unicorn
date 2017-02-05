/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_UTILITY_TEMPLATES_SINGLETON_HPP
#define VORPAL_UTILITY_TEMPLATES_SINGLETON_HPP

namespace vp
{
namespace utility
{
namespace templates
{
/** @brief  Template class describing singleton implementation
 *
 *  To use singleton semantics class @c C shall be a child
 *  of this class. If necessary, the child object shall friend
 *  this class in order to allow the usage of non-public
 *  constructor and destructor.
 *
 *  @tparam C   class that inherits Singleton semantics
 */
template <class C>
class Singleton
{
public:
    /** @brief  Returns the singleton instance
     *
     *  If no instance was stored, creates one
     *
     *  @return reference to singleton instance
     */
    static C& Instance();

    /** @brief  Destroys stored instance */
    static void Destroy();

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    //! Pointer to singleton instance
    static C* s_instance;

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;

    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;
};
}
}
}

#include <vorpal/utility/templates/Singleton.imp>

#endif // VORPAL_UTILITY_TEMPLATES_SINGLETON_HPP
