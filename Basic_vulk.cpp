/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basic_vulk.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 16:37:28 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/26 12:30:54 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"
#include <iostream>

Basic_vulk::Basic_vulk(GLFWwindow *win, uint32_t win_width, uint32_t win_height
	, uint32_t debug_mode = 0) : m_win(win)
		, m_win_width(win_width)
		, m_win_height(win_height)
		, m_debug_mode(debug_mode)
{
	
	const std::vector<const char*>	device_exts =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_MAINTENANCE_3_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
		VK_KHR_RAY_QUERY_EXTENSION_NAME,
	};
	for (const auto &ext : device_exts)
		m_required_device_extensions.push_back(ext);
}

Basic_vulk::~Basic_vulk()
{
	vkDestroySemaphore(m_device, m_semaphore_render_finish, nullptr);
	vkDestroySemaphore(m_device, m_semaphore_image_available, nullptr);
	vkDestroyCommandPool(m_device, m_command_pool, nullptr);
	for (auto framebuffer : m_framebuffers)
		vkDestroyFramebuffer(m_device, framebuffer, nullptr);
	vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
	vkDestroyRenderPass(m_device, m_render_pass, nullptr);
	for (auto image_view : m_image_views)
		vkDestroyImageView(m_device, image_view, nullptr);
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	vkDestroyDevice(m_device, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyInstance(m_instance, nullptr);
	glfwDestroyWindow(m_win);
	glfwTerminate();
}

void  Basic_vulk::init()
{
	create_instance();
	create_surface();
	choose_physical_device();
	create_logical_device();
	create_swapchain();
	create_image_views();
	create_render_pass();
	create_graphics_pipeline();
	create_framebuffers();
	create_command_pool();
	allocate_command_buffers();
	record_command_buffers();
	create_semaphores();
}

void  Basic_vulk::create_instance()
{
	VkApplicationInfo app_info = {};
	std::vector<const char*>  val_layers = {"VK_LAYER_KHRONOS_validation"};
	std::vector<const char*>  ext;
	uint32_t  ext_count;

	if (m_debug_mode & DEBUG_PRINT_INFO_VK)
		info_vulkan_api_version();
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Vulkan Basic";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	ext = get_extensions(m_debug_mode, &ext_count);
	if (ext.size() < ext_count)
		throw std::runtime_error("failed to get extensions!");

	create_info.enabledExtensionCount = ext_count;
	create_info.ppEnabledExtensionNames = ext.data();
	create_info.enabledLayerCount = 0;
	if (m_debug_mode & DEBUG_VALIDATION_LAYER)
	{
		if (!val_layer_check(m_debug_mode, val_layers))
			throw std::runtime_error("validation layers requested, but not available!");
		create_info.enabledLayerCount = val_layers.size();
		create_info.ppEnabledLayerNames = val_layers.data();
	}

	if (VK_RESULT_INFO(vkCreateInstance(&create_info, NULL, &m_instance))
			!= VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}

void	Basic_vulk::create_logical_device()
{
	VkDeviceCreateInfo				device_create_info = {};
	VkDeviceQueueCreateInfo		queue_create_info = {};
	VkPhysicalDeviceFeatures	device_features = {};

	uint32_t	queue_family_index = queue_family(m_physical_device, m_surface);
	if (queue_family_index == MAX_UINT32_T)
		throw std::runtime_error("failed to find valid queue family!");
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = queue_family_index;
	queue_create_info.queueCount = 1;
	float	queue_priority = 1.0f;
	queue_create_info.pQueuePriorities = &queue_priority;

	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pQueueCreateInfos = &queue_create_info;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.enabledLayerCount = 0; //Deprecated
	device_create_info.pEnabledFeatures = &device_features;
	device_create_info.enabledExtensionCount = m_required_device_extensions.size();
	device_create_info.ppEnabledExtensionNames = m_required_device_extensions.data();
	if (VK_RESULT_INFO(vkCreateDevice(m_physical_device
			, &device_create_info, NULL, &m_device)) != VK_SUCCESS)
		throw std::runtime_error("failed to create device!");
	vkGetDeviceQueue(m_device, queue_family_index, 0, &m_queue_graphics);
}

void	Basic_vulk::create_surface()
{
	if (VK_RESULT_INFO(glfwCreateWindowSurface(m_instance, m_win
			, nullptr, &m_surface))!= VK_SUCCESS)
    throw std::runtime_error("failed to create window surface!");
}

void	Basic_vulk::create_image_views()
{
	m_image_views.resize(m_swapchain_images.size());
	for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	{
		VkImageViewCreateInfo	create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image = m_swapchain_images[i];
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = m_swapchain_format;
		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;
		if (VK_RESULT_INFO(vkCreateImageView(m_device, &create_info, nullptr
				, &m_image_views[i])) != VK_SUCCESS)
			throw std::runtime_error("failed to create image views");
	}
}

void	Basic_vulk::create_render_pass()
{
	VkAttachmentDescription color_attachment{};
	color_attachment.format = m_swapchain_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkAttachmentReference color_attachment_ref{};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	if (VK_RESULT_INFO(vkCreateRenderPass(m_device, &render_pass_info
			, nullptr, &m_render_pass)) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");
}

void	Basic_vulk::create_framebuffers()
{
	m_framebuffers.resize(m_image_views.size());
	if (m_debug_mode & DEBUG_PRINT_INFO_VK)
		std::cout << "Framebuffers size: " << m_framebuffers.size() << std::endl;
	for (size_t i = 0; i < m_image_views.size(); ++i)
	{
		VkImageView	attachments[] = {m_image_views[i]};
		VkFramebufferCreateInfo	framebuffer_info{};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = m_render_pass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = attachments;
		framebuffer_info.width = m_swapchain_extent.width;
		framebuffer_info.height = m_swapchain_extent.height;
		framebuffer_info.layers = 1;

		if (VK_RESULT_INFO(vkCreateFramebuffer(m_device, &framebuffer_info
				, nullptr, &m_framebuffers[i])) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer!");
	}
}
