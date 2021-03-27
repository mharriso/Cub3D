/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_screenshot.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 17:10:37 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/27 20:19:25 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_screenshot(t_cub *cub)
{
	cub->wall.rays = malloc(cub->config.rx * sizeof(float));
	if (!(cub->map.cub3d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, \
		cub->config.ry)))
		exit_error("Error\nFailed creating mlx image");
	cub->map.cub3d.addr = mlx_get_data_addr(cub->map.cub3d.img, \
	&cub->map.cub3d.bits_per_pixel, &cub->map.cub3d.line_length, \
	&cub->map.cub3d.endian);
	ray_loop(cub);
}

void	take_screenshot(t_cub *cub)
{
	render_screenshot(cub);
	create_bmp_file(&cub->map.cub3d, cub->config.rx, cub->config.ry);
}
