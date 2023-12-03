#include "gnlh.h" 

static void	free_ret(char **ret)
{
	int	i;

	i = 0;
	while (ret[i])
		free(ret[i++]);
	free(ret);
}

static void	fill_strings(char **strings, const char *s, char c)
{
	int	i;
	int	k;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		k = 0;
		while (s[i] != c && s[i])
		{
			strings[j][k] = s[i];
			k++;
			i++;
		}
		if (k != 0)
		{
			strings[j][k] = '\0';
			j++;
			continue ;
		}
		i++;
	}
	strings[j] = NULL;
}

static int	check_splits(char const *s, char c)
{
	int	splits;
	int	i;

	i = 0;
	splits = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			while (s[i] != c && s[i])
				i++;
			splits++;
			continue ;
		}
		i++;
	}
	return (splits);
}

static void	make_strings(char **ret, const char *s, char c)
{
	size_t	i;
	int		j;
	int		k;

	k = 0;
	i = 0;
	while (s[i])
	{
		j = 0;
		while (s[i] != c && s[i])
		{
			j++;
			i++;
		}
		if (j > 0)
		{
			ret[k] = malloc(j + 1);
			if (ret[k] == NULL)
				return ;
			k++;
			continue ;
		}
		i++;
	}
}

char	**split(char const *s, char c)
{
	char	**ret;
	int		i;

	i = 0;
	ret = (char **) malloc(sizeof(char *) * (check_splits(s, c) + 1));
	if (ret == NULL)
		return (NULL);
	make_strings(ret, s, c);
	while (i < check_splits(s, c))
	{
		if (ret[i] == NULL)
		{
			free_ret(ret);
			return (NULL);
		}
		i++;
	}
	fill_strings(ret, s, c);
	return (ret);
}

size_t	ft_strlen(char *s)
{ 
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
 } 

int	ft_strcontains(char *s, char c)
{
		unsigned int	i;

		if (s == NULL)
			return (0);
		i = 0;
		while (s[i] != '\0')
		{
			if (s[i] == c)
				return (1);
			i++;
		}
		return (0);
}

char	*ft_strcpy_nl(char *dest, char *src, int nl_stop) 
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		if (nl_stop && src[i] == '\n')
		{
			i++;
			break ;
		}
		i++;
	}
		dest[i] = '\0';
		return (dest);
}

char	*ft_strjoin_special(char *s1, char *s2)
{
	char	*join;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2);
	join = malloc(sizeof(char) * (len + 1));
	if (join == NULL)
		return (NULL);
	i = 0;
	while (s1 != NULL && s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 != NULL && s2[j] != '\0')
		join[i++] = s2[j++];
	join[len] = '\0';
	if (s1 != NULL)
		free(s1);
	return (join);
}

char	*ft_fill(int fd, char *data)
{
	int		bytes_read;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (buffer == NULL)
		return (NULL);
	bytes_read = 1;
	while (!ft_strcontains(data, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			if (data != NULL)
				free(data);
			return (NULL);
		}
			buffer[bytes_read] = '\0';
			data = ft_strjoin_special(data, buffer);
	}
	free(buffer);
	return (data);
}

char		*ft_extract_line(char *data)
{
	char		*line;
	size_t		len;
	int			i;

	if (data[0] == '\0')
		return (NULL);
	i = 0;
	while (data[i] != '\0' && data[i] != '\n')
		i++;
	len = i;
	if(data[i] == '\n')
		len++;
	line = malloc(len + 1);
	if (line == NULL)
		return (NULL);
	ft_strcpy_nl(line, data, 1);
	return (line);
}

char	*ft_update_data(char *old_data)
{
	char		*new_data;
	int			start;

	start = 0;
	while (old_data[start] != '\0' && old_data[start] != '\n')
		start++;
	if (old_data[start] == '\0')
	{
		free(old_data);
		return (NULL);
	}
	new_data = malloc(sizeof(char) * (ft_strlen(old_data + start) + 1));
	if (new_data == NULL)
		return (NULL);
	ft_strcpy_nl(new_data, old_data + start + 1, 0);
	free(old_data);
	return (new_data);
}

char	*get_next_line(int fd)
{ 
		char		*line;
		static char	*data;

		if (fd < 0 || BUFFER_SIZE <= 0)
				return (NULL);
		data = ft_fill(fd, data);
		if (data == NULL)
			return (NULL);
		line = ft_extract_line(data);
		data = ft_update_data(data);
		return (line);
}
