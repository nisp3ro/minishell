#include "../../include/ft_printf.h"

/**
 * @brief Prints a string to the standard output.
 *
 * If the string pointer is NULL, the literal "(null)" is printed instead.
 *
 * @param str The null-terminated string to print.
 * @return int The number of characters printed.
 */
int print_str(char *str)
{
    int cur = 0;

    if (!str)
    {
        cur += print_str("(null)");
        return (cur);
    }
    while (str[cur])
        write(1, &str[cur++], 1);
    return (cur);
}

/**
 * @brief Prints a string up to a given size.
 *
 * If size is -1, the entire string is printed.
 *
 * @param str The null-terminated string to print.
 * @param size The maximum number of characters to print, or -1 to print the whole string.
 * @return int The number of characters printed.
 */
static int print_str_size(char *str, int size)
{
    int cur;

    if (size == -1)
        return (print_str(str));
    cur = 0;
    while (str[cur] && cur < size)
        write(1, &str[cur++], 1);
    return (cur);
}

/**
 * @brief Prints a string with bonus formatting options.
 *
 * This function prints a string taking into account minimum width and offset values
 * specified in the option structure. If the string pointer is NULL, the literal "(null)"
 * is printed. The precision (if dot flag is set) limits the number of characters printed.
 *
 * @param str The null-terminated string to print.
 * @param opt The options structure containing formatting options (min_width, offset, precision, dot, etc.).
 * @return int The total number of characters printed.
 */
int print_str_bonus(char *str, t_opt opt)
{
    int cur = 0;
    int len;

    if (!str)
        len = 6; /* Length of "(null)" */
    else
        len = ft_strlen_printf(str);
    
    /* Print leading spaces until reaching the minimum width */
    while (cur + len < opt.min_width)
        cur += print_char(' ');
    
    /* If no dot flag is set, disable precision by setting it to -1 */
    if (!opt.dot)
        opt.precision = -1;
    
    if (!str)
        cur += print_str_size("(null)", opt.precision);
    else
        cur += print_str_size(str, opt.precision);
    
    /* Print trailing spaces until reaching the offset value */
    while (cur < opt.offset)
        cur += print_char(' ');
    
    return (cur);
}

/**
 * @brief Prints a single character to the standard output.
 *
 * This function writes a character using the write system call.
 *
 * @param c The character to print.
 * @return int The number of characters printed (always 1 if successful).
 */
int print_char(char c)
{
    return (write(1, &c, 1));
}

/**
 * @brief Prints a character with bonus formatting options.
 *
 * This function prints leading spaces to reach the specified minimum width,
 * then prints the character, and finally prints trailing spaces until the total
 * output reaches the specified offset.
 *
 * @param c The character to print.
 * @param opt The options structure containing formatting options (min_width and offset).
 * @return int The total number of characters printed.
 */
int print_char_bonus(char c, t_opt opt)
{
    int total = 0;

    /* Print leading spaces until one character plus spaces reach min_width */
    while (total + 1 < opt.min_width)
        total += print_char(' ');
    
    total += print_char(c);
    
    /* Print trailing spaces until total printed characters reach offset */
    while (total < opt.offset)
        total += print_char(' ');
    
    return (total);
}
