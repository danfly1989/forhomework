/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:13:24 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/12 20:49:39 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>

typedef struct s_game
{
	void	*mlx;
	void	*win;
	void	*player_img;
	void	*wall_img;
	void	*exit_img;
	void	*collectible_img;
	char	**map;
	int		player_x;
	int		player_y;
	int		map_width;
	int		map_height;
	int		img_width;
	int		img_height;
	int		total_collectibles;
	int		collected;
	int		player_count;
	int		exit_count;
	int		move_count;

}			t_game;

# define WIDTH 1200
# define HEIGHT 1000
# define TITLE "so_long - window"
# define WINDOW_CLOSE_EVENT 17
# define TILE_SIZE 32
# define KEY_ESC 65307
# define KEYPRESS 2

char		**load_map(const char *filename, t_game *game);
void		find_player_start(t_game *game);
void		render_map(t_game *game);
int			key_press(int keycode, void *game_ptr);
void		draw_player(t_game *game);
void		clear_position(t_game *game, int x, int y);
int			load_and_print_map(const char *filename, t_game *game);
int			validate_args(t_game *game, int argc, char **argv);
void		draw_tile(t_game *game, int *x, int *y);
int			load_sprites(t_game *game);

// validate map has its own file. The helpers are in validation.c
int			validate_map(t_game *game, char *filename);
int			verify_uniques(t_game *game);
int			verify_collectibles(t_game *game);
int			valid_char_only(t_game *game);
int			is_rectangle(int fd);
int			walled_in(t_game *game);
void		free_and_exit(t_game *game);
void		free_map(char **map);

void		game_init(t_game *game);

#endif
