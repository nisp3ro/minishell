#include "../../include/libft.h"

/**
 * @brief Applies a function to each character of a string, in-place.
 *
 * Iterates through the string `s` and applies the function `f` to each character.
 * The function `f` receives the index of the character and a pointer to the character,
 * allowing modifications to be made directly to `s`.
 *
 * @param s The string to iterate over.
 * @param f The function to apply to each character, which takes an unsigned int (index) 
 *          and a pointer to a char.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
