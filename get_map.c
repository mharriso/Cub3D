/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:40:00 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/13 14:34:34 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_new_elem(t_list **lst, char *s)
{
	t_list	*elem;

	elem = NULL;
	if (!(elem = ft_lstnew(s)))
		exit_error("Error\nCan not create map list");
	ft_lstadd_front(lst, elem);
}

void	create_map_lst(char *first_line, t_list **map_lst, t_map *map)
{
	char	*line;
	int		ret;

	line = NULL;
	add_new_elem(map_lst, first_line);
	while ((ret = get_next_line(map->fd, &line)) > 0 && line[0])
	{
		add_new_elem(map_lst, line);
		map->height++;
	}
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	if (ret != 0)
		exit_error("Error\nInvalid map. Extra lines.");
	if (*line)
		add_new_elem(map_lst, line);
	else
		free(line);
}

char	**create_map_arr(t_list **head, int size)
{
	char	**map;
	int		i;
	t_list	*tmp;

	tmp = *head;
	if (!(map = malloc((size + 1) * sizeof(char *))))
		exit_error("Error\nCan not create map array");
	map[size] = NULL;
	while (tmp)
	{
		map[--size] = tmp->content;
		tmp = tmp->next;
	}
	return (map);
}

void	check_border(t_map *map, int x, int y)
{
	if (x == map->width - 1 || y == map->height - 1)
		exit_error("Error\nInvalid map. Gap detected!");
	if (x == 0 || y == 0)
		exit_error("Error\nInvalid map. Gap detected!");
}

void	check_cross(t_map *map, int x, int y)
{
	if (map->map[y][x - 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y][x + 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y - 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y + 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
}

void	set_player(t_cub *cub, char dir, int x, int y)
{

	if (cub->player.x != 0)
		exit_error("Error\nMany players! Need only one");
	cub->player.x = x + 0.5;
	cub->player.y = y + 0.5;
	cub->map.map[y][x] = '0';
	// if (dir == 'N')
	// {

	// }
	// else if (dir == 'S')
	// {

	// }
	// else if (dir == 'E')
	// {

	// }
	// else if (dir == 'W')
	// {

	// }
}

void	parse_map_1(t_cub *cub)
{
	int	x;
	int	y;

	y = 0;
	while (y < cub->map.height)
	{
		x = 0;
		cub->map.width = ft_strlen(cub->map.map[y]);
		while (x < cub->map.width)
		{
			if (!ft_strchr(VALID_OBJS, cub->map.map[y][x]))
				exit_error("Error\nInvalid map. Invalid object detected!");
			if (ft_strchr(INNER_OBJS, (cub->map.map)[y][x]))
				check_border(&cub->map, x, y);
			if (cub->map.map[y][x] == SPRITE)
				cub->map.spr_amt++;
			if (ft_strchr(PLAYER, cub->map.map[y][x]))
				set_player(cub, cub->map.map[y][x], x, y);
			x++;
		}
		printf("%s\n", cub->map.map[y]);
		y++;
	}
	if (cub->player.x == 0)
		exit_error("Error\nMissing player position");
	printf("sprites = %d\n", cub->map.spr_amt);
	printf("player = %f %f\n", cub->player.x, cub->player.y);
}

void	set_sprite(t_sprite **sprite, int x, int y)
{

}

void	parse_map_2(t_map *map)
{
	int	x;
	int	y;

	if (map->spr_amt)
		if (!(map->sprites = malloc(map->spr_amt + 1 * sizeof(t_sprite))))
			exit_error("Error\nCan not create sprites array");
	y = 1;
	while (y < map->height - 1)
	{
		x = 1;
		map->width = ft_strlen(map->map[y]);
		while (x < map->width - 1)
		{
			if (ft_strchr(INNER_OBJS, (map->map)[y][x]))
				check_cross(map, x, y);
			if (map->map[y][x] == SPRITE)
			{
				--map->spr_amt;
				map->sprites[map->spr_amt].x = x;
				map->sprites[map->spr_amt].y = y;
			}
			x++;
		}
		y++;
	}
}

void	get_cub_map(char *first_line, t_cub *cub)
{
	t_list	*map_lst;

	map_lst = NULL;
	create_map_lst(first_line, &map_lst, &cub->map);
	cub->map.height = ft_lstsize(map_lst);
	cub->map.map = create_map_arr(&map_lst, cub->map.height);
	parse_map_1(cub);
	parse_map_2(&cub->map);
	printf("sprites = %d\nx = %f y = %f\n", cub->map.spr_amt , cub->map.sprites[0].x,cub->map.sprites[0].y);
	ft_lstclear(&map_lst, NULL);
}
