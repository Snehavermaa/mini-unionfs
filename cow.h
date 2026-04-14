#ifndef COW_H
#define COW_H

#include <fuse.h>

int cow_open(const char *path, struct fuse_file_info *fi);
int cow_write(const char *path, const char *buf, size_t size,
              off_t offset, struct fuse_file_info *fi);

#endif
