#ifndef VORPAL_VORPAL_ENGINE_HPP
#define VORPAL_VORPAL_ENGINE_HPP

#include <vorpal/utility.hpp>

struct GLFWwindow;

namespace vp
{
    class VORPAL_EXPORT VorpalEngine
    {
    public:
        VorpalEngine();
        ~VorpalEngine();

        VorpalEngine(const VorpalEngine& other) = delete;
        VorpalEngine(const VorpalEngine&& other) = delete;
        VorpalEngine& operator=(const VorpalEngine& other) = delete;
        VorpalEngine& operator=(const VorpalEngine&& other) = delete;

        void init();
        void run();

    private:
        GLFWwindow* m_window;
    };
}

#endif /* VORPAL_VORPAL_ENGINE_HPP */
