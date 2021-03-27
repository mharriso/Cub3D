/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bmp_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:19:15 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/27 20:30:04 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define BMP_HEAD_SIZE 14
#define DIB_HEAD_SIZE 40
#define BITS_PER_PIXEL 24
#define COLOR_PLANE 1

static void	save(unsigned char *array, int n, int size)
{
	int i;

	i = 0;
	while (i < size / 8)
	{
		array[i] = n >> i * 8;
		i++;
	}
}

static void	write_bmp_header(t_bmp *bmp)
{
	unsigned char *header;

	if (!(header = ft_calloc(BMP_HEAD_SIZE, 1)))
		exit_error("Error\nFailed create array");
	header[0] = 'B';
	header[1] = 'M';
	save(header + 2, bmp->data_size + BMP_HEAD_SIZE + DIB_HEAD_SIZE, 32);
	save(header + 10, BMP_HEAD_SIZE + DIB_HEAD_SIZE, 32);
	write(bmp->fd, header, BMP_HEAD_SIZE);
	free(header);
}

static void	write_dib_header(t_bmp *bmp, int x, int y)
{
	unsigned char *header;

	if (!(header = ft_calloc(DIB_HEAD_SIZE, 1)))
		exit_error("Error\nFailed create array");
	save(header, DIB_HEAD_SIZE, 32);
	save(header + 4, x, 32);
	save(header + 8, y, 32);
	save(header + 12, COLOR_PLANE, 16);
	save(header + 14, BITS_PER_PIXEL, 16);
	save(header + 20, bmp->data_size, 32);
	write(bmp->fd, header, DIB_HEAD_SIZE);
	free(header);
}

static void	write_pixel_array(t_bmp *bmp, t_img *cub3d, int r_x, int r_y)
{
	unsigned char	*pixels;
	int				x;
	int				offset;
	int				color;

	if (!(pixels = ft_calloc(bmp->data_size, 1)))
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
		offset += bmp->padding_size;
	}
	write(bmp->fd, pixels, bmp->data_size);
	free(pixels);
}

void		create_bmp_file(t_img *cub3d, int x, int y)
{
	t_bmp			bmp;

	if ((bmp.fd = open("screenshot_cub3d.bmp", O_CREAT | O_WRONLY | O_TRUNC, \
		S_IRUSR | S_IWUSR)) == -1)
		exit_error("Error\nСan't create screenshot_cub3d.bmp");
	bmp.padding_size = x % 4;
	bmp.data_size = (x * 3 + bmp.padding_size) * y;
	write_bmp_header(&bmp);
	write_dib_header(&bmp, x, y);
	write_pixel_array(&bmp, cub3d, x, y);
	close(bmp.fd);
}
