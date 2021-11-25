NAME = ircserv

CC = clang++
MAKE = make --no-print-directory

CFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS += -g
# CFLAGS += -O3 -fno-builtin
CFLAGS += -fsanitize=address

INCLUDE_PATH= ./includes
HEADERS = \
		$(INCLUDE_PATH)/Ircserv.hpp \
		$(INCLUDE_PATH)/servData.hpp \
		$(INCLUDE_PATH)/ServerException.hpp \
		$(INCLUDE_PATH)/User.hpp \

SRCS_PATH= ./src
SRCS = \
		$(SRCS_PATH)/Ircserv.cpp \
		$(SRCS_PATH)/main.cpp \
		$(SRCS_PATH)/servData.cpp \
		$(SRCS_PATH)/User.cpp\

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
