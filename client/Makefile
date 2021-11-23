NAME = ft_irc_client

CC = clang++
MAKE = make --no-print-directory

CFLAGS = -Wall -Wextra -Werror
CFLAGS = -g
# CFLAGS = -O3 -fno-builtin
CFLAGS += -fsanitize=address

HEADERS = \

SRCS_PATH = ./src

SRCS = \
		$(SRCS_PATH)/main.cpp \

OBJS = $(SRCS:%.cpp=%.o)

%.o: %.cpp Makefile $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean: clean
		$(RM) $(NAME)

re: fclean
		$(MAKE) all

.PHONY: all clean fclean re
