#include "../../include/get_next_line.h"

/**
 * @brief Frees a pointer and sets it to NULL.
 *
 * This helper function frees the memory pointed to by *str, sets the pointer
 * to NULL, and returns NULL. It is used to clean up the saved buffer.
 *
 * @param str Pointer to the pointer to free.
 * @return char* Always returns NULL.
 */
char *ft_free(char **str)
{
    free(*str);
    *str = NULL;
    return (NULL);
}

/**
 * @brief Updates the saved buffer by removing the extracted line.
 *
 * This function finds the first newline character in the saved buffer and
 * creates a new string containing the remainder of the buffer (after the newline).
 * If no newline is found or if there is nothing left after the newline,
 * the saved buffer is freed.
 *
 * @param save The saved buffer containing previously read data.
 * @return char* The updated saved buffer, or NULL if there is no remaining data.
 */
char *ft_subs_line_from_save(char *save)
{
    char *newsave;
    char *ptr;
    int len;

    ptr = ft_strchr_gnl(save, '\n');
    if (!ptr)
    {
        newsave = NULL;
        return (ft_free(&save));
    }
    else
        len = (ptr - save) + 1;
    if (!save[len])
        return (ft_free(&save));
    newsave = ft_substr_gnl(save, len, ft_strlen_gnl(save) - len);
    ft_free(&save);
    if (!newsave)
        return (NULL);
    return (newsave);
}

/**
 * @brief Extracts a line (up to and including the newline) from the saved buffer.
 *
 * Finds the first newline character in the saved buffer and returns a new string
 * containing the characters from the start of the buffer up to and including the newline.
 *
 * @param save The saved buffer containing data read from the file descriptor.
 * @return char* A new string representing the next line, or NULL on allocation failure.
 */
char *ft_get_line(char *save)
{
    char *line;
    char *ptr;
    int len;

    ptr = ft_strchr_gnl(save, '\n');
    len = (ptr - save) + 1;
    line = ft_substr_gnl(save, 0, len);
    if (!line)
        return (NULL);
    return (line);
}

/**
 * @brief Reads from the file descriptor until a newline is found.
 *
 * Allocates a buffer and reads data from the file descriptor until either a newline is encountered
 * or the end-of-file is reached. The newly read data is appended to the saved buffer.
 *
 * @param fd The file descriptor to read from.
 * @param save The current saved buffer.
 * @return char* The updated saved buffer with newly read data appended, or NULL on error.
 */
char *ft_read_till_n(int fd, char *save)
{
    int readed;
    char *buffer;

    readed = 1;
    buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (ft_free(&save));
    buffer[0] = '\0';
    /* Loop until a newline is encountered in the buffer */
    while (readed > 0 && !ft_strchr_gnl(buffer, '\n'))
    {
        readed = read(fd, buffer, BUFFER_SIZE);
        if (readed > 0)
        {
            buffer[readed] = '\0';
            save = ft_strjoin_modded(save, buffer);
        }
    }
    free(buffer);
    if (readed == -1)
        return (ft_free(&save));
    return (save);
}

/**
 * @brief Retrieves the next line from a file descriptor.
 *
 * This function reads from the file descriptor and returns the next line,
 * including the newline character if present. It uses a static variable to maintain
 * state between calls. When there is no more data to read, it returns NULL.
 *
 * @param fd The file descriptor to read from.
 * @return char* The next line from the file descriptor, or NULL if there is no more data or on error.
 */
char *get_next_line(int fd)
{
    static char *save;
    char *line;

    if (fd < 0)
        return (NULL);
    if ((save && !ft_strchr_gnl(save, '\n')) || !save)
        save = ft_read_till_n(fd, save);
    if (!save)
        return (NULL);
    line = ft_get_line(save);
    if (!line)
        return (ft_free(&save));
    save = ft_subs_line_from_save(save);
    return (line);
}
