#include "../../include/libft.h"

/*
 * ft_strcat - Concatenates two strings.
 *
 * @dest: Pointer to the destination string, which should be a C-string that has enough
 *        space allocated to hold the concatenated resulting string.
 * @src:  Pointer to the source string to append to dest.
 *
 * This function appends the string pointed to by src to the end of the string pointed to by dest.
 * It first finds the terminating null character of dest, then copies each character from src,
 * and finally adds a null terminator to the concatenated string.
 *
 * If dest is NULL, the function returns NULL.
 * If src is NULL, no characters are appended to dest.
 *
 * Returns:
 *   A pointer to the resulting string dest.
 *
 * Example usage:
 *   char buffer[50] = "Hello, ";
 *   char *src = "world!";
 *   ft_strcat(buffer, src);
 *   // buffer now contains "Hello, world!"
 */
char *ft_strcat(char *dest, char *src)
{
    int i = 0;
    int j = 0;

    // Check if the destination string is valid.
    if (!dest)
        return (NULL);

    // Find the end of the destination string.
    while (dest[i] != '\0')
        i++;

    // Append the source string to the destination string if src is not NULL.
    if (src)
    {
        while (src[j] != '\0')
        {
            dest[i + j] = src[j];
            j++;
        }
    }

    // Null-terminate the resulting string.
    dest[i + j] = '\0';

    return (dest);
}
