/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 16:28:16 by trobicho          #+#    #+#             */
/*   Updated: 2021/10/22 16:38:08 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_vulk.hpp"
#include <iostream>

static void	main_loop(Basic_vulk &vulk, GLFWwindow *win)
{
	while(!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		vulk.draw_frame();
	}
	vulk.wait_idle();
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow *win = glfwCreateWindow(800, 800, "Vulkan", NULL, NULL);
	Basic_vulk  vulk(win, 800, 800,
			DEBUG_PRINT_INFO_ALL | DEBUG_VALIDATION_LAYER | DEBUG_EXTENSION);

	vulk.init();
	main_loop(vulk, win);

	return (0);
}
