# ========================
#       CONFIGURATION
# ========================
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
INCLUDES = -I includes -I libft/includes
NAME = libft_malloc
# ========================
#       HOSTTYPE CHECK
# ========================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIB_NAME = libft_malloc.so
FULLNAME = $(NAME)_$(HOSTTYPE).so

# ========================
#          SOURCES
# ========================
SRC_MAIN = srcs
SRC_LIBFT = libft/srcs
SRC_UTILS = utils
OBJ_DIR = objs

SRCS =	$(SRC_MAIN)/malloc.c $(SRC_MAIN)/free.c $(SRC_MAIN)/realloc.c \
		$(SRC_MAIN)/show_alloc_mem.c $(SRC_MAIN)/memory_zones.c\
        $(SRC_UTILS)/block_append.c $(SRC_UTILS)/block_fill.c $(SRC_UTILS)/block_find.c \
		$(SRC_UTILS)/block_remove.c $(SRC_UTILS)/block_merge.c\
        $(SRC_UTILS)/global_var.c $(SRC_UTILS)/size.c $(SRC_UTILS)/show.c $(SRC_UTILS)/ptr_search.c \
        $(SRC_UTILS)/heap_create.c $(SRC_UTILS)/heap_find.c $(SRC_UTILS)/heap_remove.c \
        $(SRC_LIBFT)/ft_memset.c $(SRC_LIBFT)/ft_strlen.c $(SRC_LIBFT)/ft_putchar.c $(SRC_LIBFT)/ft_putstr.c 

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# ========================
#           RULES
# ========================
all: $(FULLNAME)

# Build the shared library
$(FULLNAME): $(OBJS)
	$(CC) -shared -o $@ $^
	ln -sf $@ $(LIB_NAME)


# Compile each .c file
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)
	@echo "Object files cleaned"

# Full clean
fclean: clean
	rm -f $(FULLNAME) $(LINKNAME)
	@echo "Full clean done"

# Rebuild
re: fclean all

.PHONY: all libft link clean fclean re
