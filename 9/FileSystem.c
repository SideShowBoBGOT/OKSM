#include "FileSystem.h"

int FileSystemGETATTR(const char *path, struct stat *st)
{
    struct Directory *dir = DirectoryFindDir(path);
    if (dir != NULL)
    {
        st->st_uid = dir->uid;
        st->st_gid = dir->gid;
        st->st_atime = dir->atime;
        st->st_mtime = dir->mtime;
        st->st_mode = dir->mode;
        st->st_nlink = dir->n_link;

        return 0;
    }
    return -ENOENT;
}
int FileSystemMKDIR(const char *path, mode_t mode)
{
    // get names of directories separately
    int dir_names_num = 2;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // find parent directory
    dir_names_num -= 1;
    char *parent_path = DirectoryReParsePath(dir_names, dir_names_num);
    struct Directory *parent_dir = DirectoryFindDir(parent_path);
    // create new instance of Directory
    struct Directory *new_sub_dir = DirectoryInit(parent_dir, dir_names[dir_names_num]);
    new_sub_dir->mode = 0777 | S_IFDIR;
    // add subdirectory to subdirectories array of parent directory
    DirectoryAddSubDir(parent_dir, new_sub_dir);
    // free memory allocated for dir_names
    dir_names_num++;
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
int FileSystemREADDIR(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    filler(buffer, ".", NULL, 0);  // Current Directory
    filler(buffer, "..", NULL, 0); // Parent Directory

    struct Directory *dir = DirectoryFindDir(path);
    for (int i = 0; i < dir->n_link - 2; i++)
    {
        filler(buffer, dir->sub_dirs[i]->name, NULL, 0);
    }

    return 0;
}