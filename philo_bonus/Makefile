.PHONY : all clean fclean re

#files
SRC_FILES = \
	main_bonus.c \
	test.c \
	thread_routine_bonus.c

#directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

#files full paths
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
DEP = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.d))

NAME = philo_bonus

CC = cc
THREAD_FLAGS = -pthread
CFLAGS = -Wall -Wextra

DEBUG_VALGRIND = valgrind --leak-check=full --show-leak-kinds=all -s

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(THREAD_FLAGS) $(OBJ) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c Makefile | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -I$(INC_DIR) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

-include $(DEP)

debug: all
	$(DEBUG_VALGRIND) ./$(NAME) $(ARGS)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all
