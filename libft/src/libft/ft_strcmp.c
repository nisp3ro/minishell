#include "../../include/libft.h"

/*
 * ft_strcmp - Compare two strings.
 *
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 *
 * This function compares the strings 's1' and 's2' lexicographically.
 * The comparison is done character by character until a difference is found
 * or until the end of one of the strings is reached.
 *
 * Returns:
 *   0 if both strings are equal.
 *   A positive value if the first differing character in 's1' has a greater
 *     ASCII value than the corresponding character in 's2'.
 *   A negative value if the first differing character in 's1' has a lower
 *     ASCII value than the corresponding character in 's2'.
 *
 * Example usage:
 *   char *str1 = "Hello";
 *   char *str2 = "Hello World";
 *   int result = ft_strcmp(str1, str2);
 *
 *   // result will be negative because "Hello" is less than "Hello World"
 */
int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    
    // Compare characters while they are equal and not at the end of either string.
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
        i++;
    
    // Return the difference between the first pair of differing characters.
    return (s1[i] - s2[i]);
}
