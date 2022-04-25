// author: choleraplague
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

struct Directory
{
    //  !!!NOTE!!!    To see what these attributes mean     !!!NOTE!!!
    //  !!!NOTE!!!    see the next link:                    !!!NOTE!!!
    //  https://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html

    //  Specifies the name of the file.
    char *name;
    //  The user ID of the file’s owner.
    uid_t uid;
    //  The group ID of the file.
    gid_t gid;
    //  This is the last access time for the file.
    time_t atime;
    //  This is the time of the last modification to the contents of the file.
    time_t mtime;
    //  Specifies the mode of the file.
    //  This includes file type informationand the file permission bits
    mode_t mode;
    //  The number of hard links to the file.
    //  This count keeps track of how many directories have entries for this file.
    nlink_t n_link;
    //  Pointer to parent directory
    struct Directory *parent_dir;
    //  Pointer to itself
    struct Directory *self_dir;
    //  Array of subdirectories
    struct Directory **sub_dirs;
    //  Array of files in the directory
    struct File *files;
};
struct File
{
    char *path;
    char *name;
    uid_t uid;
    gid_t gid;
    time_t atime;
    time_t mtime;
    mode_t mode;
    nlink_t n_link;
    off_t size;
    struct Directory *parent_dir;
};
struct Directory *root_dir;
char *DirectoryGiveStr(const char *str);
struct Directory *DirectoryFindDir(const char *path);
struct Directory *DirectoryInit(struct Directory *parent_dir, const char *name);
char **DirectoryParsePath(const char *path, int *length);
char *DirectoryReParsePath(const char *const *dir_names, int dir_names_num);
void DirectoryAddSubDir(struct Directory *parent_dir, struct Directory *new_sub_dir);
struct File *FileInit(struct Directory *parent_dir, const char *name);

#endif DIRECTORY_H_INCLUDED