/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 03:10:35 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/21 15:00:32 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"
#include <iostream>

uint32_t	queue_family(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t	queue_count;
	VkBool32	present_support;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_count, NULL);
	std::vector<VkQueueFamilyProperties>	queue(queue_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_count, queue.data());
	for (uint32_t i = 0; i < queue_count; ++i)
	{
		present_support = 0;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
		if (queue[i].queueCount > 0 && queue[i].queueFlags
			& VK_QUEUE_GRAPHICS_BIT && present_support)
			return (i);
	}
	return (MAX_UINT32_T);
}
