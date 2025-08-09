.PHONY : all clean fclean re

#files
SRC_FILES = \
	main_bonus.c \
	main_process/cleanup.c \
	main_process/create_philos.c \
	main_process/create_childs_process.c \
	main_process/init_data.c \
	main_process/wait_philos.c \
	main_process/trigger_ending.c \
	main_process/secur_sem_open.c \
	main_process/start_end_simulation.c \
	childs_process/child_thread_routines.c \
	childs_process/philo_actions.c \
	childs_process/philo_managing_forks.c \
	childs_process/process_routine.c \
	childs_process/philo_log.c \
	childs_process/simulation_check.c \
	utils/atol_alt.c \
	utils/ft_calloc.c \
	utils/ft_itoa.c \
	utils/ft_strdup.c \
	utils/ft_strjoin.c \
	utils/ft_strlen.c \
	utils/misc.c \

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
CFLAGS = -Wall -Wextra -Werror

DEBUG_VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes -s

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
