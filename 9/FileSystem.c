#include "FileSystem.h"

int FileSystemGETATTR(const char *path, struct stat *st)
{
    struct Directory *dir = DirectoryFindDir(path);
    struct File *file = DirectoryFindFile(path);
    if (dir != NULL)
    {
        st->st_uid = dir->uid;
        st->st_gid = dir->gid;
        st->st_atime = dir->atime;
        st->st_mtime = dir->mtime;
        st->st_mode = dir->mode;
        st->st_nlink = dir->n_link;
    }
    else if (file != NULL)
    {
        st->st_uid = file->uid;
        st->st_gid = file->gid;
        st->st_atime = file->atime;
        st->st_mtime = file->mtime;
        st->st_mode = file->mode;
        st->st_nlink = file->n_link;
        st->st_size = file->size;
    }
    else
    {
        return -ENOENT;
    }
    return 0;
}
int FileSystemMKDIR(const char *path, mode_t mode)
{
    // get names of directories separately
    int dir_names_num = 0;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // find parent directory
    char *parent_path = DirectoryReParsePath(dir_names, dir_names_num - 1);
    struct Directory *parent_dir = DirectoryFindDir(parent_path);
    // create new instance of Directory
    struct Directory *new_sub_dir = DirectoryInit(parent_dir, dir_names[dir_names_num - 1]);
    new_sub_dir->mode = mode | S_IFDIR;
    // add subdirectory to subdirectories array of parent directory
    DirectoryAddSubDir(parent_dir, new_sub_dir);
    // free memory allocated for dir_names
    for (int i = 0; i < dir_names_num - 1; i++)
    {
        free(dir_names[i]);
    }
    if (dir_names_num)
    {
        free(dir_names);
    }
    // free memoty allocated for parent path
    free(parent_path);
    return 0;
}
int FileSystemMKNOD(const char *path, mode_t mode, dev_t rdev)
{
    // get names of directories separately
    int dir_names_num = 0;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // find parent directory
    char *parent_path = DirectoryReParsePath(dir_names, dir_names_num - 1);
    struct Directory *parent_dir = DirectoryFindDir(parent_path);
    // create new instance of Directory
    struct File *new_file = DirectoryFileInit(parent_dir, dir_names[dir_names_num - 1]);
    new_file->mode = mode | S_IFREG;
    // add subdirectory to subdirectories array of parent directory
    DirectoryAddFile(parent_dir, new_file);
    // free memory allocated for dir_names
    for (int i = 0; i < dir_names_num - 1; i++)
    {
        free(dir_names[i]);
    }
    if (dir_names_num)
    {
        free(dir_names);
    }
    // free memoty allocated for parent path
    free(parent_path);
    return 0;
}
int FileSystemREAD(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    struct File *file = DirectoryFindFile(path);
    if (file != NULL && file->content != NULL)
    {
        char *content = file->content;
        memcpy(buffer, content + offset, size);
        return strlen(content) - offset;
    }
    return -1;
}
int FileSystemWRITE(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
    struct File *file = DirectoryFindFile(path);
    if (file != NULL)
    {
        if (file->content != NULL)
        {
            free(file->content);
        }
        file->content = (char *)malloc(sizeof(char));
        file->content[0] = '\0';
        strcpy(file->content, buffer);
    }
    return size;
}
int FileSystemREADDIR(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    filler(buffer, ".", NULL, 0);  // Current Directory
    filler(buffer, "..", NULL, 0); // Parent Directory

    struct Directory *dir = DirectoryFindDir(path);
    if (dir != NULL)
    {
        for (int i = 0; i < dir->n_link - 2; i++)
        {
            filler(buffer, dir->sub_dirs[i]->name, NULL, 0);
        }
        for (int i = 0; i < dir->files_num; i++)
        {
            filler(buffer, dir->files[i]->name, NULL, 0);
        }
    }

    return 0;
}