CC=c++
CPPFLAGS=-Wall -Wextra -Werror -std=c++98
CPPFILES= $(shell find . -name '*.cpp')
HEADER=$(shell find . -name '*.hpp')
OBJS=$(CPPFILES:.cpp=.o)


NAME=ircserv


all:$(NAME)

%.o:%.cpp $(HEADER)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME):$(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean:clean
	@rm -rf $(NAME)

re : fclean all



PORT ?= 9001
PASS ?= MYPASS

run: all clean
	@echo "\n\033[1;31m🚀 Running $(NAME)...\033[0m\n"
	@./$(NAME) $(PORT) $(PASS)
	@echo "\n\033[1;32m✅ Done! Enjoy!\033[0m\n"
