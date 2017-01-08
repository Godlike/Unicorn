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
             *  To use singleton semantics the object shall be a child
             *  of this class. If necessary the child object shall friend
             *  this class in order to allow usage of non-public
             *  constructor and destructor.
             */
            template <class C> class Singleton
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

#include <vorpal/utility/templates/Singleton.imp>

#endif // VORPAL_UTILITY_TEMPLATES_SINGLETON_HPP
