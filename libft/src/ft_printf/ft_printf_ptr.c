#include "../../include/ft_printf.h"

/**
 * @brief Recursively prints an unsigned long integer as a hexadecimal string.
 *
 * This function prints the hexadecimal representation of the address.
 * It converts each digit to the corresponding character ('0'-'9', 'a'-'f')
 * and writes it to the standard output.
 *
 * @param addr The unsigned long integer (address) to print.
 * @return int The number of characters printed.
 */
static int print_long_as_hex(long unsigned addr)
{
    char c;
    int total = 0;

    if (addr >= 16)
    {
        total += print_long_as_hex(addr / 16);
        total += print_long_as_hex(addr % 16);
    }
    else
    {
        c = (addr % 16) + '0';
        if ((addr % 16) > 9)
            c = ((addr % 16) + 87);  /* 87 = 'a' - 10 */
        write(1, &c, 1);
        total++;
    }
    return (total);
}

/**
 * @brief Recursively calculates the number of hexadecimal digits required to represent an address.
 *
 * This function returns the count of hexadecimal digits needed to represent the given unsigned long address.
 *
 * @param addr The unsigned long address.
 * @return int The number of hexadecimal digits.
 */
static int get_hex_size(long unsigned addr)
{
    int total = 0;

    if (addr >= 16)
    {
        total += get_hex_size(addr / 16);
        total += get_hex_size(addr % 16);
    }
    else
        total++;
    return (total);
}

/**
 * @brief Prints a pointer with bonus formatting options.
 *
 * This function prints a pointer in hexadecimal format with a "0x" prefix.
 * If the pointer is NULL, it prints "(nil)". It also applies bonus formatting:
 * leading spaces are added until the minimum width (opt.min_width) is reached,
 * and trailing spaces are printed until the total output reaches opt.offset.
 *
 * @param ptr The pointer to print.
 * @param opt The formatting options structure.
 * @return int The total number of characters printed.
 */
int print_ptr_bonus(void *ptr, t_opt opt)
{
    long addr;
    int total = 0;
    int len;

    addr = (long unsigned)ptr;
    len = get_hex_size(addr) + 2;  /* +2 for "0x" prefix */
    
    /* Print leading spaces until minimum width is met */
    while (len + total < opt.min_width)
        total += print_char(' ');
    
    if (!ptr)
    {
        total += print_str("(nil)");
        while (total < opt.offset)
            total += print_char(' ');
        return (total);
    }
    
    total += print_str("0x");
    total += print_long_as_hex(addr);
    
    /* Print trailing spaces until the offset is reached */
    while (total < opt.offset)
        total += print_char(' ');
    
    return (total);
}

/**
 * @brief Prints a pointer in hexadecimal format.
 *
 * This function prints a pointer value in the standard hexadecimal notation with a "0x" prefix.
 * If the pointer is NULL, it prints "(nil)".
 *
 * @param ptr The pointer to print.
 * @return int The number of characters printed.
 */
int print_ptr(void *ptr)
{
    long addr;
    int total = 0;

    addr = (long unsigned)ptr;
    if (!ptr)
    {
        total += print_str("(nil)");
        return (total);
    }
    total += print_str("0x");
    total += print_long_as_hex(addr);
    return (total);
}
