/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:40:00 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/29 20:19:11 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		add_new_elem(t_list **lst, char *s)
{
	t_list	*elem;

	elem = NULL;
	if (!(elem = ft_lstnew(s)))
		exit_error("Error\nCan not create map list");
	ft_lstadd_front(lst, elem);
}

static size_t	map_line_len(char *line)
{
	size_t	len;

	len = ft_strlen(line) - 1;
	while (line[len] == ' ')
		len--;
	return (len + 1);
}

static void		create_map_lst(char *first_line, t_list **map_lst, t_map *map)
{
	char	*line;
	int		ret;
	size_t	len;

	line = NULL;
	add_new_elem(map_lst, first_line);
	while ((ret = get_next_line(map->fd, &line)) > 0 && line[0])
	{
		add_new_elem(map_lst, line);
		len = map_line_len(line);
		if (len > map->width)
			map->width = len;
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

static char		**create_map_arr(t_list **head, int height, int width)
{
	char	**map;
	t_list	*tmp;
	int		len;

	tmp = *head;
	if (!(map = malloc((height + 1) * sizeof(char *))))
		exit_error("Error\nCan not create map array");
	map[height] = NULL;
	while (tmp)
	{
		height--;
		if (!(map[height] = malloc(width + 1)))
			exit_error("Error\nCan not create map array");
		if ((len = ft_strlen(tmp->content)) > width)
			len = width;
		ft_memcpy(map[height], tmp->content, len);
		while (len < width)
			map[height][len++] = ' ';
		map[height][width] = '\0';
		tmp = tmp->next;
	}
	return (map);
}

void			get_cub_map(char *first_line, t_cub *cub)
{
	t_list	*map_lst;

	map_lst = NULL;
	create_map_lst(first_line, &map_lst, &cub->map);
	cub->map.height = ft_lstsize(map_lst);
	cub->map.map = create_map_arr(&map_lst, cub->map.height, cub->map.width);
	parse_map_1(cub);
	parse_map_2(&cub->map);
	ft_lstclear(&map_lst, free);
}
