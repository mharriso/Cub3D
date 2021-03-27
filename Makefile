NAME	=	cub3D
LIB		=	lib
INCL	=	includes
MLX		=	mlx

HEADER	=	bmp_def.h \
			exit.h \
			game_settings.h \
			game_loop.h \
			handle_all.h \
			handlers.h \
			img_save.h \
			input.h \
			int_to_arr.h \
			map_validation.h \
			mlx_add.h \
			movement.h \
			rendering.h \
			set_player.h \
			sprite_sorting.h \
			structs.h

HFILES	=	$(addprefix $(INCL)/, $(HEADER)) ${LIB}/libft.h ${MLX}/mlx.h

LIB_SRC =	${LIB}/ft_atoi.c \
			${LIB}/ft_calloc.c \
			${LIB}/ft_get_next_line.c \
			${LIB}/ft_isdigit.c \
			${LIB}/ft_lstadd_back.c \
			${LIB}/ft_lstclear.c \
			${LIB}/ft_lstdelone.c \
			${LIB}/ft_lstlast.c \
			${LIB}/ft_lstnew.c \
			${LIB}/ft_memchr.c \
			${LIB}/ft_memcmp.c \
			${LIB}/ft_memcpy.c \
			${LIB}/ft_putstr_fd.c \
			${LIB}/ft_s_atoi.c \
			${LIB}/ft_skip_spaces.c \
			${LIB}/ft_strchr.c \
			${LIB}/ft_strdup.c \
			${LIB}/ft_strlen.c \
			${LIB}/ft_trim_space_from_end.c\
			${LIB}/ft_lerp.c

SOURCE =	exit.c \
			game_loop.c \
			mlx_add.c \
			img_save.c \
			int_to_arr.c \
			input.c \
			movement.c \
			rendering1.c \
			rendering2.c \
			sprite_sorting.c \
			sprite_render.c \
			color_handler.c \
			handle_all.c \
			img_handler.c \
			map_validation.c \
			res_handler.c \
			set_player.c \
			sprite_handler.c \
			map_handler.c \
			${LIB_SRC}

FLAGS	= -Wall -Wextra -Werror -I ${MLX} -I ${LIB}

OBJECTS	= ${SOURCE:.c=.o}

all: ${NAME}

$(NAME): ${OBJECTS}
	make -C ${MLX}
	gcc ${FLAGS} -I ${INCL} main.c $(SOURCE) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

%.o: %.c ${HFILES}
	gcc ${FLAGS} -I ${INCL} -Imlx -c $< -o $@

clean:
	make clean -C ${MLX}
	rm -f ${OBJECTS}
fclean: clean
	rm -f ${NAME}
re: fclean all

norm:
	norminette lib/* includes/* src/*

.PHONY: all clean fclean re
