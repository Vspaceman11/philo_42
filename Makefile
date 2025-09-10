# ==== Project Name ====
NAME = philo

# ==== Compiler Settings ====
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

# ==== Directories ====
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

# ==== Source and Object Files ====
SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/philo_simulation.c \
		$(SRC_DIR)/utils.c \
		$(SRC_DIR)/init.c \
		$(SRC_DIR)/philo_utils.c
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# ==== Tools ====
RM = rm -f
MKDIR = mkdir -p

# ==== Colored Messages ====
YELLOW = \033[1;33m
GREEN  = \033[1;32m
RED    = \033[1;31m
RESET  = \033[0m

# ==== Targets ====

all: $(NAME)
	@echo "$(GREEN)✓ Build complete: $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)→ Compiling $<$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(NAME): $(OBJS)
	@echo "$(YELLOW)→ Linking $(NAME)$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $@

# ==== Cleanup ====
clean:
	@echo "$(RED)✗ Cleaning object files...$(RESET)"
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo "$(RED)✗ Removing binary: $(NAME)$(RESET)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
