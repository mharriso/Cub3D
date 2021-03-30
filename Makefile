CC		=	gcc

NAME	=	cub3D

CFLAGS	=	-Wall -Wextra -Werror

LIBFT	=	libft

HDRS	=	./

SRCS	:= get_cub_settings_1.c get_cub_settings_2.c \
			get_map.c parse_map_1.c parse_map_2.c \
			render_cub.c render_wall.c render_sprites.c \
			take_screenshot.c create_bmp_file \
			cub3d_utils_1.c cub3d_utils_2.c \
			key_handler.c

OBJS	=	$(SRCS:.c=.o)

RM		=	rm -f

all:	$(NAME)

$(NAME):	$(OBJS)
		make bonus -C $(LIBFT)
		cp libft/libft.a ./$(NAME)
		make -C ${MLX}
		gcc ${CFLAGS} -I ${HDRS} main.c libft.a $(SRCS) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)


bonus: $(NAME)

.c.o:
		$(CC) $(CFLAGS) -I $(HDRS) -c $< -o $(<:.c=.o)

clean:
		$(RM) $(OBJS)
		make clean -C $(LIBFT)

fclean:	clean
		$(RM) $(NAME)
		make fclean -C $(LIBFT)

re:		fclean all

.PHONY:	all clean fclean re
