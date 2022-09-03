SRCS =		main.cpp \
			Server.cpp \
			User.cpp \
			utils.cpp \
			Message.cpp \
			Parser.cpp

INCLUDES = Server.hpp User.hpp utils.h Message.hpp Parser.hpp

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