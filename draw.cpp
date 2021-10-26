/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:17:39 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/22 16:43:09 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"

void	Basic_vulk::draw_frame()
{
	uint32_t	image_index;

	vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX
		, m_semaphore_image_available, VK_NULL_HANDLE, &image_index);

	VkSubmitInfo	submit_info{};
	VkSemaphore	wait_semaphores[] = {m_semaphore_image_available};
	VkPipelineStageFlags	wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_command_buffers[image_index];
	VkSemaphore	signal_semaphores[] = {m_semaphore_render_finish};
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphores;
	if (VK_RESULT_INFO(vkQueueSubmit(m_queue_graphics, 1
			, &submit_info, VK_NULL_HANDLE)) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkPresentInfoKHR present_info{};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;
	VkSwapchainKHR swapchains[] = {m_swapchain};
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapchains;
	present_info.pImageIndices = &image_index;
	present_info.pResults = nullptr; //TODO: check result
	vkQueuePresentKHR(m_queue_graphics, &present_info);
	vkQueueWaitIdle(m_queue_graphics);
}
