CC		=	gcc
NAME	=	cub3D
CFLAGS	=	-Wall -Wextra -Werror
LIB	=	libft.a
MLX	=	libmlx.a
MLXFLAGS	=	-framework OpenGL -framework AppKit
INCLUDE		=	./include
SRCDIR		= src
OBJDIR		= .obj
SRCS	:= get_cub_settings_1.c get_cub_settings_2.c \
			get_map.c parse_map_1.c parse_map_2.c \
			render_cub.c render_wall.c render_sprites.c \
			take_screenshot.c create_bmp_file.c \
			cub3d_utils_1.c cub3d_utils_2.c \
			key_handler.c main.c
SRCS	:= $(addprefix $(SRCDIR)/, $(SRCS))
OBJS	= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

GREEN	= \033[1;32m
RESET	= \033[0m

all:	$(NAME)

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

$(LIB):
	@$(MAKE) bonus -C libft
	@mv libft/libft.a .
$(MLX):
	@$(MAKE) -C mlx
	@mv mlx/libmlx.a .
# $(NAME): $(OBJS) $(LIB) $(MLX)
# 	gcc $(CFLAGS) -I $(HDRS) -I libft $(OBJS) $(LIB) $(MLX) $(MLXFLAGS) -o $(NAME)

$(NAME): $(OBJS) $(LIB) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) $(MLX) $(MLXFLAGS) -o $@
	@echo "$(GREEN)Built target $(NAME)$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@
$(OBJDIR):
	@mkdir -p $@

bonus: $(NAME)

clean:
		@-rm -rf $(OBJDIR)
		make clean -C libft
		make clean -C mlx

fclean:	clean
		@-rm -f $(NAME)
		rm -f $(LIB)
		rm -f $(MLX)

re:		fclean all

.PHONY:	all clean fclean re
