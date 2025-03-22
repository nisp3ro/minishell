#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# ifndef BONUS
#  define BONUS 1
# endif

# include <stdarg.h>
# include <unistd.h>

// Mandatory
int		ft_printf(const char *str, ...);
int		print_char(char c);
int		print_str(char *str);
int		print_ptr(void *ptr);
int		print_unumber(unsigned long nl);
int		ft_putnbr_base(long nbr, char *base);

// Utils
int		ft_strlen_printf(char *str);
int		atoi_n_mov_ptr(const char *str, int *o_cur);
int		in_set(char c, char *set);

// Bonus
typedef struct s_opt
{
	int	sharp;
	int	space;
	int	plus;
	int	min_width;
	int	minus;
	int	dot;
	int	precision;
	int	offset;
	int	zero;
	int	zero_offset;
}		t_opt;

int		print_char_bonus(char c, t_opt opt);
int		print_str_bonus(char *str, t_opt opt);
int		print_ptr_bonus(void *ptr, t_opt opt);
int		print_number_bonus(long nl, t_opt opt);
int		print_unumber_bonus(unsigned long num, t_opt opt);
int		print_nbrbase_bonus(int nbr, int mayus, t_opt opt);

// Bonus Utils
void	init_opt(t_opt *opt);

#endif
