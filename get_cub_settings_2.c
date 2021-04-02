/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cub_settings_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:51:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/04/02 00:59:52 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_digits(char *s)
{
	while (*s)
	{
		if (!(ft_isdigit(*s)))
			return (0);
		s++;
	}
	return (1);
}

static int	check_commas(char *s)
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

static int	get_color(char *color)
{
	int c;

	if (!check_digits(color))
		exit_error("Error\nWrong color format");
	c = ft_atoi(color);
	if (c < 0 || c > 255)
		exit_error("Error\nWrong color range");
	return (c);
}

void		parse_resolution(t_setting *setting, t_cub *cub)
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
	get_screen_size(&cub->config.rx, &cub->config.ry);
	x = ft_atoi(setting->words[1]);
	y = ft_atoi(setting->words[2]);
	if (!x || !y)
		exit_error("Error\nWrong resolution range");
	if (x < cub->config.rx && x != -1)
		cub->config.rx = x;
	if (y < cub->config.ry && y != -1)
		cub->config.ry = y;
}

void		parse_color(t_setting *setting, int *color)
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
