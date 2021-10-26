/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_layer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 21:28:03 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/26 12:29:24 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

bool  val_layer_check(uint32_t debug, std::vector<const char *> &validation_layers)
{
  uint32_t  layer_count;

  if (VK_RESULT_INFO(vkEnumerateInstanceLayerProperties(&layer_count, nullptr))
    != VK_SUCCESS)
    throw std::runtime_error("failed to get instance layer properties");

  std::vector<VkLayerProperties> available_layers(layer_count);
  if (VK_RESULT_INFO(
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()))
    != VK_SUCCESS)
    throw std::runtime_error("failed to get instance layer properties");

  if (debug & DEBUG_PRINT_INFO_VK)
  {
    std::cout << "available validation layers :" << std::endl;
    for (const auto& layer_properties : available_layers)
      std::cout << '\t' << layer_properties.layerName << std::endl;
		std::cout << std::endl;
  }
  for (const char* layer_name : validation_layers)
  {
    bool layer_found = false;

    for (const auto& layer_properties : available_layers)
    {
      if (strcmp(layer_name, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }
    if (!layer_found)
      return false;
  }
  return true;
}

std::vector<const char*>  get_extensions(uint32_t debug, uint32_t *ext_count)
{
	const char  **glfw_ext;
	uint32_t    glfw_ext_count;
	std::vector <const char*> exts;
  uint32_t    extension_count = 0;

  if (VK_RESULT_INFO(
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr))
    != VK_SUCCESS)
    throw std::runtime_error("failed to get instance layer properties");
  std::vector<VkExtensionProperties> extensions(extension_count);
  if (VK_RESULT_INFO(
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()))
    != VK_SUCCESS)
    throw std::runtime_error("failed to get instance layer properties");

	glfw_ext = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
	*ext_count = glfw_ext_count;
  if (debug & DEBUG_PRINT_INFO_VK)
  {
    std::cout << "available extensions:" << std::endl;
    for (const auto& extension : extensions)
      std::cout << '\t' << extension.extensionName << std::endl;
		std::cout << std::endl;
  }
  for(int i = 0; i < glfw_ext_count; ++i)
    exts.push_back(glfw_ext[i]);
	exts.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	*ext_count += 1;
	if (debug & DEBUG_EXTENSION)
	{
    exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		*ext_count += 1;
	}
	return (exts);
}
