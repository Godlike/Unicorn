#ifndef VORPAL_GRAPHICS_HPP
#define VORPAL_GRAPHICS_HPP

namespace vp
{
    namespace video
    {
    class Renderer;
        /**
         * @brief The Graphics class abstract renderer.
         */
        class Graphics {
        public:
            Graphics();
            Graphics(const Graphics& other) = delete;
            Graphics(const Graphics&& other) = delete;
            Graphics& operator=(const Renderer& other) = delete;
            Graphics& operator=(const Graphics&& other) = delete;

            bool init();
            void deinit();
            void render();
        private:
            Renderer* m_pRenderer;
        };
    }
}

#endif // VORPAL_GRAPHICS_HPP
