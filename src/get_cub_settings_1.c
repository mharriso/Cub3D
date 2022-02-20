/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cub_settings_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:45:01 by mharriso          #+#    #+#             */
/*   Updated: 2022/02/20 04:25:35 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

void	parse_texture(void *mlx, t_setting *setting, t_img *img)
{
	if (img->img)
		exit_error("Error\nWrong texture path format. Double definition");
	if (setting->len != 2)
		exit_error("Error\nWrong texture path format");
	if (!(img->img = mlx_xpm_file_to_image(mlx, setting->words[1], \
	&img->width, &img->height)))
		exit_error("Error\nFailed creating mlx image instance from file");
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, \
	&img->line_length, &img->endian);
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
	if (!cub->map.map)
		exit_error("Eror\nMissing map!");
	close(cub->map.fd);
}
