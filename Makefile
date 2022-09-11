
NAME		=	ft_irc

INCDIR		= include
SRCDIR		= src
OBJDIR		= obj

SRC_FILES =	main.cpp \
			Server.cpp \
			User.cpp \
			utils.cpp \
			MessageInput.cpp \
			Parser.cpp \
			ServerData.cpp \
			Channel.cpp

INC_FILES =	Server.hpp \
			User.hpp \
			utils.hpp \
			MessageInput.hpp \
			Parser.hpp \
			ServerData.hpp \
			Channel.hpp \
			const.hpp

OBJS_FILES	= $(SRC_FILES:%.cpp=%.o)

OBJS		:= $(addprefix $(OBJDIR)/, $(OBJS_FILES))
INCLUDES	:= $(addprefix $(INCDIR)/, $(INC_FILES))

FLAGS	=	-Wall -Wextra -Werror -std=c++98 -g
CC		= clang++

all:
	@make --no-print-directory -j $(NAME)

$(NAME):	$(OBJDIR) $(OBJS)
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