#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>   // for access()
#include <limits.h>   // for PATH_MAX

struct mini_unionfs_state {
    char *lower_dir;
    char *upper_dir;
};

#define UNIONFS_DATA ((struct mini_unionfs_state *) fuse_get_context()->private_data)

// Dummy resolve (Person 2 will implement)
int resolve_path(const char *path, char *resolved_path) {
    struct mini_unionfs_state *fs = UNIONFS_DATA;

    char upper_path[PATH_MAX];
    char lower_path[PATH_MAX];
    char whiteout_path[PATH_MAX];

    // Construct paths
    snprintf(upper_path, PATH_MAX, "%s%s", fs->upper_dir, path);
    snprintf(lower_path, PATH_MAX, "%s%s", fs->lower_dir, path);

    // Whiteout path: upper_dir/.wh.filename
    const char *filename = strrchr(path, '/');
    if (filename != NULL) {
        filename++; // skip '/'
    } else {
        filename = path;
    }

    char dir_path[PATH_MAX];
    strncpy(dir_path, path, PATH_MAX);

    char *last_slash = strrchr(dir_path, '/');
    if (last_slash != NULL && last_slash != dir_path) {
        *last_slash = '\0';
    } else {
        strcpy(dir_path, "");
    }

    snprintf(whiteout_path, PATH_MAX, "%s%s/.wh.%s",
             fs->upper_dir, dir_path, filename);

    // 1. Check whiteout → hide file
    if (access(whiteout_path, F_OK) == 0) {
        return -1;
    }

    // 2. Check upper layer
    if (access(upper_path, F_OK) == 0) {
        strcpy(resolved_path, upper_path);
        return 0;
    }

    // 3. Check lower layer
    if (access(lower_path, F_OK) == 0) {
        strcpy(resolved_path, lower_path);
        return 0;
    }

    // 4. Not found
    return -1;
}

// getattr
static int unionfs_getattr(const char *path, struct stat *stbuf,
                          struct fuse_file_info *fi) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    return -ENOENT;
}

// readdir
static int unionfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                          off_t offset, struct fuse_file_info *fi,
                          enum fuse_readdir_flags flags) {

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    return 0;
}

// operations
static struct fuse_operations unionfs_oper = {
    .getattr = unionfs_getattr,
    .readdir = unionfs_readdir,
};

// main
int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &unionfs_oper, NULL);
}
