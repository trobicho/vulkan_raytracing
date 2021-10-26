/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swapchain.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 09:44:51 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/22 17:12:37 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"

s_swapchain_details		query_swapchain_details(VkPhysicalDevice phy_dev
		, VkSurfaceKHR surface)
{
	s_swapchain_details	details;
	uint32_t	formats_count;
	uint32_t	present_mode_count;

	if (VK_RESULT_INFO(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_dev, surface
			, &details.capabilities)) != VK_SUCCESS)
		throw std::runtime_error("failed to query device surface capabilities!");
	if (VK_RESULT_INFO(vkGetPhysicalDeviceSurfaceFormatsKHR(
			phy_dev, surface, &formats_count, nullptr))!= VK_SUCCESS)
		throw std::runtime_error("failed to query device surface formats!");
	if (formats_count)
	{
		details.formats.resize(formats_count);
		if (VK_RESULT_INFO(vkGetPhysicalDeviceSurfaceFormatsKHR(
				phy_dev, surface, &formats_count, details.formats.data()))!= VK_SUCCESS)
			throw std::runtime_error("failed to query device surface formats!");
	}
	if (VK_RESULT_INFO(vkGetPhysicalDeviceSurfacePresentModesKHR(
			phy_dev, surface, &present_mode_count, nullptr))!= VK_SUCCESS)
		throw std::runtime_error("failed to query device surface present modes!");
	if (present_mode_count)
	{
		details.present_modes.resize(present_mode_count);
		if (VK_RESULT_INFO(vkGetPhysicalDeviceSurfacePresentModesKHR(phy_dev, surface
				, &present_mode_count, details.present_modes.data()))!= VK_SUCCESS)
			throw std::runtime_error("failed to query device surface present modes!");
	}
	return(details);
}

static VkSurfaceFormatKHR			choose_format(
    std::vector<VkSurfaceFormatKHR>	&formats)
{
	if (formats.size() == 0 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return ((VkSurfaceFormatKHR){VK_FORMAT_B8G8R8A8_UNORM
				, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
	}
	for (const auto& format : formats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return format;
	}
	return formats[0];
}

static VkPresentModeKHR			choose_present_mode(
	std::vector<VkPresentModeKHR> &present_modes)
{
	for (const auto& present_mode : present_modes)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			return (present_mode);
	}
	return (VK_PRESENT_MODE_FIFO_KHR);
}

static VkExtent2D		choose_extent(uint32_t win_width, uint32_t win_height,
	const VkSurfaceCapabilitiesKHR capabilities)
{
	VkExtent2D	actual_ext;

	if (capabilities.currentExtent.width != UINT32_MAX)
		return (capabilities.currentExtent);
	actual_ext = (VkExtent2D){win_width, win_height};
	if (capabilities.maxImageExtent.width < actual_ext.width)
		actual_ext.width = capabilities.maxImageExtent.width;
	if (capabilities.minImageExtent.width > actual_ext.width)
		actual_ext.width = capabilities.minImageExtent.width;
	if (capabilities.maxImageExtent.height < actual_ext.height)
		actual_ext.height = capabilities.maxImageExtent.height;
	if (capabilities.minImageExtent.height > actual_ext.height)
		actual_ext.height = capabilities.minImageExtent.height;
	return (actual_ext);
}

void	Basic_vulk::create_swapchain()
{
	s_swapchain_details	details
		= query_swapchain_details(m_physical_device, m_surface);

	VkSurfaceFormatKHR	surface_format = choose_format(details.formats);
	VkPresentModeKHR		present_mode = choose_present_mode(details.present_modes);
	VkExtent2D					extent = choose_extent(m_win_width, m_win_height
												, details.capabilities);

	uint32_t image_count = details.capabilities.minImageCount + 1;
	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = m_surface;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	create_info.preTransform = details.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;
	if (VK_RESULT_INFO(vkCreateSwapchainKHR(m_device, &create_info
			, nullptr, &m_swapchain)) != VK_SUCCESS)
  	throw std::runtime_error("failed to create swap chain!");

	if (VK_RESULT_INFO(vkGetSwapchainImagesKHR(m_device, m_swapchain
			, &image_count, nullptr)) != VK_SUCCESS)
  	throw std::runtime_error("failed to retrive swap chain images!");
	m_swapchain_images.resize(image_count);
	if (VK_RESULT_INFO(vkGetSwapchainImagesKHR(m_device, m_swapchain
			, &image_count, m_swapchain_images.data())) != VK_SUCCESS)
  	throw std::runtime_error("failed to retrive swap chain images!");
	m_swapchain_format = surface_format.format;
	m_swapchain_extent = extent;
}
