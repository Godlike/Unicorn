/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/GraphicsPipeline.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            GraphicsPipeline::GraphicsPipeline()
            {
            }

            GraphicsPipeline::~GraphicsPipeline()
            {
            }

            vk::Result GraphicsPipeline::Create(
                vk::Device device,
                vk::ShaderModule vertexShader,
                vk::ShaderModule fragmentShader,
                vk::DescriptorSetLayout setLayout,
                const std::vector<vk::VertexInputBindingDescription>& vertexBindings,
                const std::vector<vk::VertexInputAttributeDescription>& vertexAttributes,
                vk::Viewport viewport,
                vk::Rect2D scissor,
                vk::RenderPass renderPass)
            {
                m_device = device;

                vk::PipelineCacheCreateInfo cacheInfo;
                cacheInfo.setFlags(vk::PipelineCacheCreateFlagBits());
                cacheInfo.setInitialDataSize(0);
                cacheInfo.setPInitialData(nullptr);

                vk::Result result = m_device.createPipelineCache(
                    &cacheInfo,
                    nullptr,
                    &m_pipelineCache);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                vk::PipelineShaderStageCreateInfo stageInfos[2];
                stageInfos[0].setFlags(vk::PipelineShaderStageCreateFlagBits());
                stageInfos[0].setStage(vk::ShaderStageFlagBits::eVertex);
                stageInfos[0].setModule(vertexShader);
                stageInfos[0].setPName("main");
                stageInfos[0].setPSpecializationInfo(nullptr);

                stageInfos[1].setFlags(vk::PipelineShaderStageCreateFlagBits());
                stageInfos[1].setStage(vk::ShaderStageFlagBits::eFragment);
                stageInfos[1].setModule(fragmentShader);
                stageInfos[1].setPName("main");
                stageInfos[1].setPSpecializationInfo(nullptr);

                vk::PipelineLayoutCreateInfo layoutInfo;
                layoutInfo.setPushConstantRangeCount(0);
                layoutInfo.setPPushConstantRanges(nullptr);
                layoutInfo.setLayoutCount = 1;
                layoutInfo.setPSetLayouts(&setLayout);

                result = m_device.createPipelineLayout(
                    &layoutInfo,
                    nullptr,
                    &m_pipelineLayout);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
                vertexInputInfo.setVertexBindingDescriptionCount(vertexBindings.size());
                vertexInputInfo.setPVertexBindingDescriptions(vertexBindings.data());
                vertexInputInfo.setVertexAttributeDescriptionCount(vertexAttributes.size());
                vertexInputInfo.setPVertexAttributeDescriptions(vertexAttributes.data());

                vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
                inputAssemblyInfo.setTopology(vk::PrimitiveTopology::eTriangleList);
                inputAssemblyInfo.setPrimitiveRestartEnable(false);

                vk::PipelineViewportStateCreateInfo viewportInfo;
                viewportInfo.setViewportCount(1);
                viewportInfo.setPViewports(&viewport);
                viewportInfo.setScissorCount(1);
                viewportInfo.setPScissors(&scissor);

                vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
                rasterizationInfo.setDepthBiasClamp(false);
                rasterizationInfo.setRasterizerDiscardEnable(false);
                rasterizationInfo.setPolygonMode(vk::PolygonMode::eFill);
                rasterizationInfo.setCullMode(vk::CullModeFlagBits::eNone);
                rasterizationInfo.setFrontFace(vk::FrontFace::eCounterClockwise);
                rasterizationInfo.setDepthBiasEnable(false);
                rasterizationInfo.setDepthBiasConstantFactor(0.0f);
                rasterizationInfo.setDepthBiasClamp(0.0f);
                rasterizationInfo.setDepthBiasSlopeFactor(0.0f);
                rasterizationInfo.setLineWidth(1.0f);

                vk::PipelineMultisampleStateCreateInfo multisampleInfo;
                multisampleInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);
                multisampleInfo.setSampleShadingEnable(false);
                multisampleInfo.setMinSampleShading(0.0f);
                multisampleInfo.setPSampleMask(nullptr);
                multisampleInfo.setAlphaToCoverageEnable(false);
                multisampleInfo.setAlphaToOneEnable(false);

                vk::StencilOpState stencilState;
                stencilState.setFailOp(vk::StencilOp::eKeep);
                stencilState.setPassOp(vk::StencilOp::eKeep);
                stencilState.setDepthFailOp(vk::StencilOp::eKeep);
                stencilState.setCompareOp(vk::CompareOp::eNever);
                stencilState.setCompareMask(0);
                stencilState.setWriteMask(0);
                stencilState.setReference(0);

                vk::PipelineDepthStencilStateCreateInfo depthInfo;
                depthInfo.setDepthTestEnable(true); //debug
                depthInfo.setDepthWriteEnable(true);
                depthInfo.setDepthCompareOp(vk::CompareOp::eLessOrEqual);
                depthInfo.setDepthBoundsTestEnable(false);
                depthInfo.setStencilTestEnable(false);
                depthInfo.setFront(stencilState);
                depthInfo.setBack(stencilState);
                depthInfo.setMinDepthBounds(0.0f);
                depthInfo.setMaxDepthBounds(0.0f);

                //init PipelineColorBlendAttachmentState
                vk::PipelineColorBlendAttachmentState blendState;
                blendState.setBlendEnable(false);
                blendState.setSrcColorBlendFactor(vk::BlendFactor::eZero);
                blendState.setDstColorBlendFactor(vk::BlendFactor::eZero);
                blendState.setColorBlendOp(vk::BlendOp::eAdd);
                blendState.setSrcAlphaBlendFactor(vk::BlendFactor::eZero);
                blendState.setDstAlphaBlendFactor(vk::BlendFactor::eZero);
                blendState.setAlphaBlendOp(vk::BlendOp::eAdd);
                blendState.setColorWriteMask(
                    vk::ColorComponentFlagBits::eR |
                    vk::ColorComponentFlagBits::eG |
                    vk::ColorComponentFlagBits::eB |
                    vk::ColorComponentFlagBits::eA);

                vk::PipelineColorBlendStateCreateInfo blendInfo;
                blendInfo.setLogicOpEnable(false);
                blendInfo.setLogicOp(vk::LogicOp::eClear);
                blendInfo.setAttachmentCount(1);
                blendInfo.setPAttachments(&blendState);
                blendInfo.blendConstants[0] = 0.0f;
                blendInfo.blendConstants[1] = 0.0f;
                blendInfo.blendConstants[2] = 0.0f;
                blendInfo.blendConstants[3] = 0.0f;

                vk::GraphicsPipelineCreateInfo pipelineInfo;
                pipelineInfo.setStageCount(2);
                pipelineInfo.setPStages(stageInfos);
                pipelineInfo.setPVertexInputState(&vertexInputInfo);
                pipelineInfo.setPInputAssemblyState(&inputAssemblyInfo);
                pipelineInfo.setPTessellationState(nullptr);
                pipelineInfo.setPViewportState(&viewportInfo);
                pipelineInfo.setPRasterizationState(&rasterizationInfo);
                pipelineInfo.setPMultisampleState(&multisampleInfo);
                pipelineInfo.setPDepthStencilState(&depthInfo);
                pipelineInfo.setPColorBlendState(&blendInfo);
                pipelineInfo.setPDynamicState(nullptr);
                pipelineInfo.setLayout(m_pipelineLayout);
                pipelineInfo.setRenderPass(renderPass);
                pipelineInfo.setSubpass(0);
                pipelineInfo.setBasePipelineHandle(vk::Pipeline());
                pipelineInfo.setBasePipelineIndex(0);

                result = m_device.createGraphicsPipelines(
                    m_pipelineCache,
                    1,
                    &pipelineInfo,
                    nullptr,
                    &m_pipeline);
                return result;
            }

            void GraphicsPipeline::Destroy()
            {
                m_device.destroyPipeline(m_pipeline, nullptr);
                m_device.destroyPipelineLayout(m_pipelineLayout, nullptr);
                m_device.destroyPipelineCache(m_pipelineCache, nullptr);
            }

            vk::PipelineLayout& GraphicsPipeline::GetVkPipelineLayout()
            {
                return m_pipelineLayout;
            }

            vk::Pipeline& GraphicsPipeline::GetVkPipeline()
            {
                return m_pipeline;
            }
        }
    }
}