/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:40:20 by mharriso          #+#    #+#             */
/*   Updated: 2022/02/20 04:27:41 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	swap_sprite(t_sprite *a, t_sprite *b)
{
	t_sprite tmp;

	tmp.dst = a->dst;
	tmp.x = a->x;
	tmp.y = a->y;
	a->dst = b->dst;
	a->x = b->x;
	a->y = b->y;
	b->dst = tmp.dst;
	b->x = tmp.x;
	b->y = tmp.y;
}

void		sort_array(t_sprite *arr, int n)
{
	int		max;
	int		index;
	int		i;
	int		j;

	i = -1;
	while (++i < n)
	{
		max = arr[i].dst;
		index = -1;
		j = i;
		while (++j < n)
		{
			if (arr[j].dst > max)
			{
				max = arr[j].dst;
				index = j;
			}
		}
		if (index != -1)
			swap_sprite(&(arr[i]), &(arr[index]));
	}
}

void		check_angle(float *a)
{
	if (*a > (float)M_PI * 2)
		*a -= (float)M_PI * 2;
	else if (*a < 0)
		*a += (float)2 * M_PI;
}

int			my_mlx_pixel_get(t_img *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)dst);
}

void		my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
