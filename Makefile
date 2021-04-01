CC		=	gcc

NAME	=	cub3D

CFLAGS	=	-Wall -Wextra -Werror

LIB	=	libft.a

MLX	=	libmlx.a

MLXFLAGS	=	-framework OpenGL -framework AppKit

HDRS	=	./

SRCS	:= get_cub_settings_1.c get_cub_settings_2.c \
			get_map.c parse_map_1.c parse_map_2.c \
			render_cub.c render_wall.c render_sprites.c \
			take_screenshot.c create_bmp_file.c \
			cub3d_utils_1.c cub3d_utils_2.c \
			key_handler.c main.c

OBJS	=	$(SRCS:.c=.o)

RM		=	rm -f

all:	$(NAME)

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

$(LIB):
	$(MAKE) bonus -C libft
	@mv libft/libft.a .
$(MLX):
	$(MAKE) -C mlx
	@mv mlx/libmlx.a .
$(NAME): $(OBJS) $(LIB) $(MLX)
	gcc $(CFLAGS) -I $(HDRS) -I libft $(OBJS) $(LIB) $(MLX) $(MLXFLAGS) -o $(NAME)

bonus: $(NAME)

clean:
		$(RM) $(OBJS)
		make clean -C libft
		make clean -C mlx

fclean:	clean
		$(RM) $(NAME)
		rm -f $(LIB)
		rm -f $(MLX)

re:		fclean all

.PHONY:	all clean fclean re
