#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


#define DEBUG_PRINT_INFO_VK       0x0001
#define DEBUG_PRINT_INFO_DEVICE   0x0002
#define DEBUG_PRINT_INFO_ALL      0x000F

#define DEBUG_VALIDATION_LAYER    0x0010
#define DEBUG_EXTENSION           0x0020

#define DEBUG_EVERYTHING          0xFFFF

#define DEBUG_SHOW_EVERY_RETURN_CODE

#ifdef DEBUG_SHOW_EVERY_RETURN_CODE
	#define VK_RESULT_INFO(result)  (info_vulkan_error_code(result))
#else
	#define VK_RESULT_INFO(result)  (result)
#endif

#define MAX_UINT32_T		std::numeric_limits<uint32_t>::max()

#include <limits>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

struct s_swapchain_details
{
    VkSurfaceCapabilitiesKHR				capabilities;
    std::vector<VkSurfaceFormatKHR>	formats;
    std::vector<VkPresentModeKHR>		present_modes;
};

class Basic_vulk
{
	public:
		Basic_vulk(GLFWwindow *win, uint32_t win_width, uint32_t win_height): 
			Basic_vulk(win, win_width, win_height, 0x0){};
		Basic_vulk(GLFWwindow *win, uint32_t win_width, uint32_t win_height
			, uint32_t debug_mode);
		~Basic_vulk();
		void  init();
		void	draw_frame();
		void	wait_idle(){vkDeviceWaitIdle(m_device);}

	private:
		void  create_instance();
		void	choose_physical_device();
		void	create_logical_device();
		void	create_surface();
		void	create_swapchain();
		void	create_image_views();
		void	create_render_pass();
		void	create_graphics_pipeline();
		void	create_framebuffers();
		void	create_command_pool();
		void	create_semaphores();
		void	allocate_command_buffers();
		void	record_command_buffers();

		VkShaderModule	create_shader_module(std::vector<char> &code);

		GLFWwindow* const	m_win;
		uint32_t					m_win_width;
		uint32_t					m_win_height;

		//STATE VARIABLE
		uint32_t          						m_debug_mode;
		std::vector<const char*>			m_required_device_extensions;

		//VULKAN VARIABLE
		VkInstance        						m_instance;
		VkPhysicalDevice							m_physical_device;
		VkDevice          						m_device;
		VkQueue												m_queue_graphics;
		VkSurfaceKHR									m_surface;
		std::vector<VkImageView>			m_image_views;
		VkPipelineLayout							m_pipeline_layout;
		VkPipeline										m_graphics_pipeline;
		VkRenderPass									m_render_pass;
		std::vector<VkFramebuffer>		m_framebuffers;

		//COMMAND	VARIABLE
		VkCommandPool									m_command_pool;
		std::vector<VkCommandBuffer>	m_command_buffers;
		VkSemaphore										m_semaphore_image_available;
		VkSemaphore										m_semaphore_render_finish;

		//SWAPCHAIN VARIABLE
		VkSwapchainKHR								m_swapchain;
		std::vector<VkImage>					m_swapchain_images;
		VkFormat											m_swapchain_format;
		VkExtent2D										m_swapchain_extent;
};

//VAL_LAYER and EXTENSION
bool	val_layer_check(uint32_t debug, std::vector<const char *> &validation_layers);
std::vector<const char*>  get_extensions(uint32_t debug, uint32_t *ext_count);
bool	check_device_extensions(const VkPhysicalDevice &dev_phy,
	std::vector<const char *> required_ext);

//INFO
void			info_vulkan_api_version();
VkResult	info_vulkan_error_code(VkResult error_code);
void			info_physical_device(const VkPhysicalDevice &phy_dev, int tab = 1);
void			info_device_extension(const VkPhysicalDevice &phy_dev, int tab);
void			info_queue_family_properties(const VkPhysicalDevice &phy_dev, int tab = 1);

//OTHER
uint32_t	queue_family(VkPhysicalDevice device, VkSurfaceKHR surface);
s_swapchain_details	query_swapchain_details(VkPhysicalDevice phy_dev
	, VkSurfaceKHR surface);
std::vector<char>	read_file(const std::string& filename);
