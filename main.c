/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerelo <amerelo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/04 15:01:57 by amerelo           #+#    #+#             */
/*   Updated: 2015/12/18 14:56:10 by matirell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>

int	check_block_plus(char *str, int x)
{
	if (str[x - 1] == '#' && str[x - 5] == '#')
		return (1);
	else if (str[x + 1] == '#' && str[x - 5] == '#')
		return (1);
	else if (str[x + 1] == '#' && str[x + 5] == '#')
		return (1);
	else if (str[x - 1] == '#' && str[x + 5] == '#')
		return (1);
	else if (str[x - 1] == '#' && str[x + 1] == '#')
		return (1);
	else if (str[x - 5] == '#' && str[x + 5] == '#')
		return (1);
	return (0);
}

int	check_block(char *str)
{
	int x;
	int count_chars;
	int count_diezes;
	int t;

	t = 0;
	x = -1;
	count_chars = 0;
	count_diezes = 0;
	while (str[++x])
	{
		if ((x == 9 || x == 14 || x == 19) && str[x - 5] != '\n')
			return (0);
		else if (str[x] == '#' && (str[x - 1] == '#' || str[x + 1] == '#' ||
		str[x + 5] == '#' || str[x - 5] == '#'))
			count_diezes++;
		if (str[x] == '#' && check_block_plus(str, x) && !t--)
			count_diezes++;
		if (str[x] == '.')
			count_chars++;
	}
	if (count_chars == 12 && count_diezes == 5)
		return (1);
	return (0);
}

void	ft_puttab(char **tab)
{
	int x;
	int y;

	y = 0;
	while (tab[y])
	{
		x = 0;
		while (tab[y][x])
		{
			ft_putchar(tab[y][x]);
			x++;
		}
		y++;
	}
	ft_putchar('\n');
}

int	power_x(int x)
{
	int y;
	int tmp;

	y = x;
	tmp = 1;
	while (tmp < y)
	{
		x += y;
		tmp++;
	}
	return (x);
}

char	**create_tableau(int size)
{
	int		size_cube;
	int		x;
	int		y;
	int		len;
	char	**final_tab;

	size_cube = 2;
	while (power_x(size_cube) < (size * 4))
		size_cube++;
	final_tab = (char **)ft_memalloc(sizeof(char *) * (size_cube + 1));
	x = 0;
	while (x < size_cube)
	{
		final_tab[x] = (char *)ft_memalloc(sizeof(char ) * (size_cube + 2));
		y = 0;
		while (y < size_cube)
		{
			final_tab[x][y] = '.';
			y++;
		}
		final_tab[x][y] = '\n';
		x++;
	}
	return (final_tab);
}

t_piece	*cut_tetri_plus(t_piece *p)
{
	int xtmp;
	int ytmp;

	ytmp = p->y;
	xtmp = p->x;
	while (p->next != NULL)
	{
		ytmp = (ytmp > p->y ? p->y : ytmp);
		xtmp = (xtmp > p->x ? p->x : xtmp);
		p = p->next;
	}
	ytmp = (ytmp > p->y ? p->y : ytmp);
	xtmp = (xtmp > p->x ? p->x : xtmp);
	while (p->prev != NULL)
	{
		p->y -= ytmp;
		p->x -= xtmp;
		p = p->prev;
	}
	p->y -= ytmp;
	p->x -= xtmp;
	return (p);
}

t_piece	*cut_tetri(char *tetri)
{
	int			x;
	t_piece		*p;
	t_piece		*tmp;

	p = NULL;
	x = 0;
	while (tetri[x] != '\0')
	{
		if (tetri[x] == '#')
		{
			tmp = (t_piece *)malloc(sizeof(t_piece));
			tmp->x = x % 5;
			tmp->y = x / 5;
			tmp->next = NULL;
			tmp->prev = p;
			p = (p == NULL ? tmp : p);
			p->next = (p == NULL ? NULL : tmp);
			p = (p == NULL ? p : p->next);
		}
		x++;
	}
	while (p->prev)
		p = p->prev;
	p = cut_tetri_plus(p);
	return (p);
}

t_tetri	*add_on_list(t_tetri *head, t_tetri *l_tmp, int x)
{
	t_tetri *tmp;

	if (x != 0)
	{
		tmp = head;
		while (head->next != NULL)
			head = head->next;
		head->next = (t_tetri *)malloc(sizeof(t_tetri));
		head->next = l_tmp;
		return (tmp);
	}
	else
		head = l_tmp;
	return (head);
}

t_tetri	*create_list_element(char *tetriminos, int x)
{
	t_tetri *l_tmp;

	l_tmp = (t_tetri *)malloc(sizeof(t_tetri));
	if (!l_tmp)
		return (NULL);
	l_tmp->p = cut_tetri(tetriminos);
	l_tmp->tetri_nuber = x;
	l_tmp->is_valid = 0;
	l_tmp->next = NULL;
	return (l_tmp);
}


int	test_2(t_tetri *mainlist, char **final_carre, int y, int x)
{
	while (mainlist->p->next != NULL)
	{
		if (final_carre[y + mainlist->p->y] && final_carre
			[y + mainlist->p->y][x + mainlist->p->x] == '.')
			mainlist->p = mainlist->p->next;
		else
		{
			while (mainlist->p->prev)
				mainlist->p = mainlist->p->prev;
			return (0);
		}
	}
	if (final_carre[y + mainlist->p->y] && final_carre
		[y + mainlist->p->y][x + mainlist->p->x] == '.')
	{
		while (mainlist->p->prev)
			mainlist->p = mainlist->p->prev;
		return (1);
	}
	else
	{
		while (mainlist->p->prev)
			mainlist->p = mainlist->p->prev;
		return (0);
	}
}

void	ft_place(t_tetri *mainlist, char **final_carre, int y, int x)
{
	mainlist->is_valid = 1;
	while (mainlist->p->next != NULL)
	{
		final_carre[y + mainlist->p->y][x + mainlist->p->x] =
		mainlist->tetri_nuber + 'A';
		mainlist->p = mainlist->p->next;
	}
	final_carre[y + mainlist->p->y][x + mainlist->p->x] =
	mainlist->tetri_nuber + 'A';
	while (mainlist->p->prev)
		mainlist->p = mainlist->p->prev;
}

void	ft_unplace(t_tetri *mainlist, char **final_carre, int y, int x)
{
	mainlist->is_valid = 0;
	while (mainlist->p->next != NULL)
	{
		final_carre[y + mainlist->p->y][x + mainlist->p->x] = '.';
		mainlist->p = mainlist->p->next;
	}
	final_carre[y + mainlist->p->y][x + mainlist->p->x] = '.';
	while (mainlist->p->prev)
		mainlist->p = mainlist->p->prev;
}

int	test_isvalid(t_tetri *tmp)
{
	while (tmp != NULL)
	{
		if (tmp->is_valid == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

int ft_solve_tetri_plus(t_tetri *mainlist, char **final_carre, int y, int x)
{
	t_tetri *tmp;

	if ((tmp = mainlist) != NULL && test_isvalid(tmp))
		return (1);
	while (tmp != NULL)
	{
		while (final_carre[y])
		{
			if (tmp->is_valid == 0 && test_2(tmp, final_carre, y, x))
			{
				ft_place(tmp, final_carre, y, x);
				if (ft_solve_tetri_plus(mainlist, final_carre, y, x))
					return (1);
				else
					ft_unplace(tmp, final_carre, y, x);
			}
			x = (final_carre[y][x] == '\n' && final_carre[y]) ? -1 : x + 1;
			y = (final_carre[y][x] == '\n' && final_carre[y]) ? y + 1 : y;
		}
		x = 0;
		y = 0;
		tmp = tmp->next;
	}
	return (0);
}

int	parcour_tableau(t_tetri *mainlist, char **final_carre, int y, int x)
{
	while (final_carre[y][x] != '.' && final_carre[y])
	{
		if (final_carre[y][x] == '\n' && final_carre[y])
		{
			x = 0;
			y++;
		}
		x++;
	}
	if (ft_solve_tetri_plus(mainlist, final_carre, y, x))
		return (1);
	return (0);
}

int	ft_solve_tetri(t_tetri *mainlist, int size)
{
	int x;
	int y;
	char **final_carre;
	t_tetri *tmp;
	t_tetri *tmp2;

	x = 0;
	y = 0;
	tmp = mainlist;
	final_carre = create_tableau(size);
	if (ft_solve_tetri_plus(mainlist, final_carre, y, x))
		ft_puttab(final_carre);
	else
	{
		free(final_carre);
		while (mainlist)
		{
			mainlist->is_valid = 0;
			mainlist = mainlist->next;
		}
		return (ft_solve_tetri(tmp, (size + 1)));
	}
	return (1);
}

int		ft_check_tetri(char *str)
{
	t_tmp	u;

	u.x = 0;
	u.y = 0;
	while (str[u.x])
	{
		if (!(u.tmp = (char *)ft_memalloc(sizeof(char) * 21)))
			return (0);
		u.i = 0;
		while (u.i < 20 && str[u.i + u.x] && (str[u.i + u.x] == '.' ||
		 str[u.i + u.x] == '#' || str[u.i + u.x] == '\n'))
		{
			u.tmp[u.i] = str[u.i + u.x];
			u.i++;
		}
		u.x += u.i;
		if ((u.i != 20 || !(check_block(u.tmp))) || \
		(str[u.x] == '\n' && str[u.x + 1] != '.' && str[u.x + 1] != '#'))
			return (0);
		u.l_tmp = create_list_element(u.tmp, u.y);
		u.head = add_on_list(u.head	, u.l_tmp, u.y);
		u.x++;
		u.y++;
	}
	return (ft_solve_tetri(u.head, u.y));
}

int		main(int ac, char **av)
{
	int		fi;
	char	buf[1025];
	int		r;

	if (ac == 2)
	{
		fi = open(av[1], O_RDONLY);
		if (fi > 0)
		{
			r = read(fi, buf, 1024);
			buf[r] = '\0';
			if (r < 1 || !ft_check_tetri(buf))
			{
				ft_putendl("error");
				close(fi);
				return (-1);
			}
			close(fi);
		}
		else
			ft_putendl("error");
	}
	else
		ft_putendl("error");
	return (0);
}
