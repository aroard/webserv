CC = c++

CFLAGS = -I/usr/include/kqueue/ \
		 -std=c++98 #-Wall -Werror -Wextra
IFLAGS = -L/usr/local/lib -lkqueue

OBJ_DIR = obj/

OBJ = main.o

PREFIXE = $(addprefix  $(OBJ_DIR),$(OBJ))

NAME = webserver

all : mkdir_obj $(NAME)

$(NAME) : $(PREFIXE)
	$(CC) -o $@ $^ $(IFLAGS)

$(OBJ_DIR)%.o : ./srcs/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

mkdir_obj :
	@mkdir -p obj

.PHONY : all mkdir_obj clean fclean re rr r

clean :
	rm -f $(OBJ_DIR)*.o

fclean : clean
	rm -f $(NAME)

re : fclean all

rr : re r

r : all
	./$(NAME)

install:
	apt-get install -y libkqueue-dev
