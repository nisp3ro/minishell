#include "../../include/get_next_line.h"

/**
 * @brief Joins two strings into a new string.
 *
 * This function concatenates the strings s1 and s2 into a new string.
 * If s1 is NULL, it is treated as an empty string.
 * The original s1 is freed after concatenation.
 *
 * @param s1 The first string. May be NULL.
 * @param s2 The second string.
 * @return char* A newly allocated string that is the concatenation of s1 and s2,
 *               or NULL if memory allocation fails.
 */
char *ft_strjoin_modded(char *s1, char *s2)
{
    char    *str;
    size_t  i;
    size_t  c;

    if (!s1)
    {
        s1 = malloc(sizeof(char) + 1);
        if (!s1)
            return (0);
        s1[0] = '\0';
    }
    str = (char *)malloc(sizeof(char) * (ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1));
    if (!str)
        return (ft_free(&s1));
    i = 0;
    while (s1[i])
    {
        str[i] = s1[i];
        i++;
    }
    c = 0;
    while (s2[c])
    {
        str[i + c] = s2[c];
        c++;
    }
    str[i + c] = '\0';
    free(s1);
    return (str);
}

/**
 * @brief Calculates the length of a null-terminated string.
 *
 * This function iterates through the string until the null terminator is encountered
 * and returns the number of characters.
 *
 * @param str The input string.
 * @return size_t The length of the string.
 */
size_t ft_strlen_gnl(const char *str)
{
    size_t len;

    len = 0;
    while (str[len])
        len++;
    return (len);
}

/**
 * @brief Locates the first occurrence of a character in a string.
 *
 * Searches for the character 'tofind' in the string 'str'. If found, a pointer
 * to the first occurrence is returned; otherwise, NULL is returned.
 *
 * @param str The string to search.
 * @param tofind The character to locate.
 * @return char* A pointer to the first occurrence of 'tofind' in 'str',
 *               or NULL if not found.
 */
char *ft_strchr_gnl(const char *str, int tofind)
{
    while (*str && *str != (char)tofind)
        str++;
    if (*str == (char)tofind)
        return ((char *)str);
    return (0);
}

/**
 * @brief Extracts a substring from a given string.
 *
 * Allocates and returns a substring from the string 's'. The substring begins at
 * index 'start' and is of maximum length 'len'. If 'start' is greater than the
 * length of 's', an empty string is returned. If the remaining characters in 's'
 * are less than 'len', the substring will contain only the available characters.
 *
 * @param s The source string.
 * @param start The starting index of the substring.
 * @param len The maximum length of the substring.
 * @return char* The allocated substring, or NULL if allocation fails.
 */
char *ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
    char    *subs;
    size_t  i;
    size_t  slen;

    i = 0;
    if (!s)
        return (NULL);
    slen = ft_strlen_gnl(s);
    if (start >= slen)
        len = 0;
    if ((slen - start) < len)
        len = slen - start;
    subs = (char *)malloc(sizeof(char) * (len + 1));
    if (subs == NULL)
        return (NULL);
    while ((start + i) < slen && i < len)
    {
        subs[i] = s[start + i];
        i++;
    }
    subs[i] = '\0';
    return (subs);
}
