# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 10:39:30 by victor            #+#    #+#              #
#    Updated: 2024/11/26 18:36:58 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# List all your source files here
SRCS = src/main.c src/parse.c src/pipes.c

# Set the object directory
OBJ_DIR = ./obj
# Replace .c with .o for object files
OBJ = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Include directories
INCLUDE = inc/minishell.h libft/libft.h

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -I inc

# Linker flags (add any required libraries here)
LDFLAGS = -lcurses -ltermcap -lreadline -lhistory -lncurses

# LIBFT settings
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Colors for output
GREEN_DARK = \033[0;32m    # Green
WHITE = \033[0m            # White
RED = \033[0;91m           # Red
RESET = \033[0m            # Reset

# Default target to build the project
all: libs $(NAME)

# Create the executable
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBFT)
	@echo "$(GREEN_DARK)Executable $(NAME) created successfully!$(RESET)"

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.c $(INCLUDE) Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN_DARK)Compiled: $<$(RESET)"

# Rule to build libft
libs:
	@make -C $(LIBFT_DIR)

# Clean up object files
clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(RED)Cleaned up object files.$(RESET)"

# Remove executable and cleaned files
fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(RED)Executable $(NAME) removed.$(RESET)"

# Rebuild the project from scratch
re: fclean all

# Phony targets
.PHONY: all libs clean fclean re
