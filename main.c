#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "./libft/libft.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define SPACE	" \t\v\f\r"
#define FD			1
#define RUN_GAME	0
#define SCREENSHOT	1

enum				e_check_settings
{
	R,
	NO,
	SO,
	WE,
	EA,
	S,
	F,
	C
};

typedef struct  s_img {
    void*	img;
    char*	addr;
    int		bits_per_pixel;
    int		line_length;
    int		endian;
	int		height;
	int		width;
	int		x;
	int		y;
}               t_img;

typedef struct  s_mlx {
    void	*mlx;
    void	*win;
}				t_mlx;

typedef struct s_cube {
	int		rx;
	int		ry;
	int		floor;
	int		cellar;
	int		check[8];
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
	t_img	sprite;
}				t_cube;

// void	set_resolution(char *r, t_mlx *vars)
// {

// 	mlx_get_screen_size(vars->mlx, &vars->rx, &vars->ry);

// }

// void	exit_perror(const char *s)
// {
// 	printf("%d\n", errno);
// 	perror(s);
// 	exit(EXIT_FAILURE);
// }

void	exit_error(char *s)
{
	ft_putstr_fd(RED, FD);
	if(errno)
		perror(s);
	else
		ft_putendl_fd(s, FD);
	ft_putstr_fd(RESET, FD);
	exit(EXIT_FAILURE);
}

static	char	**free_2d_array(char **array)
{
	size_t		i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

int		check_map_type(char *path)
{
	size_t len;

	len = ft_strlen(path);
	if(len < 5)
		exit_error("Wrong map file type. Expect (*.cub)");
	return(ft_strcmp(path + (len - 4), ".cub"));
}

void		check_args(int argc, char **argv, int *cub3d_mode)
{
	if (argc < 2)
		exit_error("Program requires at least one argument (*.cub)");
	if (check_map_type(argv[1]))
		exit_error("Wrong map file type. Expect (*.cub)");
	if (argc > 3)
		exit_error("Too many arguments");
	if (argc == 3)
	{
		if (!(ft_strcmp("--save", argv[2])))
		{
			*cub3d_mode = SCREENSHOT;
			return ;
		}
		else
			exit_error("Unknown argument");
	}
	*cub3d_mode = RUN_GAME;
}
void	parse_resolution(char **s, int rx, int ry)
{
	printf("%s\n%s\n", s[1], s[2]);
}
void	parse_texture(char **s, t_img img)
{
	printf("%s\n", s[1]);
}
void	parse_color(char **s, char *line, int color)
{
	char **RGB;

	if(!(ft_isdigit(s[1][0])))
		exit_error("Wrong color format");

	printf("%s\n%s\n%s\n", s[1], s[2], s[3]);
}
void	parse_settings(char **setting, char *line, t_cube *cube)
{
	if(!(setting[0]))
		return ;
	else if(!(ft_strcmp("R", setting[0])))
		parse_resolution(setting, cube->rx, cube->ry);
	else if(!(ft_strcmp("NO", setting[0])))
		parse_texture(setting, cube->north);
	else if(!(ft_strcmp("SO", setting[0])))
		parse_texture(setting, cube->south);
	else if(!(ft_strcmp("WE", setting[0])))
		parse_texture(setting, cube->west);
	else if(!(ft_strcmp("EA", setting[0])))
		parse_texture(setting, cube->east);
	else if(!(ft_strcmp("S", setting[0])))
		parse_texture(setting, cube->sprite);
	else if(!(ft_strcmp("C", setting[0])))
		parse_color(setting, line, cube->cellar);
	else if(!(ft_strcmp("F", setting[0])))
		parse_color(setting, line, cube->floor);
	else if(setting[0][0] == '1')
		printf("%s\n",setting[0]);
}
int	get_setting(char *line, t_cube *cube)
{
	char	**setting;

	if(!(setting = ft_split_set(line, SPACE)))
			exit_error("Error while creating settings array");
	parse_settings(setting, line, cube);
	free(line);
	line = NULL;
	free_2d_array(setting);
	return (1);
}
void		get_cub_settings(int fd, t_cube *cube)
{
	char	*line;
	int		ret;

	line = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
		get_setting(line, cube);
	if(ret == -1)
		exit_error("Error while reading map file");
	get_setting(line, cube);
}
char	**create_map_arr(t_list **head, int size)
{
	char	**map;
	int		i;
	t_list	*tmp = *head;

	if (!(map = malloc((size + 1) * sizeof(char *))))
		exit_error("Error while creating map array");
	map[size] = NULL;
	i = size;
	while (tmp)
	{
		map[--size] = tmp->content;
		tmp= tmp->next;
	}
	i = -1;
	while (map[++i])
		ft_putendl_fd(map[i], 1);
	return (map);
}

void create_map_lst(char *path, t_list **map, int fd)
{
	char	*line;
	int		ret;
	t_list	*elem;

	elem = NULL;
	line = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if(!(elem = ft_lstnew(line)))
			exit_error("Error while creating map list");
		ft_lstadd_front(map, elem);
		//free(line);
	}
	if(ret == -1)
		exit_error("Error while reading map file");
	if(!(elem = ft_lstnew(line)))
		exit_error("Error while creating map list");
	ft_lstadd_front(map, elem);
	//free(line);
}

int		main(int argc, char **argv)
{
	t_list	*map;
	int		cub3d_mode;
	int		fd;
	t_cube	cube;
	map = NULL;

	// int a[10] = {};
	// for(int i = 0; i < 10; i++)
	// 	printf("%d ", a[i]);
	// printf("\n");

	check_args(argc, argv, &cub3d_mode);
	if((fd = open(argv[1], O_RDONLY)) == -1)
		exit_error("Open map file failed");
	ft_memset(cube.check, 0, sizeof(cube.check));
	get_cub_settings(fd, &cube);

	//create_map_lst(argv[1], &map, fd);
	//ft_lstclear(&map, free);
	//create_map_arr(&map, ft_lstsize(map));

	//sleep(40);
}
