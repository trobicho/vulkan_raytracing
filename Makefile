# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/16 22:25:09 by trobicho          #+#    #+#              #
#    Updated: 2021/10/22 17:39:09 by trobicho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++
GLS = glslangValidator
CXXFLAGS	=	-std=c++14 -g

NAME = new_vulkan

INCLUDE_PATH = /home/tom/projects/lib
SRCS_PATH	=	./
HDRS_PATH	=	./
OBJS_PATH	=	./obj
SHADERS_PATH	=	./shaders

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	INCS_FLAGS	= -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include \
		-I $(INCLUDE_PATH)
	LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan -lm
else
	LDFLAGS = -L$(GLFW3_PATH)/lib -L$(VULKAN_SDK)/lib `pkg-config --static --libs glm` -lvulkan -lglfw -lm
endif

SHADERS_NAME =	fractal.frag \
								shader.vert \

SHADERS_RESULT_NAME =	frag.spv \
											vert.spv \

SRCS_NAME =	main.cpp \
						Basic_vulk.cpp \
						validation_layer.cpp \
						device_phy.cpp \
						queue.cpp \
						swapchain.cpp \
						graphics_pipeline.cpp \
						pipeline_utils.cpp \
						command.cpp \
						info.cpp \
						draw.cpp

HDRS_NAME =	Basic_vulk.hpp \

OBJS_NAME	=	$(SRCS_NAME:.cpp=.o) $(SRCS_UI_NAME:.cpp=.o) 

SRCS = $(addprefix $(SRCS_PATH)/, $(SRCS_NAME))
HDRS = $(addprefix $(HDRS_PATH)/, $(HDRS_NAME))
OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAME))
SHADERS = $(addprefix $(SHADERS_PATH)/, $(SHADERS_NAME))
SHADERS_RESULT = $(addprefix $(SHADERS_PATH)/, $(SHADERS_RESULT_NAME))

all: $(NAME) shaders Makefile

$(NAME): $(SRCS) $(HDRS) $(OBJS) Makefile
	$(CC) $(CXXFLAGS) $(INCS_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.cpp $(HDRS) Makefile
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	@echo "\033[38;2;0;255;0m[cc]\033[0m: $< -> $@"
	@printf "\e[1A"
	@$(CC) $(CXXFLAGS) -I $(HDRS_PATH) $(INCS_FLAGS) -c $< -o $@
	@printf "\e[0K"

shaders: $(SHADERS_RESULT) Makefile

$(SHADERS_RESULT): $(SHADERS)
	$(GLS) -V $(SHADERS)
	mv *.spv $(SHADERS_PATH)/

clean:
	rm -f $(OBJS)
	rm -f $(SHADERS_PATH)/*.spv

fclean: clean
	rm -f $(NAME)

re: fclean all
