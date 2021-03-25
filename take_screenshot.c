/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_screenshot.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 17:10:37 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/26 01:05:44 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define BMP_HEAD_SIZE 14
#define DIB_HEAD_SIZE 40
#define BITS_PER_PIXEL 24
#define COLOR_PLANE 1

void	render_screenshot(t_cub *cub)
{
	// 	cub->wall.rays = malloc(cub->config.rx * sizeof(float));
	// if (!(cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, \
	// 	cub->config.ry, "CUB3D")))
	// 	exit_error("Error\nFailed creating mlx window");
	// if (!(cub->map.cub3d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, \
	// 	cub->config.ry)))
	// 	exit_error("Error\nFailed creating mlx image");
	// cub->map.cub3d.addr = mlx_get_data_addr(cub->map.cub3d.img, \
	// &cub->map.cub3d.bits_per_pixel, &cub->map.cub3d.line_length, \
	// &cub->map.cub3d.endian);
	// ray_loop(cub);
	// mlx_loop(cub->mlx.mlx);

	cub->wall.rays = malloc(cub->config.rx * sizeof(float));
	if (!(cub->map.cub3d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, \
		cub->config.ry)))
		exit_error("Error\nFailed creating mlx image");
	cub->map.cub3d.addr = mlx_get_data_addr(cub->map.cub3d.img, \
	&cub->map.cub3d.bits_per_pixel, &cub->map.cub3d.line_length, \
	&cub->map.cub3d.endian);
	ray_loop(cub);
	mlx_loop(cub->mlx.mlx);
}

void	save(unsigned char *array, int n, int size)
{
	int i;

	i = 0;
	while (i < size / 8)
	{
		array[i] = n >> i * 8;
		i++;
	}
}

unsigned char	*create_bmp_header(int data_size)
{
	unsigned char *header;

	if (!(header = ft_calloc(BMP_HEAD_SIZE, 1)))
		exit_error("Error\nFailed create array");
	header[0] = 'B';
	header[1] = 'M';
	save(header + 2, data_size + BMP_HEAD_SIZE + DIB_HEAD_SIZE, 32);
	save(header + 10, BMP_HEAD_SIZE + DIB_HEAD_SIZE, 32);
	return (header);
}

unsigned char	*create_dib_header(int data_size, int x, int y)
{
	unsigned char *header;

	if (!(header = ft_calloc(DIB_HEAD_SIZE, 1)))
		exit_error("Error\nFailed create array");
	save(header, DIB_HEAD_SIZE, 32);
	save(header + 4, x, 32);
	save(header + 8, y, 32);
	save(header + 12, COLOR_PLANE, 16);
	save(header + 14, BITS_PER_PIXEL, 16);
	save(header + 20, data_size, 32);
	return (header);
}

unsigned char	*create_pixel_array(int data_size, int padding, t_img *cub3d, int r_x, int r_y)
{
	unsigned char	*pixels;
	int				x;
	int				offset;
	int				color;

	if (!(pixels = ft_calloc(data_size, 1)))
		exit_error("Error\nFailed create array");
	offset = 0;
	while (r_y--)
	{
		x = 0;
		while (x < r_x)
		{
			color = my_mlx_pixel_get(cub3d, x, r_y);
			save(pixels + offset, color, 24);
			offset += 3;
			x++;
		}
		offset += padding;
	}
	return (pixels);
}

void	create_bmp_file(t_img *cub3d, int x, int y)
{
	int				fd;
	int				data_size;
	int				padding_size;

	padding_size = x % 4;
	data_size = (x * 3 + padding_size) * y;
	write(fd, create_bmp_header(data_size), BMP_HEAD_SIZE);
	write(fd, create_dib_header(data_size, x, y), DIB_HEAD_SIZE);
	write(fd, create_pixel_array(data_size, padding_size, cub3d, x, y), data_size);
	close(fd);
}

void	take_screenshot(t_cub *cub)
{
	render_screenshot(cub);
	printf("RENDER\n");
	create_bmp_file(&cub->map.cub3d, cub->config.rx, cub->config.ry);
}
