/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_GRAPHICS_PIPELINE_HPP
#define UNICORN_VIDEO_VULKAN_GRAPHICS_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class GraphicsPipeline
            {
            public:
                GraphicsPipeline();
                ~GraphicsPipeline();
                Result create(
                    vk::Device device,
                    vk::ShaderModule vertexShader,
                    vk::ShaderModule fragmentShader,
                    vk::DescriptorSetLayout setLayout,
                    const std::vector<vk::VertexInputBindingDescription>& vertexBindings,
                    const std::vector<vk::VertexInputAttributeDescription>& vertexAttributes,
                    vk::Viewport viewport,
                    vk::Rect2D scissor,
                    vk::RenderPass renderPass);
                void destroy();

                vk::PipelineLayout& getVkPipelineLayout();
                vk::Pipeline& getVkPipeline();
            private:
                vk::Device m_device;
                vk::PipelineCache m_pipelineCache;
                vk::PipelineLayout m_pipelineLayout;
                vk::Pipeline m_pipeline;
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_GRAPHICS_PIPELINE_HPP