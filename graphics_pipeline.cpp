/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_pipeline.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 09:56:39 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/22 17:32:26 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"

static VkPipelineMultisampleStateCreateInfo	multisampling_create(void)
{
	VkPipelineMultisampleStateCreateInfo	multisampling_info;

	multisampling_info = (VkPipelineMultisampleStateCreateInfo){};
	multisampling_info.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling_info.sampleShadingEnable = VK_FALSE;
	multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling_info.minSampleShading = 1.0f;
	multisampling_info.pSampleMask = nullptr;
	multisampling_info.alphaToCoverageEnable = VK_FALSE;
	multisampling_info.alphaToOneEnable = VK_FALSE;
	return (multisampling_info);
}

static VkPipelineRasterizationStateCreateInfo	rasterizer_create(void)
{
	VkPipelineRasterizationStateCreateInfo	rasterizer_info;

	rasterizer_info = (VkPipelineRasterizationStateCreateInfo){};
	rasterizer_info.sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_info.depthClampEnable = VK_FALSE;
	rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
	rasterizer_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_info.lineWidth = 1.0f;
	rasterizer_info.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer_info.depthBiasEnable = VK_FALSE;
	rasterizer_info.depthBiasConstantFactor = 0.0f;
	rasterizer_info.depthBiasClamp = 0.0f;
	rasterizer_info.depthBiasSlopeFactor = 0.0f;
	return (rasterizer_info);
}

static VkPipelineViewportStateCreateInfo	viewport_create(VkExtent2D extent)
{
	VkPipelineViewportStateCreateInfo		viewport_info;
	static VkViewport								viewport;
	static VkRect2D								scissor;

	viewport_info = (VkPipelineViewportStateCreateInfo){};
	viewport = (VkViewport){};
	scissor = (VkRect2D){};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = extent.width;
	viewport.height = extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	scissor.offset = (VkOffset2D){0, 0};
	scissor.extent = extent;
	viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_info.viewportCount = 1;
	viewport_info.pViewports = &viewport;
	viewport_info.scissorCount = 1;
	viewport_info.pScissors = &scissor;
	return (viewport_info);
}

static VkPipelineColorBlendStateCreateInfo	color_blend_create(
		VkPipelineColorBlendAttachmentState &color_blend)
{
	VkPipelineColorBlendStateCreateInfo		color_blend_info;

	color_blend = (VkPipelineColorBlendAttachmentState){};
	color_blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
															| VK_COLOR_COMPONENT_G_BIT 
															| VK_COLOR_COMPONENT_B_BIT
															| VK_COLOR_COMPONENT_A_BIT;
	color_blend.blendEnable = VK_FALSE;
	color_blend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	color_blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
	color_blend_info = (VkPipelineColorBlendStateCreateInfo){};
	color_blend_info.sType =
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_info.logicOpEnable = VK_FALSE;
	color_blend_info.attachmentCount = 1;
	color_blend_info.pAttachments = &color_blend;
	return (color_blend_info);
}
	
static VkPipelineInputAssemblyStateCreateInfo	input_assembly_create(void)
{
	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType
		= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	//input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;
	return (input_assembly);
}

static VkPipelineVertexInputStateCreateInfo	vert_input_create(void)
{
/*
	VkPipelineVertexInputStateCreateInfo	vert_input_info;
	static auto binding_description = s_vertex::get_binding_description();
	static auto attribute_descriptions = s_vertex::get_attribute_descriptions();

	vert_input_info = (VkPipelineVertexInputStateCreateInfo){};
	vert_input_info.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vert_input_info.vertexBindingDescriptionCount = 1;
	vert_input_info.pVertexBindingDescriptions = &binding_description;
	vert_input_info.vertexAttributeDescriptionCount =
		static_cast<uint32_t>(attribute_descriptions.size());
	vert_input_info.pVertexAttributeDescriptions =
		attribute_descriptions.data();
*/
	VkPipelineVertexInputStateCreateInfo	vertex_input_info{};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.vertexBindingDescriptionCount = 0;
	vertex_input_info.pVertexBindingDescriptions = nullptr;
	vertex_input_info.vertexAttributeDescriptionCount = 0;
	vertex_input_info.pVertexAttributeDescriptions = nullptr;
	return (vertex_input_info);
}

void	Basic_vulk::create_graphics_pipeline()
{
	auto	vert_shader_code = read_file("shaders/vert.spv");
	auto	frag_shader_code = read_file("shaders/frag.spv");
	VkShaderModule	vert_shader_module = create_shader_module(vert_shader_code);
	VkShaderModule	frag_shader_module = create_shader_module(frag_shader_code);

	VkPipelineShaderStageCreateInfo	vert_shader_stage_info{};
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_shader_stage_info.module = vert_shader_module;
	vert_shader_stage_info.pName = "main";
	VkPipelineShaderStageCreateInfo	frag_shader_stage_info{};
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.module = frag_shader_module;
	frag_shader_stage_info.pName = "main";
	VkPipelineShaderStageCreateInfo shader_stages[] =
		{vert_shader_stage_info, frag_shader_stage_info};

	VkPipelineColorBlendAttachmentState			color_blend;
	VkPipelineRasterizationStateCreateInfo	rasterizer_info = rasterizer_create();
	VkPipelineViewportStateCreateInfo				viewport_info =
			viewport_create(m_swapchain_extent);
	VkPipelineInputAssemblyStateCreateInfo	input_assembly = input_assembly_create();
	VkPipelineColorBlendStateCreateInfo			color_blend_info =
			color_blend_create(color_blend);
	VkPipelineMultisampleStateCreateInfo		multisampling_info =
			multisampling_create();
	VkPipelineVertexInputStateCreateInfo		vert_input_info = vert_input_create();

	VkPipelineLayoutCreateInfo pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 0; // Optional
	pipeline_layout_info.pSetLayouts = nullptr; // Optional
	pipeline_layout_info.pushConstantRangeCount = 0; // Optional
	pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

	if (VK_RESULT_INFO(vkCreatePipelineLayout(m_device, &pipeline_layout_info
			, nullptr, &m_pipeline_layout)) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");
	
	VkGraphicsPipelineCreateInfo	pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vert_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_info;
	pipeline_info.pRasterizationState = &rasterizer_info;
	pipeline_info.pMultisampleState = &multisampling_info;
	pipeline_info.pDepthStencilState = nullptr;
	pipeline_info.pColorBlendState = &color_blend_info;
	pipeline_info.pDynamicState = nullptr;
	pipeline_info.layout = m_pipeline_layout;
	pipeline_info.renderPass = m_render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;
	if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1
			, &pipeline_info, nullptr, &m_graphics_pipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");

	vkDestroyShaderModule(m_device, vert_shader_module, nullptr);
	vkDestroyShaderModule(m_device, frag_shader_module, nullptr);
}
