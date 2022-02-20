/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:53:51 by mharriso          #+#    #+#             */
/*   Updated: 2022/02/20 04:28:01 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void		init_cub(t_cub *cub)
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

static int		check_map_type(char *path)
{
	size_t len;

	len = ft_strlen(path);
	if (len < 5)
		exit_error("Error\nWrong map file type. Expect (*.cub)");
	return (ft_strcmp(path + (len - 4), ".cub"));
}

static void		check_args(int argc, char **argv, int *cub3d_mode)
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
		else
			exit_error("Error\nInvalid argument");
	}
	*cub3d_mode = RUN_GAME;
}

int				main(int argc, char **argv)
{
	int		cub3d_mode;
	t_cub	cub;

	init_cub(&cub);
	check_args(argc, argv, &cub3d_mode);
	get_cub_settings(argv[1], &cub);
	if (cub3d_mode == SCREENSHOT)
		take_screenshot(&cub);
	else
		render_cub(&cub);
	return (0);
}
