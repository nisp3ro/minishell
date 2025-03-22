#include "../../include/ft_printf.h"

/**
 * @brief Checks if a given base string is valid.
 *
 * A valid base must have at least two characters and must not contain '+' or '-'
 * or any duplicate characters.
 *
 * @param base The base string to validate.
 * @return int Returns 1 if the base is valid, otherwise returns 0.
 */
static int is_valid_base(char *base)
{
    int i;
    int j;

    if (!base || ft_strlen_printf(base) < 2)
        return (0);
    i = 0;
    while (base[i])
    {
        if (base[i] == '+' || base[i] == '-')
            return (0);
        j = i + 1;
        while (base[j])
        {
            if (base[i] == base[j])
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

/**
 * @brief Recursively prints an unsigned long number in the given base.
 *
 * This function prints the number represented by 'nbru' in the numeral system
 * defined by the provided 'base' string. It works recursively by dividing the number
 * and printing each digit.
 *
 * @param nbru The unsigned long number to print.
 * @param base The base string used for conversion (e.g., "0123456789ABCDEF").
 * @return int The total number of characters printed.
 */
static int ft_putunbr_base(unsigned long nbru, char *base)
{
    unsigned long base_len;
    int total;

    total = 0;
    base_len = ft_strlen_printf(base);
    if (nbru >= base_len)
    {
        total += ft_putunbr_base(nbru / base_len, base);
        total += ft_putunbr_base(nbru % base_len, base);
    }
    else
        total += print_char(base[nbru]);
    return (total);
}

/**
 * @brief Prints a number in a given base.
 *
 * This function prints the integer 'nbr' in the numeral system defined by 'base'.
 * For decimal (base 10), it handles negative numbers by printing a '-' sign and converting
 * the number to its absolute value. For non-decimal bases, the number is cast to an unsigned
 * integer to print its two's complement representation.
 *
 * @param nbr The number to print.
 * @param base The base string for conversion (e.g., "0123456789" for decimal,
 *             "0123456789abcdef" for lowercase hexadecimal).
 * @return int The total number of characters printed. Returns 0 if the base is invalid.
 */
int ft_putnbr_base(long nbr, char *base)
{
    unsigned long base_len;
    unsigned long nbru;
    int total;

    if (!is_valid_base(base))
        return (0);
    base_len = ft_strlen_printf(base);
    total = 0;
    if (nbr < 0)
    {
        /* For decimal base, print '-' and work with the absolute value */
        if (base_len == 10)
        {
            total += print_char('-');
            nbru = (unsigned long)(-nbr);
        }
        /* For non-decimal bases, treat the number as unsigned */
        else
            nbru = (unsigned int)nbr;
    }
    else
        nbru = (unsigned long)nbr;
    total += ft_putunbr_base(nbru, base);
    return (total);
}
