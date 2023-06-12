SRC = ./boggle.c
OBJ = $(SRC:.c=.o)

CC = clang
CFLAGS += -Wall -Werror -Wextra

NAME = boggle

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all