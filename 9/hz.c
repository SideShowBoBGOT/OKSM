#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
char **DirectoryParsePath(const char *path, int *length)
{
    //  Function to parse path into separate names of directories
    //  arguments:
    //          1) path     const char*     full path to the certain directory
    //          2) length   int*            int pointer which holds an adress to a number of words in names
    //  result:
    //          names       char**          array of names of directories
    //  Example 1:
    //          input: "/"
    //          result: names = NULL, length = 0
    //  Example 2:
    //          input: "/bar/baz"
    //          result: names[0] = "bar", names[1] = "baz", length = 2

    // assign two copies of path
    char *path_c1 = path;
    char *path_c2 = path;
    length[0] = 0;
    int index = 0;
    // count how many '/' in the path
    while ((*path_c1) != '\0')
    {
        // if "/" is the path iteself then it is root directory and length would be 0
        if ((*path_c1) == '/' && strlen(path_c1) > 1)
        {
            (*length)++;
        }
        path_c1++;
    }
    char **names = NULL;
    if ((*length))
    {
        names = (char **)malloc(sizeof(char *) * (*length));
        while ((*path_c2) != '\0')
        {
            if ((*path_c2) != '/')
            {
                names[index] = (char *)malloc(sizeof(char) * 255);
                while ((*path_c2) != '/' && (*path_c2) != '\0')
                {
                    char buf[2];
                    buf[0] = (*path_c2);
                    buf[1] = '\0';
                    names[index] = strcat(names[index], buf);
                    path_c2++;
                }
                path_c2--;
                index++;
            }
            path_c2++;
        }
    }
    return names;
}
char *DirectoryReParsePath(const char *const *dir_names, int dir_names_num)
{
    //  Function to reparse separate names of directories to full path
    //  arguments:
    //          1) dir_names        const char*const*   array of the names of the directories
    //          2) dir_names_num    int                 length of the array of the names of the directories
    //  result:
    //          path                char*               path that consists of the elements of the array of the names of the directories
    //  Example 1:
    //          input:  dir_names[0] = "baz", dir_names[1] = "bar", dir_names_num = 2
    //          result: path = "/baz/bar"

    char *path = (char *)malloc(sizeof(char) * 255);
    strcat(path, "/");
    for (int i = 0; i < dir_names_num; i++)
    {
        if (i == 0)
        {
            strcat(path, dir_names[i]);
        }
        else
        {
            strcat(strcat(path, "/"), dir_names[i]);
        }
    }
    return path;
}
int main()
{
    char *path = "/lalalal/alalalala/dsfdf";
    int l = 0;
    char **names = DirectoryParsePath(path, &l);

    path = DirectoryReParsePath(names, l - 1);
    printf(path);
}