#include "../../include/ft_printf.h"

/**
 * @brief Processes a format specifier for the bonus version of ft_printf.
 *
 * This function checks the specifier character (at position 1 of the passed string)
 * and calls the corresponding bonus print function with the provided options.
 *
 * @param str The format specifier string starting with '%' (e.g., "%c", "%s").
 * @param params Pointer to the variable argument list.
 * @param opt The options structure containing formatting flags.
 * @return int The total number of characters printed for this specifier.
 */
int process_spec_bonus(const char *str, va_list *params, t_opt opt)
{
    int total = 0;

    if (str[1] == 'c')
        total += print_char_bonus(va_arg(*params, int), opt);
    else if (str[1] == 's')
        total += print_str_bonus(va_arg(*params, char *), opt);
    else if (str[1] == 'p')
        total += print_ptr_bonus(va_arg(*params, void *), opt);
    else if (str[1] == 'd' || str[1] == 'i')
        total += print_number_bonus(va_arg(*params, int), opt);
    else if (str[1] == 'u')
        total += print_unumber_bonus(va_arg(*params, unsigned int), opt);
    else if (str[1] == 'x')
        total += print_nbrbase_bonus(va_arg(*params, int), 0, opt);
    else if (str[1] == 'X')
        total += print_nbrbase_bonus(va_arg(*params, int), 1, opt);
    else if (str[1] != '\0')
        total += print_char(str[1]);
    return (total);
}

/**
 * @brief Processes formatting flags in a format specifier.
 *
 * Reads the flag from the format string (starting at index 1) and sets the corresponding
 * fields in the options structure. Also moves the cursor pointer accordingly.
 *
 * @param opt Pointer to the options structure to update.
 * @param str The format string starting at the current specifier.
 * @param cur Pointer to the current index in the format string.
 */
void process_flags(t_opt *opt, const char *str, int *cur)
{
    if (str[1] == '#')
        opt->sharp = 1;
    else if (str[1] == ' ')
        opt->space = 1;
    else if (str[1] == '+')
        opt->plus = 1;
    else if (str[1] == '0')
    {
        opt->zero_offset = atoi_n_mov_ptr(str + 2, cur);
        opt->zero = 1;
    }
    else if (str[1] == '-')
    {
        opt->offset = atoi_n_mov_ptr(str + 2, cur);
        opt->minus = 1;
    }
    else if (str[1] == '.')
    {
        opt->precision = atoi_n_mov_ptr(str + 2, cur);
        opt->dot = 1;
    }
}

/**
 * @brief Processes a full format specifier with flags and width for the bonus version.
 *
 * Parses the format string starting at the current position, collecting all flags,
 * width, and precision values into an options structure. Then, it processes the actual
 * specifier and prints the formatted output.
 *
 * @param str The complete format string.
 * @param params Pointer to the variable argument list.
 * @param cur Pointer to the current index in the format string.
 * @return int The total number of characters printed for this specifier.
 */
static int process_selector(const char *str, va_list *params, int *cur)
{
    int total = 0;
    t_opt opt;

    init_opt(&opt);
    /* Process flags, width, and precision until a non-matching character is found */
    while (in_set(str[(*cur) + 1], "0123456789# +-."))
    {
        if (in_set(str[(*cur) + 1], "# +0-."))
            process_flags(&opt, str + (*cur), cur);
        else
        {
            opt.min_width = atoi_n_mov_ptr(str + (*cur) + 1, cur);
            (*cur)--;
        }
        (*cur)++;
    }
    total += process_spec_bonus(str + (*cur), params, opt);
    (*cur)++;
    return (total);
}

/**
 * @brief Processes a basic format specifier (without bonus flags).
 *
 * This function handles format specifiers for the standard version of ft_printf.
 * It increments the current position, checks the specifier, and calls the appropriate
 * printing function.
 *
 * @param str The format string starting with '%' (e.g., "%d", "%s").
 * @param params Pointer to the variable argument list.
 * @param cur Pointer to the current index in the format string.
 * @return int The total number of characters printed for this specifier.
 */
int process_spec(const char *str, va_list *params, int *cur)
{
    int total = 0;
    (*cur)++;  /* Move past the '%' */

    if (str[*cur] == 'c')
        total += print_char(va_arg(*params, int));
    else if (str[*cur] == 's')
        total += print_str(va_arg(*params, char *));
    else if (str[*cur] == 'p')
        total += print_ptr(va_arg(*params, void *));
    else if (str[*cur] == 'd' || str[*cur] == 'i')
        total += ft_putnbr_base(va_arg(*params, int), "0123456789");
    else if (str[*cur] == 'u')
        total += print_unumber(va_arg(*params, unsigned int));
    else if (str[*cur] == 'x')
        total += ft_putnbr_base(va_arg(*params, int), "0123456789abcdef");
    else if (str[*cur] == 'X')
        total += ft_putnbr_base(va_arg(*params, int), "0123456789ABCDEF");
    else
        total += print_char(str[*cur]);

    return (total);
}

/**
 * @brief Custom printf function that formats and prints data.
 *
 * This function mimics the standard printf behavior. It processes the format string,
 * handles conversion specifiers (with or without bonus formatting), and prints the output.
 * The BONUS macro determines whether bonus formatting is enabled.
 *
 * @param str The format string containing conversion specifiers.
 * @param ... Variable arguments to be formatted.
 * @return int The total number of characters printed, or -1 on error.
 */
int ft_printf(const char *str, ...)
{
    va_list params;
    int cur = -1;
    int total = 0;

    va_start(params, str);
    while (str[++cur])
    {
        if (str[cur] == '%')
        {
            if (str[cur + 1] == '\0')
                return (-1);
            if (!BONUS)
                total += process_spec(str, &params, &cur);
            if (BONUS)
                total += process_selector(str, &params, &cur);
            if (str[cur] == '\0')
                return (-1);
        }
        else
        {
            total += print_char(str[cur]);
        }
    }
    va_end(params);
    return (total);
}
