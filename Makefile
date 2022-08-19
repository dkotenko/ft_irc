SRCS =		main.cpp

INCLUDES =

OBJS	= 	$(SRCS:%.cpp=%.o)

NAME	=	ft_irc

FLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic
CC		= clang++

all: 		$(NAME)

$(NAME):	$(OBJS) $(INCLUDES)
			$(CC) $(FLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(FLAGS) -I ./ -c $< -o $@

clean:
			@rm -rf *.o

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all

.PHONY: 	all clean fclean re