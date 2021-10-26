/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:15:26 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/22 17:31:17 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"
#include <iostream>

void	Basic_vulk::create_command_pool()
{
	auto	queue_family_index = queue_family(m_physical_device, m_surface);

	VkCommandPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queue_family_index;
	pool_info.flags = 0;
	if (VK_RESULT_INFO(vkCreateCommandPool(m_device, &pool_info
			, nullptr, &m_command_pool)) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");
}

void	Basic_vulk::allocate_command_buffers()
{
	m_command_buffers.resize(m_framebuffers.size());
	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = m_command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = (uint32_t)m_command_buffers.size();
	if (VK_RESULT_INFO(vkAllocateCommandBuffers(m_device, &alloc_info
			, m_command_buffers.data())) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers");
}

void	Basic_vulk::record_command_buffers()
{
	for (size_t i = 0; i < m_command_buffers.size(); i++)
	{
		VkCommandBufferBeginInfo	begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = 0;
		begin_info.pInheritanceInfo = nullptr;
		if (VK_RESULT_INFO(vkBeginCommandBuffer(m_command_buffers[i]
				, &begin_info)) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");
		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = m_render_pass;
		render_pass_info.framebuffer = m_framebuffers[i];
		render_pass_info.renderArea.offset = {0, 0};
		render_pass_info.renderArea.extent = m_swapchain_extent;
		VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = &clear_color;
		vkCmdBeginRenderPass(m_command_buffers[i], &render_pass_info
				, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_command_buffers[i]
				, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
		vkCmdDraw(m_command_buffers[i], 4, 1, 0, 0);
		vkCmdEndRenderPass(m_command_buffers[i]);
		if (VK_RESULT_INFO(vkEndCommandBuffer(m_command_buffers[i])) !=VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}
}

void	Basic_vulk::create_semaphores()
{
	VkSemaphoreCreateInfo semaphore_info{};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (VK_RESULT_INFO(vkCreateSemaphore(m_device, &semaphore_info
				, nullptr, &m_semaphore_image_available)) != VK_SUCCESS
			| VK_RESULT_INFO(vkCreateSemaphore(m_device, &semaphore_info
				, nullptr, &m_semaphore_render_finish)) != VK_SUCCESS)
		throw std::runtime_error("failed to create semaphore!");
}
