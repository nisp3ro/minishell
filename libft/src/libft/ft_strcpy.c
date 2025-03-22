/**
 * @brief Copies a null-terminated string from source to destination.
 *
 * This function copies the string pointed to by 'src' (including the terminating null byte)
 * into the buffer pointed to by 'dest'. It is assumed that 'dest' has enough space to hold
 * the source string.
 *
 * @param dest Pointer to the destination buffer where the content is to be copied.
 * @param src Pointer to the null-terminated source string to be copied.
 * @return char* Pointer to the destination string 'dest'.
 */
char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
