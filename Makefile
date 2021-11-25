NAME = ircserv

CC = clang++
MAKE = make --no-print-directory

CFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS += -g
# CFLAGS += -O3 -fno-builtin
CFLAGS += -fsanitize=address

INCLUDE_PATH= ./srcs/Server
HEADERS = \
		$(INCLUDE_PATH)/servData.hpp \
		$(INCLUDE_PATH)/Ircserv.hpp \

SRCS_PATH= ./srcs
SRCS = \
		$(SRCS_PATH)/main.cpp \
		$(SRCS_PATH)/Server/Ircserv.cpp \
		$(SRCS_PATH)/Server/servData.cpp \

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
