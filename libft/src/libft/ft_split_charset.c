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
	{
		free(res[i]);
		i++;
	}
	free(res);
}

/**
 * @brief Checks if a character is present in the charset.
 *
 * Iterates through the string `charset` and returns 1 if the character `ch` is found,
 * otherwise returns 0.
 *
 * @param ch The character to check.
 * @param charset A string containing all separator characters.
 * @return int Returns 1 if `ch` is a separator, otherwise 0.
 */
static int	ft_is_separator(char ch, char const *charset)
{
	while (*charset)
	{
		if (ch == *charset)
			return (1);
		charset++;
	}
	return (0);
}

/**
 * @brief Counts the number of words in a string using a charset as separators.
 *
 * Iterates through the string `str` and counts words, where a word is a sequence of characters
 * that are not present in `charset`.
 *
 * @param str The input string.
 * @param charset A string containing separator characters.
 * @return int The number of words found.
 */
static int	ft_wordcounter(char const *str, char const *charset)
{
	int	words = 0;

	while (*str)
	{
		/* Skip all separator characters */
		while (*str && ft_is_separator(*str, charset))
			str++;
		/* If we're not at the end, we've found a word */
		if (*str)
		{
			words++;
			while (*str && !ft_is_separator(*str, charset))
				str++;
		}
	}
	return (words);
}

/**
 * @brief Splits the string into words and allocates each word.
 *
 * Uses ft_substr to allocate and copy each word from `str` (separated by any character in `charset`)
 * into the array `res`. If allocation fails for any word, frees all previously allocated words.
 *
 * @param res The array to store allocated words.
 * @param str The input string.
 * @param charset A string containing separator characters.
 * @param words The total number of words expected.
 * @return int Returns 1 on success, or 0 on memory allocation failure.
 */
static int	ft_splitter(char **res, char const *str, char const *charset, int words)
{
	int w = 0;
	int len;

	while (w < words)
	{
		/* Skip separator characters */
		while (*str && ft_is_separator(*str, charset))
			str++;
		len = 0;
		/* Determine the length of the current word */
		while (str[len] && !ft_is_separator(str[len], charset))
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
 * @brief Splits a string into an array of substrings using a charset as delimiters.
 *
 * Allocates and returns an array of strings by splitting `str` using any of the characters found
 * in `charset` as separators. The array is terminated with a NULL pointer.
 * Returns NULL if allocation fails.
 *
 * @param str The string to split.
 * @param charset A string containing separator characters.
 * @return char** Pointer to the array of substrings, or NULL if allocation fails.
 */
char	**ft_split_charset(char const *str, char const *charset)
{
	char	**res;
	int		words;

	if (!str)
		return (NULL);
	words = ft_wordcounter(str, charset);
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	if (!ft_splitter(res, str, charset, words))
		return (NULL);
	res[words] = NULL;
	return (res);
}
