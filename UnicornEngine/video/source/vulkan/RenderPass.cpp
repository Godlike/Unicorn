/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/RenderPass.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
RenderPass::RenderPass()
{
}
RenderPass::~RenderPass()
{
}

vk::Result RenderPass::Create(vk::Device device, vk::Format colorFormat, vk::Format depthFormat)
{
    m_device = device;

    vk::AttachmentDescription attachments[2];
    attachments[0].setFormat(colorFormat);
    attachments[0].setSamples(vk::SampleCountFlagBits::e1);
    attachments[0].setLoadOp(vk::AttachmentLoadOp::eClear);
    attachments[0].setStoreOp(vk::AttachmentStoreOp::eStore);
    attachments[0].setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    attachments[0].setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    attachments[0].setInitialLayout(vk::ImageLayout::eColorAttachmentOptimal);
    attachments[0].setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);
    attachments[0].setFlags(vk::AttachmentDescriptionFlagBits());
    attachments[1].setFormat(depthFormat);
    attachments[1].setSamples(vk::SampleCountFlagBits::e1);
    attachments[1].setLoadOp(vk::AttachmentLoadOp::eClear);
    attachments[1].setStoreOp(vk::AttachmentStoreOp::eDontCare);
    attachments[1].setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    attachments[1].setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    attachments[1].setInitialLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
    attachments[1].setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
    attachments[1].setFlags(vk::AttachmentDescriptionFlagBits());

    vk::AttachmentReference colorRef;
    colorRef.setAttachment(0);
    colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference depthRef;
    depthRef.setAttachment(1);
    depthRef.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::SubpassDescription subpassDesc;
    subpassDesc.setInputAttachmentCount(0);
    subpassDesc.setPInputAttachments(nullptr);
    subpassDesc.setColorAttachmentCount(1);
    subpassDesc.setPColorAttachments(&colorRef);
    subpassDesc.setPResolveAttachments(nullptr);
    subpassDesc.setPDepthStencilAttachment(&depthRef);
    subpassDesc.setPDepthStencilAttachment(nullptr);
    subpassDesc.setPreserveAttachmentCount(0);
    subpassDesc.setPPreserveAttachments(nullptr);

    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.setAttachmentCount(2);
    renderPassInfo.setPAttachments(attachments);
    renderPassInfo.setSubpassCount(1);
    renderPassInfo.setPSubpasses(&subpassDesc);
    renderPassInfo.setDependencyCount(0);
    renderPassInfo.setPDependencies(nullptr);

    vk::Result result = m_device.createRenderPass(&renderPassInfo, nullptr, &m_renderPass);
    return result;
}

void RenderPass::Destroy()
{
    m_device.destroyRenderPass(m_renderPass, nullptr);
}

vk::RenderPass& RenderPass::GetVkRenderPass()
{
    return m_renderPass;
}
}
}
}