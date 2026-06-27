NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -Iincludes
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main/main.c \
       $(SRC_DIR)/main/args.c \
       $(SRC_DIR)/main/init.c \
       $(SRC_DIR)/main/cleanup.c \
       $(SRC_DIR)/main/main_utils.c \
       $(SRC_DIR)/coder/coder_dongle.c \
       $(SRC_DIR)/coder/coder_routine.c \
       $(SRC_DIR)/coder/coder_utils.c \
       $(SRC_DIR)/dongle/dongle_cmp.c \
       $(SRC_DIR)/dongle/dongle_init.c \
       $(SRC_DIR)/dongle/dongle_heap.c \
       $(SRC_DIR)/dongle/dongle_take.c \
       $(SRC_DIR)/dongle/dongle_utils.c \
       $(SRC_DIR)/monitor/monitor.c \
       $(SRC_DIR)/monitor/monitor_utils.c \
       $(SRC_DIR)/utils/utils_time.c \
       $(SRC_DIR)/utils/utils_log.c

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re