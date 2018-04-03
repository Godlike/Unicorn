/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VkMesh.hpp>
#include <unicorn/video/Material.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
VkMesh::VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice,
    vk::CommandPool pool, vk::Queue queue, Mesh& mesh,
    vk::RenderPass& renderPass, vk::PipelineLayout& pipelineLayout,
    vk::Extent2D& swapChainExtent, std::string vertexShader, std::string fragmentShader)
    : m_valid(false)
    , m_device(device)
    , m_physicalDevice(physicalDevice)
    , m_pool(pool)
    , m_queue(queue)
    , m_mesh(mesh)
    , m_renderPass(renderPass)
    , m_pipelineLayout(pipelineLayout)
    , m_swapChainExtent(swapChainExtent)
    , m_vertexShader(vertexShader)
    , m_fragmentShader(fragmentShader)
{
    m_mesh.MaterialUpdated.connect(this, &VkMesh::OnMaterialUpdated);
    m_mesh.VerticesUpdated.connect(this, &VkMesh::AllocateOnGPU);
}

VkMesh::~VkMesh()
{
    DeallocateOnGPU();
    m_mesh.VerticesUpdated.disconnect(this, &VkMesh::AllocateOnGPU);
    m_mesh.MaterialUpdated.disconnect(this, &VkMesh::OnMaterialUpdated);
}

bool VkMesh::operator==(const Mesh& mesh) const
{
    return &mesh == &m_mesh;
}

const glm::mat4& VkMesh::GetModelMatrix() const
{
    return m_mesh.GetModelMatrix();
}

Mesh const& VkMesh::GetMesh() const
{
    return m_mesh;
}

void VkMesh::AllocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
    Buffer stagingBuffer;
    //Vertexes filling
    auto size = sizeof(m_mesh.GetVertices()[0]) * m_mesh.GetVertices().size();
    stagingBuffer.Create(m_physicalDevice, m_device,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent, size);

    m_vertexBuffer.Create(m_physicalDevice, m_device,
        vk::BufferUsageFlagBits::eVertexBuffer |
        vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal, size);

    stagingBuffer.Map();
    stagingBuffer.Write(m_mesh.GetVertices().data());

    stagingBuffer.CopyToBuffer(m_pool, m_queue,
        m_vertexBuffer,
        m_vertexBuffer.GetSize());

    stagingBuffer.Destroy();
    //Indexes filling
    size = sizeof(m_mesh.GetIndices()[0]) * m_mesh.GetIndices().size();

    stagingBuffer.Create(m_physicalDevice, m_device,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent, size);

    stagingBuffer.Map();

    m_indexBuffer.Create(m_physicalDevice, m_device,
        vk::BufferUsageFlagBits::eIndexBuffer |
        vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal, size);

    stagingBuffer.Write(m_mesh.GetIndices().data());
    stagingBuffer.CopyToBuffer(m_pool, m_queue, m_indexBuffer,
        m_indexBuffer.GetSize());
    stagingBuffer.Destroy();

    vk::Result result;
    if(m_pipeline)
    {
        m_device.destroyPipeline(m_pipeline);
        m_pipeline = nullptr;
    }

    m_shaderProgram = new ShaderProgram(m_device,
        m_vertexShader,
        m_fragmentShader);

    if(!m_shaderProgram->IsCreated())
    {
        LOG_VULKAN->Error("Vulkan can't create shader program!");
        //return false;
    }

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

    vk::Viewport viewport;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor;
    scissor.offset.setX(0);
    scissor.offset.setY(0);
    scissor.extent = m_swapChainExtent;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eNone;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.polygonMode = vk::PolygonMode::eFill;

    vk::PipelineMultisampleStateCreateInfo multisampling; // TODO: configure MSAA at global level.
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = VK_TRUE; // TODO:
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = vk::CompareOp::eLessOrEqual;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.back.failOp = vk::StencilOp::eKeep;
    depthStencil.back.passOp = vk::StencilOp::eKeep;
    depthStencil.back.compareOp = vk::CompareOp::eAlways;
    depthStencil.back.compareMask = 0;
    depthStencil.back.reference = 0;
    depthStencil.back.depthFailOp = vk::StencilOp::eKeep;
    depthStencil.back.writeMask = 0;
    depthStencil.front = depthStencil.back;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
                                          vk::ColorComponentFlagBits::eG |
                                          vk::ColorComponentFlagBits::eB |
                                          vk::ColorComponentFlagBits::eA;

    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    auto vertexInputInfo = m_shaderProgram->GetVertexInputInfo();

    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = m_shaderProgram->GetShaderStageInfoData();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1; // Optional

    std::tie(result, m_pipeline) = m_device.createGraphicsPipeline({}, pipelineInfo);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create solid pipeline.");
        //return false;
    }

    m_valid = true;
    ReallocatedOnGpu.emit(this);
}

void VkMesh::DeallocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
}

vk::Buffer VkMesh::GetVertexBuffer() const
{
    return m_vertexBuffer.GetVkBuffer();
}

vk::Buffer VkMesh::GetIndexBuffer() const
{
    return m_indexBuffer.GetVkBuffer();
}

void VkMesh::OnMaterialUpdated()
{
    MaterialUpdated.emit(&m_mesh, this);
}
}
}
}
