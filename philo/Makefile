.PHONY : all clean fclean re

#files
SRC_FILES = \
	main.c \
	main_process/cleanup.c \
	main_process/create_forks.c \
	main_process/create_philos.c \
	main_process/create_threads.c \
	main_process/init_data.c \
	main_process/monitoring.c \
	main_process/start_end_simulation.c \
	threads/philo_actions.c \
	threads/philo_log.c \
	threads/philo_managing_forks.c \
	threads/simulation_check.c \
	threads/thread_routine.c \
	utils/atol_alt.c \
	utils/ft_strlen.c \
	utils/ft_calloc.c \
	utils/misc.c

#directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

#files full paths
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
DEP = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.d))

NAME = philo

CC = cc
THREAD_FLAGS = -pthread
CFLAGS = -Wall -Wextra -Werror

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
