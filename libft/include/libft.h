#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

// Madatory (libc functions)
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
size_t				ft_strlen(const char *str);
void				*ft_memset(void *str, int c, size_t n);
void				ft_bzero(void *str, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
size_t				ft_strlcpy(char *dst, const char *src, size_t dsize);
char				*ft_strcat(char *dest, char *src);
size_t				ft_strlcat(char *dst, const char *src, size_t dsize);
int					ft_toupper(int ch);
int					ft_tolower(int ch);
char				*ft_strchr(const char *str, int tofind);
char				*ft_strrchr(const char *str, int tofind);
int				ft_strcmp(char *s1, char *s2);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
void				*ft_memchr(const void *str, int c, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
int					ft_memcmp(const void *str1, const void *str2, size_t n);
char				*ft_strnstr(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);
void				*ft_calloc(size_t num, size_t size);
char				*ft_strdup(const char *src);

// Mandatory (Addicional functions)
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(const char *s1, const char *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *str, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

// Bonus
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

// Extras
typedef enum e_action
{
	MALLOC,
	FREE,
	CLEAR
}					t_action;

typedef struct s_alloc
{
	void *ptr;            /**< Pointer to allocated memory */
	struct s_alloc *next; /**< Pointer to the next allocation record */
}					t_alloc;

int					ft_isspace(int c);
char				*ft_strcpy(char *dest, char *src);
char				**ft_split_charset(char const *str, char const *charset);
void				*ft_trackalloc(size_t size);
void				ft_trackedfree(void *ptr);
void				ft_clearallocs(void);
void				*ft_tracked_alloc(t_action action, void *ptr, size_t size);

#endif
