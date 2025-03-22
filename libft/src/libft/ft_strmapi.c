#include "../../include/libft.h"

/**
 * @brief Applies a function to each character of a string.
 *
 * Allocates (with malloc) and returns a new string resulting from successive
 * applications of the function 'f'. Each character in the input string 's' is passed
 * to the function 'f' along with its index, and the result is stored in the new string.
 *
 * @param s The input string.
 * @param f The function to apply to each character; it receives the index and the character.
 * @return char* The new string resulting from the applications of 'f', or NULL if allocation fails.
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		len;
	char	*res;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
