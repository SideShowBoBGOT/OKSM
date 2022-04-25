#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#include "Directory.h"

#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED

int FileSystemGETATTR(const char *path, struct stat *st);
int FileSystemMKDIR(const char *path, mode_t mode);
int FileSystemREADDIR(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

#endif FILE_SYSTEM_H_INCLUDED