#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>

#include "cow.h"

// Access shared struct from main
struct mini_unionfs_state {
    char *lower_dir;
    char *upper_dir;
};

#define UNIONFS_DATA ((struct mini_unionfs_state *) fuse_get_context()->private_data)

// ---------------- COPY FUNCTION ----------------
void copy_file(const char *src, const char *dest) {
    int in = open(src, O_RDONLY);
    int out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (in < 0 || out < 0) {
        perror("File open failed");
        return;
    }

    char buf[1024];
    int bytes;

    while ((bytes = read(in, buf, sizeof(buf))) > 0) {
        write(out, buf, bytes);
    }

    close(in);
    close(out);
}

// ---------------- OPEN (COW) ----------------
int cow_open(const char *path, struct fuse_file_info *fi) {
    struct mini_unionfs_state *fs = UNIONFS_DATA;

    char lower_path[PATH_MAX], upper_path[PATH_MAX];

    sprintf(lower_path, "%s%s", fs->lower_dir, path);
    sprintf(upper_path, "%s%s", fs->upper_dir, path);

    int flags = fi->flags;

    // If writing → check for CoW
    if ((flags & O_WRONLY) || (flags & O_RDWR)) {
        if (access(upper_path, F_OK) != 0 &&
            access(lower_path, F_OK) == 0) {

            printf("Copying file from lower to upper: %s\n", path);

            copy_file(lower_path, upper_path);
        }
    }

    // Open file (prefer upper)
    if (access(upper_path, F_OK) == 0)
        fi->fh = open(upper_path, flags);
    else
        fi->fh = open(lower_path, flags);

    if (fi->fh < 0)
        return -errno;

    return 0;
}

// ---------------- WRITE ----------------
int cow_write(const char *path, const char *buf, size_t size,
              off_t offset, struct fuse_file_info *fi) {

    int res = pwrite(fi->fh, buf, size, offset);

    if (res < 0)
        return -errno;

    return res;
}
