#include "../../include/ft_printf.h"

/**
 * @brief Prints a minus sign for negative numbers and adjusts the number and length.
 *
 * This function writes the '-' character to the output, converts the number to its
 * absolute value, and adjusts the length variables accordingly. If the precision flag
 * is set (opt.dot), an additional adjustment is performed.
 *
 * @param nl Pointer to the number to be printed. It is converted to its absolute value.
 * @param len Pointer to the length of the number (number of digits). Decremented as needed.
 * @param opt The options structure containing formatting flags.
 * @return int Always returns 1 (the number of characters printed for the minus sign).
 */
static int print_minus(long *nl, int *len, t_opt opt)
{
    print_char('-');
    *nl = *nl * -1;
    (*len)--;
    if (opt.dot)
        (*len)--;
    return (1);
}

/**
 * @brief Counts the number of digits in a number.
 *
 * This function returns the total number of digits required to represent the given number
 * in base 10. A negative number increases the count by 1 (for the minus sign).
 *
 * @param num The number whose digits are to be counted.
 * @return int The number of digits (including the sign for negatives).
 */
static int number_of_digit(long num)
{
    int cur = 0;
    
    if (num == 0)
        return (1);
    if (num < 0)
        cur++;  /* For the '-' sign */
    while (num != 0)
    {
        num = num / 10;
        cur++;
    }
    return (cur);
}

/**
 * @brief Computes formatting values for decimal output.
 *
 * Calculates the original number length, adjusts it for precision, and determines the
 * effective length to be used for padding. It also chooses the prefix padding character
 * based on formatting options and updates the minimum width if necessary.
 *
 * @param prefix Pointer to a character that will store the chosen padding prefix ('0' or ' ').
 * @param len_prec Pointer to an integer that will store the adjusted length considering precision.
 * @param opt Pointer to the options structure containing formatting settings.
 * @param nl The number to be printed.
 * @return int The original length (in digits) of the number.
 */
static int get_values(char *prefix, int *len_prec, t_opt *opt, long nl)
{
    int len = number_of_digit(nl);
    *len_prec = len;
    
    if (opt->precision > len)
        *len_prec = opt->precision;
    
    if (nl < 0 && opt->zero_offset > len && opt->precision > len)
        (*len_prec)++;
    
    if (nl < 0 && opt->dot && opt->precision < opt->zero_offset)
        len++;
    
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
 * @brief Prints the decimal number using base "0123456789".
 *
 * This helper function handles special cases when printing zero with various formatting
 * options. Otherwise, it prints the number using ft_putnbr_base.
 *
 * @param len The original length (number of digits) of the number.
 * @param nl The (possibly adjusted) number to be printed.
 * @param opt The formatting options structure.
 * @return int The number of characters printed.
 */
static int print_di(int len, long nl, t_opt opt)
{
    int total = 0;
    
    if (nl == 0 && opt.min_width && opt.min_width < len)
        total += print_char(' ');
    else if (nl == 0 && opt.dot && !opt.precision && opt.min_width >= len)
        total += print_char(' ');
    else if (!(nl == 0 && opt.dot && !opt.precision))
        total += ft_putnbr_base(nl, "0123456789");
    
    return (total);
}

/**
 * @brief Prints a number in decimal format with bonus formatting options.
 *
 * This function applies bonus formatting options such as minimum width, precision,
 * zero-padding, space, plus, and sharp flags. It first calculates the number of digits
 * and adjusts for precision. If the number is negative, it prints the '-' sign. For
 * positive numbers, it may print a leading space or plus sign based on the options.
 * It then prints leading padding, the number itself, and trailing spaces until the
 * total printed characters reach the specified offset.
 *
 * @param nl The number to be printed.
 * @param opt The options structure containing formatting flags (min_width, precision, offset, etc.).
 * @return int The total number of characters printed.
 */
int print_number_bonus(long nl, t_opt opt)
{
    int total = 0;
    int len;
    char prefix;
    int len_prec;
    
    /* Compute original length and adjust formatting values */
    len = get_values(&prefix, &len_prec, &opt, nl);
    
    /* Print leading padding using the prefix character if it's a space */
    while (prefix == ' ' && len_prec + total < opt.min_width)
        total += print_char(' ');
    
    /* If the number is negative, print the minus sign and adjust the number */
    if (nl < 0)
        total += print_minus(&nl, &len, opt);
    else if (opt.space && nl >= 0 && !opt.plus && !opt.dot)
        total += print_char(' ');
    else if (opt.plus && nl >= 0 && !opt.dot)
        total += print_char('+');
    
    /* Print additional zero padding until total printed digits match min_width */
    while (len + total < opt.min_width)
        total += print_char('0');
    
    /* Print the number (or special cases for zero) */
    total += print_di(len, nl, opt);
    
    /* Print trailing spaces until total printed characters reach the offset value */
    while (total < opt.offset)
        total += print_char(' ');
    
    return (total);
}
