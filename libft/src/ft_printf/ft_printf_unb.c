#include "../../include/ft_printf.h"

/**
 * @brief Recursively prints an unsigned long number in decimal.
 *
 * This function prints the digits of the given unsigned long number in base 10.
 *
 * @param nl The unsigned long number to print.
 * @return int The number of characters printed.
 */
int print_unumber(unsigned long nl)
{
    int total = 0;
    
    if (nl >= 10)
        total += print_unumber(nl / 10);
    total += print_char((nl % 10) + '0');
    return (total);
}

/**
 * @brief Calculates the number of digits in an unsigned long number.
 *
 * This function returns the number of digits required to represent the number in base 10.
 *
 * @param num The unsigned long number.
 * @return int The number of digits.
 */
static int number_of_digit(unsigned long num)
{
    int cur = 0;
    
    if (num == 0)
        return (1);
    while (num != 0)
    {
        num = num / 10;
        cur++;
    }
    return (cur);
}

/**
 * @brief Computes formatting values for printing an unsigned number.
 *
 * This function calculates the original digit count of the number and adjusts the 
 * effective length based on the precision option. It also selects a prefix character 
 * for padding ('0' or space) and updates the minimum width if needed.
 *
 * @param prefix Pointer to a character that will hold the chosen padding character.
 * @param lp Pointer to an integer that will store the effective length after precision is applied.
 * @param opt Pointer to the options structure containing formatting settings.
 * @param num The unsigned long number to be printed.
 * @return int The original number of digits in the number.
 */
static int get_values(char *prefix, int *lp, t_opt *opt, unsigned long num)
{
    int len = number_of_digit(num);
    *lp = len;
    
    if (opt->precision > len)
        *lp = opt->precision;
    
    *prefix = '0';
    if (opt->zero && opt->dot && opt->zero_offset > opt->precision)
        *prefix = ' ';
    if (!opt->zero)
        *prefix = ' ';
    if (opt->zero)
        opt->min_width = opt->zero_offset;
    if (opt->precision > opt->min_width)
        opt->min_width = opt->precision;
    
    return (len);
}

/**
 * @brief Prints the unsigned number according to the basic rules.
 *
 * Handles special cases when the number is zero and the precision or minimum width 
 * formatting options are set. Otherwise, prints the number using print_unumber.
 *
 * @param len The original number of digits.
 * @param num The unsigned long number to print.
 * @param opt The formatting options structure.
 * @return int The number of characters printed.
 */
static int print_u(int len, unsigned long num, t_opt opt)
{
    int total = 0;
    
    if (num == 0 && opt.min_width && opt.min_width < len)
        total += print_char(' ');
    else if (num == 0 && opt.dot && !opt.precision && opt.min_width >= len)
        total += print_char(' ');
    else if (!(num == 0 && opt.dot && !opt.precision))
        total += print_unumber(num);
    
    return (total);
}

/**
 * @brief Prints an unsigned number in decimal with bonus formatting options.
 *
 * This function applies bonus formatting options such as minimum width, precision, 
 * and offset for an unsigned number. It first computes the number of digits and 
 * adjusts for precision, then prints any necessary leading padding using the chosen 
 * prefix character, prints extra zero-padding if required, and finally prints the number.
 * Trailing spaces are added until the total printed characters reach the offset value.
 *
 * @param num The unsigned long number to print.
 * @param opt The formatting options structure (including min_width, precision, zero_offset, offset, etc.).
 * @return int The total number of characters printed.
 */
int print_unumber_bonus(unsigned long num, t_opt opt)
{
    int total = 0;
    int len;
    int len_prec;
    char prefix;
    
    len = get_values(&prefix, &len_prec, &opt, num);
    
    /* Print leading padding until effective length reaches min_width */
    while (len_prec + total < opt.min_width)
        total += print_char(prefix);
    
    /* Print extra zero padding if needed */
    while (len + total < opt.min_width)
        total += print_char('0');
    
    /* Print the unsigned number */
    total += print_u(len, num, opt);
    
    /* Print trailing spaces until the overall printed width reaches the offset */
    while (total < opt.offset)
        total += print_char(' ');
    
    return (total);
}
