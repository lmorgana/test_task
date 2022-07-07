NAME            = tz
SRCS            = main.cpp socket.cpp  server.cpp session.cpp EventSelector.cpp FdListener.cpp logg.cpp utils.cpp
OBJS            = $(patsubst %.cpp, %.o, $(SRCS))
HEADERS         = header.hpp sockets.hpp server.hpp session.hpp Eventselector.hpp FdListener.hpp logg.hpp
CC              = clang++
RM              = rm -f
FLAGS           = -Wall -Wextra -Werror
%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@
all:            $(NAME)
$(NAME):        $(HEADERS) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
clean:
	$(RM) $(OBJS)
fclean:         clean
	$(RM) $(NAME)
re:             fclean $(NAME)
.PHONY:         all clean fclean re