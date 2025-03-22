#include "../../include/ft_printf.h"

/**
 * @brief Initializes the options structure for formatting.
 *
 * This function sets all the formatting flags and width/precision values
 * in the provided `t_opt` structure to their default (zero) state.
 *
 * @param opt Pointer to the options structure to initialize.
 */
void init_opt(t_opt *opt)
{
    opt->sharp = 0;
    opt->space = 0;
    opt->plus = 0;
    opt->minus = 0;
    opt->min_width = 0;
    opt->dot = 0;
    opt->precision = 0;
    opt->zero = 0;
    opt->zero_offset = 0;
    opt->offset = 0;
}

/**
 * @brief Calculates the length of a null-terminated string.
 *
 * Iterates through the given string until the null terminator is reached
 * and returns the number of characters in the string.
 *
 * @param str The null-terminated string.
 * @return int The length of the string. Returns 0 if the input is NULL.
 */
int ft_strlen_printf(char *str)
{
    int cur = 0;
    
    if (!str)
        return (cur);
    while (str[cur])
        cur++;
    return (cur);
}

/**
 * @brief Converts a numeric substring to an integer and moves the pointer.
 *
 * Parses the numeric characters from the beginning of the string, converts them
 * to an integer, and updates the current index pointer (`o_cur`) accordingly.
 *
 * @param str The string containing the numeric characters.
 * @param o_cur Pointer to the current index, which will be incremented as digits are parsed.
 * @return int The integer value represented by the numeric substring.
 */
int atoi_n_mov_ptr(const char *str, int *o_cur)
{
    int cur = 0;
    int num = 0;
    
    while (str[cur] >= '0' && str[cur] <= '9')
    {
        num = num * 10 + (str[cur] - '0');
        cur++;
        (*o_cur)++;
    }
    return (num);
}

/**
 * @brief Checks if a character is present in a given set.
 *
 * Iterates through the null-terminated string `set` to determine whether
 * the character `c` is present. Returns 1 if found, otherwise returns 0.
 *
 * @param c The character to search for.
 * @param set The null-terminated string containing the set of characters.
 * @return int Returns 1 if `c` is found in `set`, 0 otherwise.
 */
int in_set(char c, char *set)
{
    int cur = 0;
    
    while (set[cur])
    {
        if (set[cur] == c)
            return (1);
        cur++;
    }
    return (0);
}
