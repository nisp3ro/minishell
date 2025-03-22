#include "../../include/ft_printf.h"

/**
 * @brief Recursively calculates the number of hexadecimal digits needed to represent a number.
 *
 * The function converts the given integer to an unsigned value and computes
 * the total number of hexadecimal digits required.
 *
 * @param nbr The integer whose hexadecimal digit count is to be determined.
 * @return int The number of hexadecimal digits.
 */
static int get_hex_size(int nbr)
{
    int total = 0;
    unsigned int nbrl = nbr;

    if (nbrl >= 16)
    {
        total += get_hex_size(nbrl / 16);
        total += get_hex_size(nbrl % 16);
    }
    else
        total++;
    return (total);
}

/**
 * @brief Determines formatting parameters for hexadecimal output.
 *
 * This function calculates the basic length (number of digits) of the hexadecimal
 * representation and adjusts the effective length based on the precision option.
 * It also sets the prefix character and updates the minimum width in the options structure.
 *
 * @param prefix Pointer to a character that will store the chosen padding prefix ('0' or ' ').
 * @param len_prec Pointer to an integer that will store the adjusted length considering precision.
 * @param opt Pointer to the options structure containing formatting settings.
 * @param nbr The number to be printed.
 * @return int The original length (in digits) of the hexadecimal representation.
 */
static int get_values(char *prefix, int *len_prec, t_opt *opt, int nbr)
{
    int len = get_hex_size(nbr);
    *len_prec = len;
    if (opt->precision > len)
        *len_prec = opt->precision;
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
 * @brief Prints the hexadecimal representation of a number.
 *
 * Depending on the uppercase flag, this function prints the number in hexadecimal format
 * using either uppercase or lowercase letters. It also handles special cases for zero based
 * on the formatting options.
 *
 * @param mayus If non-zero, prints using uppercase letters; otherwise, lowercase.
 * @param len The original number of hexadecimal digits.
 * @param nbr The number to be printed.
 * @param opt The formatting options structure.
 * @return int The number of characters printed.
 */
static int print_x(int mayus, int len, int nbr, t_opt opt)
{
    int total = 0;
    
    if (nbr == 0 && opt.min_width && opt.min_width < len)
        total += print_char(' ');
    else if (nbr == 0 && opt.dot && !opt.precision && opt.min_width >= len)
        total += print_char(' ');
    else if (!(nbr == 0 && opt.dot && !opt.precision))
    {
        if (mayus)
            total += ft_putnbr_base(nbr, "0123456789ABCDEF");
        else
            total += ft_putnbr_base(nbr, "0123456789abcdef");
    }
    return (total);
}

/**
 * @brief Prints a number in hexadecimal format with bonus formatting options.
 *
 * Applies bonus formatting options such as minimum width, precision, and prefix flags.
 * If the sharp flag is set and the number is non-zero, a "0x" or "0X" prefix is printed.
 * Leading padding, zero padding, and trailing spaces are added according to the options.
 *
 * @param nbr The number to be printed.
 * @param mayus If non-zero, prints the hexadecimal digits in uppercase; otherwise, in lowercase.
 * @param opt The formatting options structure containing width, precision, and flags.
 * @return int The total number of characters printed.
 */
int print_nbrbase_bonus(int nbr, int mayus, t_opt opt)
{
    int total = 0;
    int len;
    char prefix;
    int len_prec;

    /* Calculate digit count and adjust formatting values */
    len = get_values(&prefix, &len_prec, &opt, nbr);

    /* Print leading padding characters (prefix may be '0' or space) */
    while (len_prec + total < opt.min_width)
        total += print_char(prefix);

    /* Print additional zeros if required */
    while (len + total < opt.min_width)
        total += print_char('0');

    /* Print sharp prefix ("0x" or "0X") if the flag is set and number is non-zero */
    if (opt.sharp && nbr != 0)
    {
        if (mayus)
            total += print_str("0X");
        else
            total += print_str("0x");
    }
    
    /* Print the hexadecimal number */
    total += print_x(mayus, len, nbr, opt);

    /* Print trailing spaces until the total printed characters reach the offset value */
    while (total < opt.offset)
        total += print_char(' ');

    return (total);
}
