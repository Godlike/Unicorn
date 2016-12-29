// 73s studio

#ifndef VORPAL_RENDERER_HPP
#define VORPAL_RENDERER_HPP

class Renderer;

namespace vp
{
    namespace video
    {
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

            void init();
            void deinit();
            void update();
        private:
            Renderer* _pRenderer;
        };
    }
}

#endif // VORPAL_RENDERER_HPP
