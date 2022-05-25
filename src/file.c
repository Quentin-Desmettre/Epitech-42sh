/*
** EPITECH PROJECT, 2022
** file.c
** File description:
** get the file and transform it
*/

int count_nbr(char *buff)
{
    int count = 0;

    for (int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == '\n')
            count++;
    }
    return count;
}

int lenght_line(char *buffer, int i)
{
    int j = 0;
    int bn = 0;
    int count = 0;

    for (j; i != bn; j++)
        if (buffer[j] == '\n')
            bn++;
    for (j; buffer[j] != '\n'; j++)
        count++;
    return count + 2;
}

char **str_to_word_array(char *buff)
{
    int size = count_nbr(buff);
    int a = 0;
    int j = 0;
    char **map = malloc(sizeof(char *) * (size + 1));

    (map == NULL) ? (exit(84)) : (0);
    count_word(buff);
    for (int i = 0; i != size; i++) {
        map[i] = malloc(sizeof(char) * (lenght_line(buff, i)));
        (map[i] == NULL) ? (exit(84)) : (0);
    }
    for (int i = 0; buff[i] != '\0'; i++) {
        map[j][a] = buff[i];
        a++;
        (buff[i] == '\n') ? (map[j][a] = '\0', j++, a = 0) : 0;
    }
    map[j] = 0;
    return map;
}

char *cp(char **array, int i)
{
    int j = 0;
    int size = lenght_line(array[i], 0);
    char *new_map = malloc(sizeof(char) * size);

    if (new_map == NULL)
        exit(84);
    for (j; array[i][j] != '\n'; j++)
        new_map[j] = array[i][j];
    new_map[j] = '\0';
    return new_map;
}

char *give(char *file)
{
    int i = 0;
    int size = 0;
    char *nm = NULL;
    char buff[30000];
    int fd = open(file, O_RDONLY);
    int lu = read(fd, buff, 29999);
    char **map = NULL;

    buff[lu] = '\0';
    count_word(buff);
    map = str_to_word_array(buff);
    for (int i = 0; map[i] != NULL; i++)
        size++;
    nm = cp(map, rand() % size);
    for (int i = 0; map[i] != NULL; i++)
        free(map[i]);
    free(map);
    return nm;
}
