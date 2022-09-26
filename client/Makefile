NAME = client

INCDIR		= include
SRCDIR		= src
OBJDIR		= obj

SRC_FILES =	client.cpp

INC_FILES =	client.hpp

OBJS_FILES	= $(SRC_FILES:%.cpp=%.o)

OBJS		:= $(addprefix $(OBJDIR)/, $(OBJS_FILES))
INCLUDES	:= $(addprefix $(INCDIR)/, $(INC_FILES))

FLAGS	=	-Wall -Wextra -Werror -std=c++98
CC		= clang++

all: 		$(OBJDIR) $(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) $(OBJS) -o $@

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES) | $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -c $< -o $@ 

clean:
			@-rm -rf $(OBJDIR)

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all

.PHONY: 	all clean fclean re
