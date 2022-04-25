#include "FileSystem.h"
#include "Directory.h"

struct fuse_operations FileSystemOperations = {
    .getattr = FileSystemGETATTR,
    .readdir = FileSystemREADDIR,
    .mkdir = FileSystemMKDIR,
};
int main(int argc, char *argv[])
{
    root_dir = DirectoryInit(&root_dir, "/");
    return fuse_main(argc, argv, &FileSystemOperations, NULL);
}