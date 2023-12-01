#include "gnlh.h" 

size_t	ft_strlen(char *s)		//Straightforward strlen
{ 
	size_t	i;

	if (s == NULL)		//Make sure we check if its NULL before playing with it (Invalid read size 1)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
 } 

int	ft_strcontains(char *s, char c)		//Straightforward strchr
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

char	*ft_strjoin_special(char *s1, char *s2)	//	Straightforward strjoin, check for s1 == NULL and s2 == NULL in loops
{
	char	*join;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2);		//	Calculate length of return string
	join = malloc(sizeof(char) * (len + 1));	//	Malloc with +1 for Null
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
	if (s1 != NULL)	// s1 is our buffer, its about to be replaced. free it
		free(s1);
	return (join);
}

char	*ft_fill(int fd, char *data)
{
	int		bytes_read;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);		// Allocate enough memory for all we want to read, plus the '/0'
	if (buffer == NULL)
		return (NULL);
	bytes_read = 1;			// Jumpstart the loop
	while (!ft_strcontains(data, '\n') && bytes_read != 0)		//strchr(buff, \n) && read more than one byte
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)		//Deal with error thrown by read
		{
			free(buffer);			//Free our buffer
			if (data != NULL)		//If data is not already NULL, free.
				free(data);
			return (NULL);
		}
			buffer[bytes_read] = '\0';	//Put the NULL at the end of our buffer
			data = ft_strjoin_special(data, buffer);		//strjoin our static var with buffer
	}
	free(buffer);		//Buffer done its job, free.
	return (data);
}

char		*ft_extract_line(char *data)
{
	char		*line;
	size_t		len;
	int			i;

	if (data[0] == '\0')	//If the first character of the static var is a '\0', the file is over, return NULL.
		return (NULL);
	i = 0;
	while (data[i] != '\0' && data[i] != '\n')	// Copy until we reach either a '\0' or a '\n'
		i++;
	len = i;
	if(data[i] == '\n')	// As requested, if the line ends in a new line, we must include it.
		len++;
	line = malloc(len + 1);	// Normal malloc
	if (line == NULL)
		return (NULL);
	ft_strcpy_nl(line, data, 1); // Copy everything up until the newline
	return (line);
}

char	*ft_update_data(char *old_data)
{
	char		*new_data;
	int			start;

	start = 0;									//Simple way to check if everything was printed or not.
	while (old_data[start] != '\0' && old_data[start] != '\n')			//Updating data	
		start++;								//Old data is "Simple\nTest"	//Old data is "SimpleTest"
	if (old_data[start] == '\0')							//Line was Simple\n		//Line was SimpleTest
	{										//New data should be Test	//New data shoulde be NULL
		free(old_data);												//since everything was printed
		return (NULL);
	}
	new_data = malloc(sizeof(char) * (ft_strlen(old_data + start) + 1));	//Calculate the length of the string at the start point.
	if (new_data == NULL)
		return (NULL);
	ft_strcpy_nl(new_data, old_data + start + 1, 0);	//Copy the string into the new malloc from the position after '\n'
	free(old_data);						//free old buffer, since its being replaced.
	return (new_data);
}

char	*get_next_line(int fd)
{ 
		char		*line;
		static char	*data;

		if (fd < 0 || BUFFER_SIZE <= 0) // Deal with invalid fd and buffer sizes
				return (NULL);
		data = ft_fill(fd, data);		// Fill up our static var
		if (data == NULL)				// If static var return NULL, return NULL.
			return (NULL);
		line = ft_extract_line(data);	// Get the line that user wants returned
		data = ft_update_data(data);	// Update the Static var with rest of content
		return (line);
}
