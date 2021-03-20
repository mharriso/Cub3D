/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:53:51 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/20 17:44:06 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

// void		my_mlx_pixel_put(t_img *data, int x, int y, int color)
// {
// 	char	*dst;

// 	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
// 	*(unsigned int*)dst = color;
// }

// int			my_mlx_pixel_get(t_img *data, int x, int y)
// {
// 	char	*dst;

// 	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
// 	return (*(unsigned int*)dst);
// }

void	exit_error(char *s)
{
	ft_putstr_fd(RED, FD);
	if (errno)
		perror(s);
	else
		ft_putendl_fd(s, FD);
	ft_putstr_fd(RESET, FD);
	exit(EXIT_FAILURE);
}

char	**free_2d_array(char **array)
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

int		check_digits(char *s)
{
	while (*s)
	{
		if (!(ft_isdigit(*s)))
			return (0);
		s++;
	}
	return (1);
}

int		check_commas(char *s)
{
	int i;

	i = 0;
	while (*s)
	{
		if (*s == ',')
			i++;
		if (i > 2)
			return (0);
		s++;
	}
	return (i == 2 ? 1 : 0);
}
void	check_cub_settings(t_cub *cub)
{
	if (cub->config.rx == -1 || cub->config.ry == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing resolution");
	if (cub->config.ceiling == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing ceiling color");
	if (cub->config.floor == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing floor color");
	if (!cub->config.north.img || !cub->config.south.img \
		|| !cub->config.east.img || !cub->config.west.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing wall texture path");
	if (!cub->config.sprite.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing sprite texture path");
}
void	parse_resolution(t_setting *setting, t_cub *cub)
{
	int	x;
	int	y;

	if (cub->config.rx != -1 || cub->config.ry != -1)
		exit_error("Error\nWrong resolution format. Double definition");
	if (setting->len != 3)
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[1]))
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[2]))
		exit_error("Error\nWrong resolution format");
	//mlx_get_screen_size(cub->mlx.mlx, &cub->config.rx, &cub->config.ry);
	cub->config.rx = 2400;
	cub->config.ry = 1600;
	x = ft_atoi(setting->words[1]);
	y = ft_atoi(setting->words[2]);
	if (x < 1 || y < 1)
		exit_error("Error\nWrong resolution range");
	if (x < cub->config.rx)
		cub->config.rx = x;
	if (y < cub->config.ry)
		cub->config.ry = y;
}

void	parse_texture(void *mlx, t_setting *setting, t_img *img)
{
	if (img->img)
		exit_error("Error\nWrong texture path format. Double definition");
	if (setting->len != 2)
		exit_error("Error\nWrong texture path format");
	//printf("%s\n", setting->words[1]);
	if (!(img->img = mlx_xpm_file_to_image(mlx, setting->words[1], \
	&img->width, &img->height)))
		exit_error("Error\nFailed creating mlx image instance from file");
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, \
	&img->line_length, &img->endian);
}

int		get_color(char *color)
{
	int c;

	if (!check_digits(color))
		exit_error("Error\nWrong color format");
	c = ft_atoi(color);
	if (c < 0 || c > 255)
		exit_error("Error\nWrong color range");
	return (c);
}

void	parse_color(t_setting *setting, int *color)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	if (*color != -1)
		exit_error("Error\nWrong color format. Double definition");
	if (!check_commas(setting->line))
		exit_error("Error\nWrong color format");
	if (!(rgb = ft_split_set(setting->line, SPACE",", &setting->len)))
		exit_error("Error\nError while creating settings array");
	if (setting->len != 4)
		exit_error("Error\nWrong color format");
	r = get_color(rgb[1]) << 16;
	g = get_color(rgb[2]) << 8;
	b = get_color(rgb[3]);
	*color = r | g | b;
	free_2d_array(rgb);
}

void	parse_settings(t_setting *setting, t_cub *cub)
{
	if (!(ft_strcmp("R", setting->words[0])))
		parse_resolution(setting, cub);
	else if (!(ft_strcmp("NO", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.north);
	else if (!(ft_strcmp("SO", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.south);
	else if (!(ft_strcmp("WE", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.west);
	else if (!(ft_strcmp("EA", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.east);
	else if (!(ft_strcmp("S", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.sprite);
	else if (!(ft_strcmp("C", setting->words[0])))
		parse_color(setting, &cub->config.ceiling);
	else if (!(ft_strcmp("F", setting->words[0])))
		parse_color(setting, &cub->config.floor);
	else if (setting->words[0][0] == '1')
	{
		check_cub_settings(cub);
		get_cub_map(setting->line, cub);
	}
	else
		exit_error("Error\nInvalid map file configs");
}

int		get_setting(t_setting *setting, t_cub *cub)
{
	if (!(setting->words = ft_split_set(setting->line, SPACE, &setting->len)))
		exit_error("Error\nCan not create settings array");
	if (!(setting->words[0]))
	{
		free(setting->line);
		free_2d_array(setting->words);
		return (0);
	}
	parse_settings(setting, cub);
	if (!cub->map.map)
		free(setting->line);
	setting->line = NULL;
	free_2d_array(setting->words);
	return (1);
}

void	get_cub_settings(char *path, t_cub *cub)
{
	int			ret;
	t_setting	setting;

	if ((cub->map.fd = open(path, O_RDONLY)) == -1)
		exit_error("Error\nOpen map file failed");
	setting.line = NULL;
	while ((ret = get_next_line(cub->map.fd, &setting.line)) > 0)
		get_setting(&setting, cub);
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	get_setting(&setting, cub);
}


void	init_cub(t_cub *cub)
{
	if (!(cub->mlx.mlx = mlx_init()))
		exit_error("Error\nMlx init error");
	errno = 0;
	cub->config.rx = -1;
	cub->config.ry = -1;
	cub->config.floor = -1;
	cub->config.ceiling = -1;
	cub->config.north.img = NULL;
	cub->config.south.img = NULL;
	cub->config.east.img = NULL;
	cub->config.west.img = NULL;
	cub->config.sprite.img = NULL;
	cub->map.map = NULL;
	cub->map.height = 0;
	cub->map.width = 0;
	cub->map.spr_amt = 0;
	cub->player.pos_x = 0;
}

int		check_map_type(char *path)
{
	size_t len;

	len = ft_strlen(path);
	if (len < 5)
		exit_error("Error\nWrong map file type. Expect (*.cub)");
	return (ft_strcmp(path + (len - 4), ".cub"));
}

void	check_args(int argc, char **argv, int *cub3d_mode)
{
	if (argc < 2)
		exit_error("Error\nProgram requires at least one argument (*.cub)");
	if (check_map_type(argv[1]))
		exit_error("Error\nWrong map file type. Expect (*.cub)");
	if (argc == 3)
	{
		if (!(ft_strcmp("--save", argv[2])))
		{
			*cub3d_mode = SCREENSHOT;
			return ;
		}
	}
	*cub3d_mode = RUN_GAME;
}

int		main(int argc, char **argv)
{
	int		cub3d_mode;
	t_cub	cub;

	errno = 0;
	init_cub(&cub);
	check_args(argc, argv, &cub3d_mode);
	get_cub_settings(argv[1], &cub);
	printf("END\n");
	render_cub(&cub);
	//close(cub.map.fd);
	sleep(30);
}
