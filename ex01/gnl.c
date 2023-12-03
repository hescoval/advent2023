#include "gnlh.h" 

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
<<<<<<< HEAD
	if (s1 != NULL)
=======
	if (s1 != NULL)	
>>>>>>> 323b6723d62e6acaef4063062331b077cb5fe17e
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
