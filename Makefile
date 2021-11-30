NAME = ircserv

CC = clang++
MAKE = make --no-print-directory

CFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS += -g
# CFLAGS += -O3 -fno-builtin
CFLAGS += -fsanitize=address

INCLUDE_PATH= ./includes

HEADERS = \
		$(INCLUDE_PATH)/commands/help.hpp \
		$(INCLUDE_PATH)/commands/join.hpp \
		$(INCLUDE_PATH)/channel.hpp \
		$(INCLUDE_PATH)/command.hpp \
		$(INCLUDE_PATH)/parser.hpp \
		$(INCLUDE_PATH)/rpl_codes.hpp \
		$(INCLUDE_PATH)/servData.hpp \
		$(INCLUDE_PATH)/serverException.hpp \
		$(INCLUDE_PATH)/user.hpp \

SRCS_PATH= ./src
SRCS = \
		$(SRCS_PATH)/commands/help.cpp \
		$(SRCS_PATH)/commands/join.cpp \
		$(SRCS_PATH)/command.cpp \
		$(SRCS_PATH)/main.cpp \
		$(SRCS_PATH)/parser.cpp\
		$(SRCS_PATH)/sendMessages.cpp \
		$(SRCS_PATH)/servData.cpp \
		$(SRCS_PATH)/user.cpp \

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

run: all
	./ircserv 8080 bite

.PHONY: all clean fclean re
