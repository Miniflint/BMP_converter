FLAGS = -Wall -Wextra -Werror
TARGET = BMP_converter
CC = gcc

OBJS = ${FILES:.c=.o}

ifeq ($(OS), Windows_NT)
	SHELL = cmd
	REMOVEFILE = del
	FILES = srcs\main.c srcs\linked_list_utils.c srcs\parse_data.c srcs\utils.c srcs\bmp_write.c srcs\bmp_read.c srcs\encode.c srcs\decode.c
	TARGET := ${TARGET}.exe
else
	FILES = srcs/main.c srcs/linked_list_utils.c srcs/parse_data.c srcs/utils.c srcs/bmp_write.c srcs/bmp_read.c srcs/encode.c srcs/decode.c
	REMOVEFILE = rm -rf
endif

ifeq (${DEBUG}, debug)
	CCDEBUG = -fsanitize=address -g
endif

all: ${TARGET}

${TARGET}: ${OBJS}
	@echo "Generating executable"
	@${CC} ${CCDEBUG} ${OBJS} -o ${TARGET}

%.o: %.c
	@echo "Generating $@"
	@${CC} ${FLAGS} -c $< -o $@

build:
	docker build -t arch .
	docker run -v .:/checking_files -ti arch

prune:
	docker system prune -a --volumes

clean:
	@echo "Deleting objs files"
	${REMOVEFILE} ${OBJS}

fclean: clean
	@echo "Deleting Executable"
	${REMOVEFILE} ${TARGET}

re: fclean all

.PHONY: clean fclean prune build all re
