CC=c++
CPPFILES=-Wall -Wextra -Werror -std=c++98  
CFILES= $(shell find . -name '*.cpp')
HEADER=$(shell find . -name '*.hpp')
OBJS=$(CFILES:.cpp=.o)


NAME=ircserv


all:$(NAME)

%.o:%.cpp $(HEADER)
	$(CC) $(CPPFILES) -c $< -o $@

$(NAME):$(OBJS)
	$(CC) $(CPPFILES) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean:clean
	@rm -rf $(NAME)

re : fclean all



PORT ?= 9000
PASS ?= MYPASS

run: all clean
	@echo "\n\033[1;31m🚀 Running $(NAME)...\033[0m\n"
	@./$(NAME) $(PORT) $(PASS)
	@echo "\n\033[1;32m✅ Done! Enjoy!\033[0m\n"
