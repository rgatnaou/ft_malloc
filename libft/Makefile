SRCS = srcs/ft_strlen.c

CFLAGS = -Wall -Wextra -Werror

CC = gcc

HEADERS = includes

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADERS)

all: libft.a

libft.a: $(OBJS) $(HEADERS)
	ar rcs libft.a $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f libft.a
