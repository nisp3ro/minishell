#include "../../include/libft.h"

/**
 * @brief Frees an array of strings.
 *
 * Frees each allocated string in the array `res` up to index `n - 1`, then frees the array.
 *
 * @param res The array of strings to free.
 * @param n The number of allocated strings in the array.
 */
static void	ft_freeall(char **res, int n)
{
	int i = 0;
	while (i < n)
		free(res[i++]);
	free(res);
}

/**
 * @brief Counts words separated by a given delimiter.
 *
 * Iterates through the string `str` and counts words, where a word is a sequence of characters
 * not equal to the delimiter `c`.
 *
 * @param str The input string.
 * @param c The delimiter character.
 * @return int The number of words found.
 */
static int	ft_wordcounter(char const *str, char c)
{
	int words = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
		{
			words++;
			while (*str && *str != c)
				str++;
		}
	}
	return (words);
}

/**
 * @brief Splits the string into words and allocates each word.
 *
 * Uses ft_substr to allocate and copy each word from `str` (separated by delimiter `c`)
 * into the array `res`. If allocation fails for any word, frees all previously allocated words.
 *
 * @param res The array to store allocated words.
 * @param str The input string.
 * @param c The delimiter character.
 * @param words The total number of words expected.
 * @return int Returns 1 on success, or 0 on memory allocation failure.
 */
static int	ft_splitter(char **res, char const *str, char c, int words)
{
	int w = 0;
	int len;

	while (w < words)
	{
		while (*str == c)
			str++;
		len = 0;
		while (str[len] && str[len] != c)
			len++;
		res[w] = ft_substr(str, 0, len);
		if (!res[w])
		{
			ft_freeall(res, w);
			return (0);
		}
		str += len;
		w++;
	}
	return (1);
}

/**
 * @brief Splits a string into an array of substrings using a delimiter.
 *
 * Allocates and returns an array of strings by splitting `str` using the delimiter `c`.
 * The array is terminated with a NULL pointer. Returns NULL if allocation fails.
 *
 * @param str The string to split.
 * @param c The delimiter character.
 * @return char** Pointer to the array of substrings, or NULL if allocation fails.
 */
char	**ft_split(char const *str, char c)
{
	char	**res;
	int		words;

	if (!str)
		return (NULL);
	words = ft_wordcounter(str, c);
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	if (!ft_splitter(res, str, c, words))
		return (NULL);
	res[words] = NULL;
	return (res);
}
