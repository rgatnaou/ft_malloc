# ========================
#       CONFIGURATION
# ========================
NAME = libft_malloc
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
INCLUDES = -Iincludes -Ilibft/includes

# ========================
#       HOSTTYPE CHECK
# ========================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

FULLNAME = $(NAME)_$(HOSTTYPE).so
LINKNAME = $(NAME).so

# ========================
#          SOURCES
# ========================
SRC_DIR = srcs
SRCS = $(SRC_DIR)/malloc.c \
       $(SRC_DIR)/free.c \
       $(SRC_DIR)/realloc.c \
       $(SRC_DIR)/show_alloc_mem.c \
       $(SRC_DIR)/memory_zones.c \
       $(SRC_DIR)/utils.c

OBJS = $(SRCS:.c=.o)

# ========================
#           RULES
# ========================
all: libft $(FULLNAME) link

# Build libft first
libft:
	@make -C libft

# Main library
$(FULLNAME): $(OBJS)
	$(CC) -shared -o $(FULLNAME) $(OBJS) -Llibft -lft
	@echo "$(FULLNAME) created"

# Create symbolic link
link: $(FULLNAME)
	@ln -sf $(FULLNAME) $(LINKNAME)
	@echo "Symbolic link $(LINKNAME) -> $(FULLNAME) created"

# Compile each .c file
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)
	@make -C libft clean
	@echo "Object files cleaned"

# Full clean
fclean: clean
	rm -f $(FULLNAME) $(LINKNAME)
	@make -C libft fclean
	@echo "Full clean done"

# Rebuild
re: fclean all

.PHONY: all libft link clean fclean re