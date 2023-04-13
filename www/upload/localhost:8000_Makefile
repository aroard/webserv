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
	apt-get update
	apt-get install -y build-essential autoconf \
		automake libtoo libkqueue-dev cmake &> /dev/null
	git clone https://github.com/mheily/libkqueue.git /libkqueue
	cd /libkqueue
	cmake3 -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib <path to source>
	make
	cpack3 -G RPM
	mkdir -p /usr/include/kqueue/sys/
	cp -f /libkqueue/include/sys/event.h /usr/include/kqueue/sys/event.h
	rm -rf /libkqueue



